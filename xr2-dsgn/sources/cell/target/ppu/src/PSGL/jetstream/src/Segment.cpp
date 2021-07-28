/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <PSGL/private.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/Malloc.h>
#include <PSGL/PerfCounters.h>

// worst case, this buffer needs to be able to hold 4 verts (including stream padding)
#define _JS_SEGMENT_BUFFER_SIZE 0x10000 // 64KB

static char* _jsSegmentBuffer = NULL;

void _jsSegmentInit( void )
{
    // The segment buffer is allocated the first time it is used.
}

void _jsSegmentExit( void )
{
    // free the segment buffer
    if ( _jsSegmentBuffer != NULL )
        jsFree( _jsSegmentBuffer );
    _jsSegmentBuffer = NULL;
}


void _jsComputeMinMaxIndices( PSGLcontext* LContext, GLuint* min, GLuint* max, const void* indices, GLenum indexType, GLsizei count )
{
    _JS_PERFORMANCE_START( ComputeMinMaxIndicesCounter );
    GLuint minIndex = 0, maxIndex = 0;

    if ( count < 1 ) { *min = 0; *max = 0; return;}
    char*origin = NULL;
    if ( LContext->attribs->ElementArrayBuffer )
    {
        jsBufferObject *buffer = _jsGetBufferObject( LContext, LContext->attribs->ElementArrayBuffer );
        // no matter what, we need the latest updated data. This may mean wait for
        // the GPU that is waiting on SPU or another thread to upload the data. Basically
        // do a full finish.
        _jsPlatformRasterFinish();
        origin = _jsPlatformBufferObjectMap( buffer, GL_READ_ONLY );
        _JS_REPORT_EXTRA( PSGL_REPORT_VERTEX_SLOW_PATH, "Reading back indices from GPU memory (slow), to compute min and max. Use glDrawRangeElements" );
    }

    switch ( indexType )
    {
#define _JS_SCAN(TYPE) \
	do { \
		TYPE* iptr = (TYPE*)(origin+((GLubyte *)indices-(GLubyte *)NULL)); \
		int i; \
		for (i=0;i<count;++i) \
		{ \
			if ((!LContext->attribs->PrimitiveRestart)||(iptr[i]!=LContext->attribs->RestartIndex)) \
			{ \
				minIndex = iptr[i]; \
				maxIndex = iptr[i];	\
				break; \
			} \
		} \
		++i; \
		for (; i < count; ++i) \
		{ \
			if ((!LContext->attribs->PrimitiveRestart)||(iptr[i]!=LContext->attribs->RestartIndex)) \
			{ \
				if (minIndex > iptr[i]) \
					minIndex = iptr[i]; \
				if (maxIndex < iptr[i]) \
					maxIndex = iptr[i]; \
			} \
		} \
	} while (0)

        case GL_UNSIGNED_BYTE:
            _JS_SCAN( GLubyte );
            break;
        case GL_UNSIGNED_SHORT:
            _JS_SCAN( GLushort );
            break;
        case GL_UNSIGNED_INT:
            _JS_SCAN( GLuint );
            break;
#undef _JS_SCAN
    }

    if ( LContext->attribs->ElementArrayBuffer )
    {
        jsBufferObject *buffer = _jsGetBufferObject( LContext, LContext->attribs->ElementArrayBuffer );
        _jsPlatformBufferObjectUnmap( buffer );
    }
    *min = minIndex;
    *max = maxIndex;
    _JS_PERFORMANCE_STOP( ComputeMinMaxIndicesCounter );
}

// Brute force method, the least likely to break.
// Goal is to copy all vertex data needed for the current segment in a global buffer.
// (potentially deindexing), and draw with that.
// This function decomposes a large draw into smaller DrawArrays calls.  It
// temporarily moves aside the client state and substitutes a client state
// for the draw subsets.
void _jsDrawSlow( jsDrawParams* dparams )
{
    PSGLcontext* LContext = _CurrentContext;
    GLsizei nVertices = ( dparams->indexType ) ? dparams->indexCount : dparams->vertexCount;
    const GLvoid* indices = dparams->indices;

    _JS_REPORT( PSGL_REPORT_VERTEX_SLOW_PATH );

    // allocate buffer space
    if ( _jsSegmentBuffer == NULL )
        _jsSegmentBuffer = ( char * )jsMalloc( _JS_SEGMENT_BUFFER_SIZE );
    if ( _jsSegmentBuffer == NULL )
    {
        _jsSetError( GL_OUT_OF_MEMORY );
        return;
    }

    // backup of current jsAttributeState configuration
    jsAttributeState* asLC = LContext->attribs;
    GLuint attribSetNameBackup = LContext->attribSetName;

    LContext->attribs = &LContext->defaultAttribs0;
    LContext->attribSetName = 0;
    LContext->attribSetDirty = GL_FALSE;

    // backup copy of default jsAttributeState to restore when done
    jsAttributeState asBackup = *( LContext->attribs );

    // if using indices VBO, map it.
    if ( dparams->indexType != 0 && asLC->ElementArrayBuffer )
    {
        jsBufferObject *buffer = _jsGetBufferObject( LContext, asLC->ElementArrayBuffer );

        // no matter what, we need the latest updated data. This may mean wait for
        // the GPU that is waiting on SPU or another thread to upload the data. Basically
        // do a full finish.
        _jsPlatformRasterFinish();

        const char* bufferBase = _jsPlatformBufferObjectMap( buffer, GL_READ_ONLY );
        JS_ASSERT( bufferBase );

        indices = bufferBase + (( const GLubyte* )( dparams->indices ) - ( const GLubyte* )NULL );
    }

    // determine new client state
    GLuint vertexSize = 0;
    GLuint streamCount = 0;
    for ( int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
    {
        // skip this attribute if not enabled
        if ( ! JSBIT_GET( asLC->EnabledMask, i ) ) continue;

        jsAttribute* attrLC = asLC->attrib + i;

        ++streamCount;

        // XXX This code would preserve the client type if it is
        // natively supported, but at the moment _jsConvertStream makes
        // everything GLfloat.
        attrLC->clientType = GL_FLOAT;

        // we always pack the streams
        attrLC->clientStride = attrLC->clientSize * _jsGetTypeSize( attrLC->clientType );
        attrLC->arrayBuffer = 0;
        attrLC->frequency = 1;
        vertexSize += attrLC->clientStride;
    }
    asLC->NeedsConversionMask &= ~asLC->EnabledMask;
    asLC->HasVBOMask &= ~asLC->EnabledMask;
    asLC->DirtyMask |= asLC->EnabledMask;
    asLC->ModuloMask = 0;
    JS_ASSERT( _jsCheckAttribCaches( asLC ) );

    // ensure modified attribs will be updated after restore.
    asBackup.DirtyMask |= asLC->EnabledMask;

    // determine the maximum number of vertices in the segment buffer
    //  Pointers in the buffer must be aligned to word boundaries, so we
    //  subtract off the maximum amount of pad before dividing the segment
    //  buffer size by the bytes per vertex.
    GLuint maxVertices =
        ( _JS_SEGMENT_BUFFER_SIZE - streamCount * sizeof( GLfloat ) ) / vertexSize;
    JS_ASSERT( maxVertices >= 4 );

    // setup the rest of the vertex clientstate
    GLuint offset = 0;
    for ( int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
    {
        if ( ! JSBIT_GET( asLC->EnabledMask, i ) ) continue;

        jsAttribute* attrLC = asLC->attrib + i;
        attrLC->clientData = _jsSegmentBuffer + offset;
        offset += attrLC->clientStride * maxVertices;
        offset = _jsPad( offset, sizeof( GLfloat ) );
    }
    JS_ASSERT( offset <= _JS_SEGMENT_BUFFER_SIZE );

    // set configuration for primitive type
    GLuint multiple = 1;
    GLuint repeatFirst = 0;
    GLuint repeatLast = 0;
    switch ( dparams->mode )
    {
        case GL_POINTS:
            break;
        case GL_LINES:
            multiple = 2;
            break;
        case GL_LINE_LOOP:
            repeatLast = 1;
            break;
        case GL_LINE_STRIP:
            repeatLast = 1;
            break;
        case GL_TRIANGLES:
            multiple = 3;
            break;
        case GL_TRIANGLE_STRIP:
            multiple = 2;
            repeatLast = 2;
            break;
        case GL_TRIANGLE_FAN:
            repeatFirst = 1;
            repeatLast = 1;
            break;
        case GL_QUADS:
            multiple = 4;
            break;
        case GL_QUAD_STRIP:
            multiple = 2;
            repeatLast = 2;
            break;
        default:
            JS_ASSERT( 0 );
            break;
    }

    // number of new vertices that are processed in a single segment
    const int verticesPerSegment = maxVertices - ( maxVertices % multiple ) - repeatLast - repeatFirst;

    GLboolean restart = GL_TRUE;
    GLuint restartIndex = dparams->firstVertex;

    // create DrawArrays
    jsDrawParams dp2 =
        {
        mode        : dparams->mode,
        firstVertex : 0,
        vertexCount : 0,
        indexMin    : 0,
        indexMax    : 0,
        indexCount  : 0,
        indexType   : 0,
        indices     : NULL,
        };

    // The number of vertices transfered on each segment (does not count first)
    for ( GLint copyIndex = restartIndex + repeatFirst; copyIndex < nVertices; )
    {
        GLint perSegmentTransferCount = MIN( nVertices - copyIndex, verticesPerSegment );

        // corner case
        if ( restartIndex + repeatFirst > ( GLuint )nVertices ) break;

        JS_ASSERT( perSegmentTransferCount > 0 );

        GLint count = 0;
        GLboolean gotRestart = GL_FALSE;
        GLint drawCount = 0;
        for ( int i = 0; i < _JS_MAX_VERTEX_ATTRIBS; ++i )
        {
            if ( ! JSBIT_GET( asLC->EnabledMask, i ) ) continue;

            // for the first pass, pull in the repeat first once, if needed.
            // this will not be done again on next segments, as the data will already be there
            if ( restart && repeatFirst != 0 )
            {
                count = _jsConvertStream( asLC, &asBackup, i, 0, restartIndex, repeatFirst, indices, dparams->indexType );
                if ( count < ( int )repeatFirst )
                {
                    // we got a restart index
                    // keep drawCount to 0 and break, we don't have enough data to draw a primitive
                    gotRestart = GL_TRUE;
                    break;
                }
            }

            // need to copy the data. to the client array we setup
            count = _jsConvertStream( asLC, &asBackup, i, repeatFirst, copyIndex, perSegmentTransferCount, indices, dparams->indexType );
            if ( count < perSegmentTransferCount )
            {
                // we got a restart index
                gotRestart = GL_TRUE;
            }
            drawCount = count + repeatFirst;
        }

        // create a DrawArrays invocation
        dp2.vertexCount = drawCount;
        _jsPlatformRequiresSlowPath( &dp2, 0, 0 );	// must always call before _jsPlatformDraw() to setup jsDrawParams
        _jsPlatformDraw( &dp2 );

        copyIndex += count;
        if ( gotRestart )
        {
            // now copyIndex references the restart index
            ++copyIndex;

            // now copyIndex references the start of the new buffer
            restartIndex = copyIndex - repeatFirst;
        }

        // set restart for next round
        restart = gotRestart;
    }

    // unmap the indices VBO
    if ( dparams->indexType != 0 && asLC->ElementArrayBuffer )
    {
        jsBufferObject *buffer = _jsGetBufferObject( LContext, asLC->ElementArrayBuffer );
        _jsPlatformBufferObjectUnmap( buffer );
    }

    // restore previous jsAttributeState from backups
    *( LContext->attribs ) = asBackup;
    LContext->attribs = asLC;
    LContext->attribSetName = attribSetNameBackup;
    LContext->attribSetDirty = ( attribSetNameBackup != 0 );

    JS_ASSERT( _jsCheckAttribCaches( asLC ) );
}
