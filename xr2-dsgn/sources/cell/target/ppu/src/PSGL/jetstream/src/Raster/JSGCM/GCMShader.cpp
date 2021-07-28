/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * 
 */

#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/private.h>
#include <JSGCM/JSGcmUtils.h>
#include <JSGCM/JSGcmFifoUtils.h>
#include <JSGCM/JSGcmShader.h>
#include <JSGCM/JSGcmDriver.h>
#include <Cg/CgCommon.h>
#include <PSGL/platform.h>
// this is for some strings functions, like atoi
#include <stdlib.h>
#include <string.h>

void _jsCreatePushBuffer( _CGprogram *program );
void _jsSetDefaultValuesFP( _CGprogram *program );
void _jsSetDefaultValuesVP( _CGprogram *program );

#if JS_ENDIAN == JS_BIG_ENDIAN
#define ENDIAN_32(X, F) ((F) ? endianSwapWord(X) : (X))
#else
#define ENDIAN_32(X, F) (X)
#endif

int _parameterAlloc = 0;
int _ucodeAlloc = 0;

int _jsGetTypeResource( _CGprogram* program, unsigned short typeIndex, short *resourceIndex );
int _jsGetTypeResourceID( _CGprogram* program, unsigned short typeIndex );
int _jsGetTypeResourceRegisterCountVP( _CGprogram* program, short resourceIndex, int resourceCount, unsigned short *resource );
void _jsPrepareFPSharedPushBuffer( GLuint subchannel, _CGprogram *program, uint32_t * &current );

// uploads the given fp shader to gpu memory. Allocates if needed.
// This also builds the shared constants push buffer if needed, since it depends on the load address
int _jsGcmLoadFPShader( jsGcmShader *shader )
{
    //TODO: still used for the fixed pipeline
    //JS_ASSERT(0 && "shouldn't be used anymore");
    CgBinaryProgram *cgProgram = &shader->program;
    char *ucode;
    unsigned int ucodeSize;
    _jsGcmGetUCode(( CGprogram )cgProgram, &ucode, &ucodeSize );
    if ( shader->loadAddressId == GMM_ERROR )
    {
        shader->loadAddressId = _jsGcmAllocateMemory( ucodeSize );
#ifndef JS_NO_ERROR_CHECK
        if ( shader->loadAddressId == GMM_ERROR )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            JS_ASSERT( 0 );  // XXX  how do we need to handle that ? Should we drop shaders/textures ?
            return GL_FALSE;
        }
#endif
    }
    // always upload shader
    _jsGcmSend( shader->loadAddressId, 0, 0, ucode, ucodeSize );
    return GL_TRUE;
}


// uploads the given fp shader to gpu memory. Allocates if needed.
// This also builds the shared constants push buffer if needed, since it depends on the load address
int _jsLoadFPShader( _CGprogram *program )
{
    unsigned int ucodeSize = program->header.instructionCount * 16;
    if ( program->loadProgramId == GMM_ERROR )
    {
        program->loadProgramId = _jsGcmAllocateMemory( ucodeSize );
        program->loadProgramOffset = 0;
#ifndef JS_NO_ERROR_CHECK
        if ( program->loadProgramId == GMM_ERROR )
        {
            _jsSetError( GL_OUT_OF_MEMORY );
            JS_ASSERT( 0 );  // XXX  how do we need to handle that ? Should we drop shaders/textures ?
            return GL_FALSE;
        }
#endif
    }
    // always upload shader
    _jsGcmSend( program->loadProgramId, program->loadProgramOffset, 0, ( char* )program->ucode, ucodeSize );
    return GL_TRUE;
}

void _jsUnloadFPShader( _CGprogram *program )
{
    if ( program->loadProgramId != GMM_ERROR )
    {
        _jsGcmFreeMemory( program->loadProgramId );
        program->loadProgramId = GMM_ERROR;
        program->loadProgramOffset = 0;
    }
}

void _jsGcmUnloadFPShader( jsGcmShader *shader )
{
    if ( shader->loadAddressId != GMM_ERROR )
    {
        _jsGcmFreeMemory( shader->loadAddressId );
        shader->loadAddressId = GMM_ERROR;
	}
}

void _jsGcmSetParameter4fv( jsGcmShader *shader, CgBinaryParameter *p, const GLfloat *params )
{
    _jsGcmFifoGlProgramParameterfv(( CGprogram )&shader->program, ( CGparameter )p, params, shader->loadAddressId );
}

/*
  The following section deals with generating all the hierarchical information from the leaf
  parameters. It deals with a number of things based on the parameter names.
  It includes
  * a custom UID table, to store/lookup strings, along with a global bit.
  * On ordering method to match the (weird) ordering requirements on parameters.
*/

typedef struct JSparamUIDRec
{
    char * string;
    GLboolean global;
}
JSparamUID;

typedef struct JSparamUIDTableRec
{
    int count;
    JSparamUID* uids;
}
JSparamUIDTable;


JSparamUID getParamUIDByIndex( const JSparamUIDTable* st, int index )
{
    JS_ASSERT( index >= 0 && index < st->count );
    return st->uids[index];
}

static void setAttribConstantIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int ) // index
{
    // XXX not sure why it was testing unused res
    _CGprogram *program = ptr->program;
    const CgParameterResource *parameterResource = _jsGetParameterResource( program, ptr->parameterEntry );
    GLuint index = parameterResource->resource - CG_ATTR0;
    JS_ASSERT( parameterResource->resource >= CG_ATTR0 && parameterResource->resource <= CG_ATTR15 );
    float * f = ( float* ) v;
    _jsVertexAttrib4fNV( index, f[0], f[1], f[2], f[3] );
}

void _jsPlatformSetVertexRegister4fv( unsigned int reg, const float * __restrict v )
{
    JS_ASSERT( reg < JS_MAX_VP_SHARED_CONSTANTS );

    // save to shared memory for context restore after flip
    __builtin_memcpy( _jsGetGcmDriver()->sharedVPConstants + reg*4*sizeof( float ), v, 4*sizeof( float ) );

    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, reg, 1, v ); 
}

void _jsPlatformSetVertexRegisterBlock( unsigned int reg, unsigned int count, const float * __restrict v )
{
    JS_ASSERT( count + reg <= JS_MAX_VP_SHARED_CONSTANTS );

    // save to shared memory for context restore after flip
    memcpy( _jsGetGcmDriver()->sharedVPConstants + reg*4*sizeof( float ), v, count*4*sizeof( float ) );

    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, reg, count, v ); 
}


// endian swapping of the fragment uniforms, if necessary
#if JS_ENDIAN == JS_BIG_ENDIAN
#define SWAP_IF_BIG_ENDIAN(arg) endianSwapWordByHalf(arg)
#elif JS_ENDIAN == JS_LITTLE_ENDIAN
#define SWAP_IF_BIG_ENDIAN(arg) arg
#else
#error include missing for endianness
#endif

void _jsPlatformSetFragmentRegister4fv( unsigned int reg, const float * __restrict v )
{
    JS_ASSERT( reg < JS_MAX_FP_SHARED_CONSTANTS );
    PSGLcontext * LContext = _CurrentContext;

    // XXX should we really get the NV driver here ?
    jsGcmDriver *driver = _jsGetGcmDriver();

    GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );
    GLuint dstVidOffset = gmmIdToOffset( driver->sharedFPConstantsId ) + reg * 16;
    void *pointer=NULL;
    GCM_FUNC( cellGcmSetInlineTransferPointer, dstVidOffset, 4, &pointer);
    float *fp = (float*)pointer;
    const float *src = (const float*)v;
    fp[0] = cellGcmSwap16Float32(src[0]);
    fp[1] = cellGcmSwap16Float32(src[1]);
    fp[2] = cellGcmSwap16Float32(src[2]);
    fp[3] = cellGcmSwap16Float32(src[3]);

    LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;

    // XXX we don't care about 32bit wrapping, do we ?
    ++LContext->LastFPConstantModification;
}

void _jsPlatformSetFragmentRegisterBlock( unsigned int reg, unsigned int count, const float * __restrict v )
{
    JS_ASSERT( reg < JS_MAX_FP_SHARED_CONSTANTS );
    JS_ASSERT( reg + count <= JS_MAX_FP_SHARED_CONSTANTS );
    PSGLcontext * LContext = _CurrentContext;
    // XXX should we really get the NV driver here ?
    jsGcmDriver *driver = _jsGetGcmDriver();

    GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );
    GLuint dstVidOffset = gmmIdToOffset( driver->sharedFPConstantsId ) + reg * 16; // *4; //(reg/4)*4 + (reg%4)*4;
    void *pointer=NULL;
    GCM_FUNC( cellGcmSetInlineTransferPointer, dstVidOffset, 4*count, &pointer);
    float *fp = (float*)pointer;
    float *src = (float*)v;
    for (uint32_t j=0; j<count;j++)
    {
	    fp[0] = cellGcmSwap16Float32(src[0]);
	    fp[1] = cellGcmSwap16Float32(src[1]);
	    fp[2] = cellGcmSwap16Float32(src[2]);
	    fp[3] = cellGcmSwap16Float32(src[3]);
	    fp+=4;src+=4;
    }

    LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;
    // XXX we don't care about 32bit wrapping, do we ?
    ++LContext->LastFPConstantModification;
}

void _jsPlatformSetBoolVertexRegisters( unsigned int values )
{
    GCM_FUNC( cellGcmSetTransformBranchBits, values );
}


//here ec has been advanced and is already on top of the embedded constant count
template<int SIZE> inline static void swapandsetfp( int ucodeSize, unsigned int loadProgramId, unsigned int loadProgramOffset, unsigned short *ec, const unsigned int   * __restrict v )
{
    //unsigned int v2[4];
    //for (long i=0; i < SIZE; ++i)
    //{
    //	v2[i] = SWAP_IF_BIG_ENDIAN(v[i]);
    //}
    GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );
    unsigned short count = *( ec++ );
    for ( unsigned long offsetIndex = 0; offsetIndex < count; ++offsetIndex )
    {
        JS_ASSERT(( *ec ) <= ( ucodeSize - 16 ) );
		void *pointer=NULL;
		const int paddedSIZE = (SIZE + 1) & ~1; // even width only	
		GCM_FUNC( cellGcmSetInlineTransferPointer, gmmIdToOffset( loadProgramId ) + loadProgramOffset + *( ec++ ), paddedSIZE, &pointer);
		float *fp = (float*)pointer;
		float *src = (float*)v;
		for (uint32_t j=0; j<SIZE;j++)
		{
			*fp = cellGcmSwap16Float32(*src);
			fp++;src++;
		}
    }

}

template<int SIZE> static void setVectorTypefp( CgRuntimeParameter* __restrict ptr, const void* __restrict v )
{
    float * __restrict  f = ( float* )v;
    float * __restrict  data = ( float* )ptr->pushBufferPointer;/*(float*)ptr->offset*;*/
    for ( long i = 0; i < SIZE; ++i ) //TODO: ced: find out if this loop for the get or for the reset in a future use of the same shader or just for the alignment???
        data[i] = f[i];
    _CGprogram *program = ptr->program;
    JS_ASSERT( program->loadProgramId != GMM_ERROR );

    CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    unsigned short *ec = ( unsigned short * )( ptr->program->resources ) + resource + 1;//+1 to skip the register
    if ( JS_LIKELY( *ec ) )
    {
        swapandsetfp<SIZE>( program->header.instructionCount*16, program->loadProgramId, program->loadProgramOffset, ec, ( unsigned int * )data );
    }
}

template<int SIZE> static void setVectorTypeSharedfpIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int /*index*/ )
{
    PSGLcontext * LContext = _CurrentContext;
    jsGcmDriver *driver = _jsGetGcmDriver();
    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    unsigned short sharedResource = *(( unsigned short * )( ptr->program->resources ) + resource );
    const unsigned int * __restrict vi = ( const unsigned int* )v;

    GLuint dstVidOffset = gmmIdToOffset( driver->sharedFPConstantsId ) + sharedResource * 16;
    unsigned int values[4];
    values[0] = SWAP_IF_BIG_ENDIAN( vi[0] );
    values[1] = ( 1 < SIZE ) ? SWAP_IF_BIG_ENDIAN( vi[1] ) : 0;
    values[2] = ( 2 < SIZE ) ? SWAP_IF_BIG_ENDIAN( vi[2] ) : 0;
    values[3] = ( 3 < SIZE ) ? SWAP_IF_BIG_ENDIAN( vi[3] ) : 0;
    GCM_FUNC( cellGcmInlineTransfer, dstVidOffset, values, 4, 0 );

    LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;
    // XXX we don't care about 32bit wrapping, do we ?
    ++LContext->LastFPConstantModification;
}

template<int SIZE> static void setVectorTypeSharedfpIndexArray( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
    PSGLcontext * LContext = _CurrentContext;
    jsGcmDriver *driver = _jsGetGcmDriver();
    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;

    //slow... skip the indices
    unsigned short *sharedResourcePtr = (( unsigned short * )( ptr->program->resources ) + resource );//no +1 here, we want the register
    int arrayIndex = index;
    while ( arrayIndex ) //jump to the right index... this is slow
    {
        sharedResourcePtr += (( *sharedResourcePtr ) + 2 );////+1 for the register, +1 for the count, +count for the number of embedded consts
        arrayIndex--;
    }
    unsigned short sharedResource = *sharedResourcePtr;

    const unsigned int * __restrict vi = ( const unsigned int* )v;

    GLuint dstVidOffset = gmmIdToOffset( driver->sharedFPConstantsId ) + sharedResource * 16;
    unsigned int values[4];
    values[0] = SWAP_IF_BIG_ENDIAN( vi[0] );
    values[1] = ( 1 < SIZE ) ? SWAP_IF_BIG_ENDIAN( vi[1] ) : 0;
    values[2] = ( 2 < SIZE ) ? SWAP_IF_BIG_ENDIAN( vi[2] ) : 0;
    values[3] = ( 3 < SIZE ) ? SWAP_IF_BIG_ENDIAN( vi[3] ) : 0;
    GCM_FUNC( cellGcmInlineTransfer, dstVidOffset, values, 4, 0 ); 

    LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;
    // XXX we don't care about 32bit wrapping, do we ?
    ++LContext->LastFPConstantModification;
}
template<int SIZE> static void setVectorTypeSharedvpIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int /*index*/ )
{
    const float * __restrict f = ( const float * __restrict )v;
    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    float * __restrict dst = ( float * __restrict )ptr->pushBufferPointer;
    for ( long i = 0; i < SIZE; ++ i )
        dst[i] = f[i];
    _jsPlatformSetVertexRegister4fv( resource, dst );
}

template<int SIZE> static void setVectorTypeSharedvpIndexArray( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
    const float * __restrict f = ( const float * __restrict )v;
    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource + index; ///TODO: assume contiguous here , right ?
    float * __restrict dst = ( float * __restrict )ptr->pushBufferPointer;
    for ( long i = 0; i < SIZE; ++ i )
        dst[i] = f[i];
    _jsPlatformSetVertexRegister4fv( resource, dst );
}


//  matrix uniforms
// note that Cg generated matrices are 1 row per binary param
// storage within the parameter is row major (so register setting is easier)

//tmp array tentative

#define ROW_MAJOR 0
#define COL_MAJOR 1

template <int SIZE> static void setVectorTypevpIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int /*index*/ )
{
    PSGLcontext * LContext = _CurrentContext;
    const float * __restrict f = ( const float* )v;
    float * __restrict dst = ( float* )ptr->pushBufferPointer;
    for ( long i = 0; i < SIZE; ++ i )
        dst[i] = f[i];
    LContext->needValidate |= PSGL_VALIDATE_VERTEX_CONSTANTS;
}
template <int SIZE> static void setVectorTypevpIndexArray( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
    PSGLcontext * LContext = _CurrentContext;
    const float * __restrict f = ( const float* )v;
    float *  __restrict dst = ( float* )( *(( unsigned int ** )ptr->pushBufferPointer + index ) );
    for ( long i = 0; i < SIZE; ++ i )
        dst[i] = f[i];
    LContext->needValidate |= PSGL_VALIDATE_VERTEX_CONSTANTS;
}
template<int SIZE> static void setVectorTypefpIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int /*index*/ )
{
    float * __restrict  f = ( float* )v;
    float * __restrict  data = ( float* )ptr->pushBufferPointer;/*(float*)ptr->offset*;*/
    for ( long i = 0; i < SIZE; ++i ) //TODO: ced: find out if this loop for the get or for the reset in a future use of the same shader or just for the alignment???
        data[i] = f[i];
    _CGprogram *program = ptr->program;
    JS_ASSERT( program->loadProgramId != GMM_ERROR );

    const CgParameterResource *parameterResource = _jsGetParameterResource( program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    unsigned short *ec = ( unsigned short * )( ptr->program->resources ) + resource + 1;
    if ( JS_LIKELY( *ec ) )
    {
        swapandsetfp<SIZE>( program->header.instructionCount*16, program->loadProgramId, program->loadProgramOffset, ec, ( unsigned int * )data );
    }
}
template<int SIZE> static void setVectorTypefpIndexArray( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
    float * __restrict  f = ( float* )v;
    float * __restrict  data = ( float* )ptr->pushBufferPointer;/*(float*)ptr->offset*;*/
    for ( long i = 0; i < SIZE; ++i ) //TODO: ced: find out if this loop for the get or for the reset in a future use of the same shader or just for the alignment???
        data[i] = f[i];
    _CGprogram *program = ptr->program;
    JS_ASSERT( program->loadProgramId != GMM_ERROR );

    const CgParameterResource *parameterResource = _jsGetParameterResource( program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    unsigned short *ec = ( unsigned short * )( program->resources ) + resource + 1;
    int arrayIndex = index;
    while ( arrayIndex ) //jump to the right index... this is slow
    {
        ec += (( *ec ) + 2 );//+1 for the register, +1 for the count, +count for the number of embedded consts
        arrayIndex--;
    }
    if ( JS_LIKELY( *ec ) )
    {
        swapandsetfp<SIZE>( program->header.instructionCount*16, program->loadProgramId, program->loadProgramOffset, ec, ( unsigned int * )data );
    }
}

//matrices
template <int ROWS, int COLS, int ORDER> static void setMatrixvpIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
    PSGLcontext * LContext = _CurrentContext;
    float *  __restrict f = ( float* )v;
    float *  __restrict dst = ( float* )ptr->pushBufferPointer;
    for ( long row = 0; row < ROWS; ++row )
    {
        for ( long col = 0; col < COLS; ++col )
            dst[row * 4 + col] = ( ORDER == ROW_MAJOR ) ? f[row * COLS + col] : f[col * ROWS + row];
    }
    LContext->needValidate |= PSGL_VALIDATE_VERTEX_CONSTANTS;
}

template <int ROWS, int COLS, int ORDER> static void setMatrixSharedvpIndex( CgRuntimeParameter*  __restrict ptr, const void*  __restrict v, const int /*index*/ )
{
    float * __restrict f = ( float* )v;
    float * __restrict dst = ( float* )ptr->pushBufferPointer;
    //jsFifoData *  __restrict jsGcmCurrent = _jsGcmState.fifo.current;

    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;

    float tmp[ROWS*4];
    for ( long row = 0; row < ROWS; ++row )
    {
        for ( long col = 0; col < COLS; ++col )
        {
            tmp[row*4 + col] = dst[row * 4 + col] = ( ORDER == ROW_MAJOR ) ? f[row * COLS + col] : f[col * ROWS + row];
        }
        for ( long col = COLS; col < 4; ++col ) tmp[row*4 + col] = dst[row*4+col];
    }

    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, resource, ROWS, ( const float* )tmp );
}

template <int ROWS, int COLS, int ORDER> static void setMatrixSharedvpIndexArray( CgRuntimeParameter*  __restrict ptr, const void*  __restrict v, const int index )
{
    float * __restrict f = ( float* )v;
    float * __restrict dst = ( float* )ptr->pushBufferPointer;

    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource + index * ROWS;

    float tmp[ROWS*4];
    for ( long row = 0; row < ROWS; ++row )
    {
        for ( long col = 0; col < COLS; ++col )
        {
            tmp[row*4 + col] = dst[row * 4 + col] = ( ORDER == ROW_MAJOR ) ? f[row * COLS + col] : f[col * ROWS + row];
        }
        for ( long col = COLS; col < 4; ++col ) tmp[row*4 + col] = dst[row*4+col];
    }
    GCM_FUNC( cellGcmSetVertexProgramParameterBlock, resource, ROWS, tmp );
}

template <int ROWS, int COLS, int ORDER> static void setMatrixSharedfpIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int /*index*/ )
{
    jsGcmDriver *driver = _jsGetGcmDriver();

    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    unsigned short sharedResource = *(( unsigned short * )( ptr->program->resources ) + resource );

    GLuint dstVidOffset = gmmIdToOffset( driver->sharedFPConstantsId ) + sharedResource * 16;
    //we assume that the assignment is contiguous
    const unsigned int * __restrict u = ( const unsigned int* )v;

    // cedric: the following function won't be added to GCM.
    //As we state in our introduction, this function doesn't conform to the current gcm which doesn't give to the
    //user the opportunity to make a mistake filling the push buffer. The user could corrupt the push buffer using that
    //function. So I am using the current gcm API below to implement the same feature, the cost is one additional copy for
    //each float value
    /*GCM_FUNC( cellGcmInlineTransferCommand, dstVidOffset, 4*ROWS, 0 );

    jsFifoData *  __restrict jsGcmCurrent = _jsGcmState.fifo.current;
    for (long row = 0; row < ROWS; ++row)
    {
    jsGcmCurrent[0].u = SWAP_IF_BIG_ENDIAN((ORDER==ROW_MAJOR)?u[row * COLS + 0]:u[0 * ROWS + row]);
    jsGcmCurrent[1].u = ((1<COLS)?SWAP_IF_BIG_ENDIAN((ORDER==ROW_MAJOR)?u[row * COLS + 1]:u[1 * ROWS + row]):0);
    jsGcmCurrent[2].u = ((2<COLS)?SWAP_IF_BIG_ENDIAN((ORDER==ROW_MAJOR)?u[row * COLS + 2]:u[2 * ROWS + row]):0);
    jsGcmCurrent[3].u = ((3<COLS)?SWAP_IF_BIG_ENDIAN((ORDER==ROW_MAJOR)?u[row * COLS + 3]:u[3 * ROWS + row]):0);
    jsGcmCurrent+=4;
    }
    JSGCM_GET_SKID_SPACE_IF_NEEDED();
    */
    unsigned int tmp[ROWS*4];
    for ( long row = 0; row < ROWS; ++row )
    {
        tmp[row*4 + 0] = (( ORDER == ROW_MAJOR ) ? u[row * COLS + 0] : u[0 * ROWS + row] );
        tmp[row*4 + 1] = (( 1 < COLS ) ? (( ORDER == ROW_MAJOR ) ? u[row * COLS + 1] : u[1 * ROWS + row] ) : 0 );
        tmp[row*4 + 2] = (( 2 < COLS ) ? (( ORDER == ROW_MAJOR ) ? u[row * COLS + 2] : u[2 * ROWS + row] ) : 0 );
        tmp[row*4 + 3] = (( 3 < COLS ) ? (( ORDER == ROW_MAJOR ) ? u[row * COLS + 3] : u[3 * ROWS + row] ) : 0 );
    }
    GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );
    void *pointer=NULL;
	GCM_FUNC( cellGcmSetInlineTransferPointer, dstVidOffset, 4*ROWS, &pointer);
	float *fp = (float*)pointer;
	float *src = (float*)tmp;
	for (uint32_t j=0; j<ROWS;j++)
	{
		fp[0] = cellGcmSwap16Float32(src[0]);
		fp[1] = cellGcmSwap16Float32(src[1]);
		fp[2] = cellGcmSwap16Float32(src[2]);
		fp[3] = cellGcmSwap16Float32(src[3]);
		fp+=4;src+=4;
	}

  PSGLcontext * LContext = _CurrentContext;
  LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;
  ++LContext->LastFPConstantModification;
}

template <int ROWS, int COLS, int ORDER> static void setMatrixSharedfpIndexArray( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
    //TODO: double check for the semi endian swap... not done here, is it done by the RSX ?
    jsGcmDriver *driver = _jsGetGcmDriver();

    const CgParameterResource *parameterResource = _jsGetParameterResource( ptr->program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    //slow... skip the indices
    unsigned short *sharedResourcePtr = (( unsigned short * )( ptr->program->resources ) + resource );
    int arrayIndex = index * ROWS;
    while ( arrayIndex ) //jump to the right index... this is slow
    {
        sharedResourcePtr += (( *sharedResourcePtr ) + 2 );//+1 for the register, +1 for the count, +count for the number of embedded consts
        arrayIndex--;
    }
    unsigned short sharedResource = *sharedResourcePtr;

    GLuint dstVidOffset = gmmIdToOffset( driver->sharedFPConstantsId ) + sharedResource * 16;
    //we assume that the assignment is contiguous
    const unsigned int * __restrict u = ( const unsigned int* )v;

    unsigned int tmp[ROWS*4];
    for ( long row = 0; row < ROWS; ++row )
    {
        tmp[row*4 + 0] = (( ORDER == ROW_MAJOR ) ? u[row * COLS + 0] : u[0 * ROWS + row] );
        tmp[row*4 + 1] = (( 1 < COLS ) ? (( ORDER == ROW_MAJOR ) ? u[row * COLS + 1] : u[1 * ROWS + row] ) : 0 );
        tmp[row*4 + 2] = (( 2 < COLS ) ? (( ORDER == ROW_MAJOR ) ? u[row * COLS + 2] : u[2 * ROWS + row] ) : 0 );
        tmp[row*4 + 3] = (( 3 < COLS ) ? (( ORDER == ROW_MAJOR ) ? u[row * COLS + 3] : u[3 * ROWS + row] ) : 0 );
    }

    GCM_FUNC( cellGcmSetTransferLocation, CELL_GCM_LOCATION_LOCAL );


	  void *pointer=NULL;
	  GCM_FUNC( cellGcmSetInlineTransferPointer, dstVidOffset, 4*ROWS, &pointer);
	  float *fp = (float*)pointer;
	  const float *src = (const float*)tmp;
	  for (uint32_t j=0; j<4*ROWS;j++)
	  {
		  *fp = cellGcmSwap16Float32(*src);
		  fp++;src++;
	  }

    PSGLcontext * LContext = _CurrentContext;
    LContext->needValidate |= PSGL_VALIDATE_FRAGMENT_SHARED_CONSTANTS;
    ++LContext->LastFPConstantModification;
}

//TODO ?: check: //works only for the consecutive alloc...
template <int ROWS, int COLS, int ORDER> static void setMatrixvpIndexArray( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
#ifdef JS_DEBUG
    _CGprogram *program = ptr->program;
    const CgParameterArray *parameterArray = _jsGetParameterArray( program, (( CgRuntimeParameter* )ptr )->parameterEntry - 1 );//the array is at the ptr address -1
    int size = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
    JS_ASSERT( index >= 0 && index <  size && "array index out of bound" );
#endif
    PSGLcontext * LContext = _CurrentContext;
    float *  __restrict f = ( float* )v;
    float *  __restrict dst = ( float* )( *(( unsigned int ** )ptr->pushBufferPointer + index ) );
    for ( long row = 0; row < ROWS; ++row )
    {
        for ( long col = 0; col < COLS; ++col )
            dst[row * 4 + col] = ( ORDER == ROW_MAJOR ) ? f[row * COLS + col] : f[col * ROWS + row];
    }
    LContext->needValidate |= PSGL_VALIDATE_VERTEX_CONSTANTS;
}
template <int ROWS, int COLS, int ORDER> static void setMatrixfpIndex( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int /*index*/ )
{
    float *  __restrict f = ( float* )v;
    float *  __restrict dst = ( float* )ptr->pushBufferPointer;
    _CGprogram *program = (( CgRuntimeParameter* )ptr )->program;
    const CgParameterResource *parameterResource = _jsGetParameterResource( program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    unsigned short *ec = ( unsigned short * )program->resources + resource + 1; //+1 to skip the register
    for ( long row = 0; row < ROWS; ++row )
    {
        for ( long col = 0; col < COLS; ++col )
            dst[row * 4 + col] = ( ORDER == ROW_MAJOR ) ? f[row * COLS + col] : f[col * ROWS + row];
        int count = *ec;
        if ( JS_LIKELY( count ) )
        {
            swapandsetfp<COLS>( program->header.instructionCount*16, program->loadProgramId, program->loadProgramOffset, ec, ( unsigned int * )dst + row * 4 );
        }
        ec += count + 2; //+1 for the register, +1 for the count, + count for the number of embedded consts
    }
}
template <int ROWS, int COLS, int ORDER> static void setMatrixfpIndexArray( CgRuntimeParameter* __restrict ptr, const void* __restrict v, const int index )
{
    float *  __restrict f = ( float* )v;
    float *  __restrict dst = ( float* )ptr->pushBufferPointer;
    _CGprogram *program = ptr->program;
    const CgParameterResource *parameterResource = _jsGetParameterResource( program, ptr->parameterEntry );
    unsigned short resource = parameterResource->resource;
    unsigned short *ec = ( unsigned short * )program->resources + resource + 1;//+1 to skip the register
    int arrayIndex = index * ROWS;
    while ( arrayIndex ) //jump to the right index... this is slow
    {
        unsigned short count = ( *ec );
        ec += ( count + 2 ); //+1 for the register, +1 for the count, +count for the number of embedded consts
        arrayIndex--;
    }
    for ( long row = 0; row < ROWS; ++row )
    {
        for ( long col = 0; col < COLS; ++col )
            dst[row * 4 + col] = ( ORDER == ROW_MAJOR ) ? f[row * COLS + col] : f[col * ROWS + row];
        int count = *ec;
        if ( JS_LIKELY( count ) )
        {
            swapandsetfp<COLS>( program->header.instructionCount*16, program->loadProgramId, program->loadProgramOffset, ec, ( unsigned int * )dst + row * 4 );
        }
        ec += count + 2;//+1 for the register, +1 for the count, +count for the number of embedded consts
    }
}

static _cgSetArrayIndexFunction setVectorTypeIndex[2][2][2][4] =
    {
        {
            {
                {&setVectorTypevpIndex<1>, &setVectorTypevpIndex<2>, &setVectorTypevpIndex<3>, &setVectorTypevpIndex<4>, },
                {&setVectorTypefpIndex<1>, &setVectorTypefpIndex<2>, &setVectorTypefpIndex<3>, &setVectorTypefpIndex<4>, }
            },
            {
                {&setVectorTypeSharedvpIndex<1>, &setVectorTypeSharedvpIndex<2>, &setVectorTypeSharedvpIndex<3>, &setVectorTypeSharedvpIndex<4>, }, //should be the shared
                {&setVectorTypeSharedfpIndex<1>, &setVectorTypeSharedfpIndex<2>, &setVectorTypeSharedfpIndex<3>, &setVectorTypeSharedfpIndex<4>, } //should be the shared
            },
        },
        {
            {
                {&setVectorTypevpIndexArray<1>, &setVectorTypevpIndexArray<2>, &setVectorTypevpIndexArray<3>, &setVectorTypevpIndexArray<4>, },
                {&setVectorTypefpIndexArray<1>, &setVectorTypefpIndexArray<2>, &setVectorTypefpIndexArray<3>, &setVectorTypefpIndexArray<4>, }
            },
            {
                {&setVectorTypeSharedvpIndexArray<1>, &setVectorTypeSharedvpIndexArray<2>, &setVectorTypeSharedvpIndexArray<3>, &setVectorTypeSharedvpIndexArray<4>, }, //should be the shared
                {&setVectorTypeSharedfpIndexArray<1>, &setVectorTypeSharedfpIndexArray<2>, &setVectorTypeSharedfpIndexArray<3>, &setVectorTypeSharedfpIndexArray<4>, } //should be the shared
            },
        },
    };

static _cgSetArrayIndexFunction setMatrixTypeIndex[2][2][2][4][4][2] =
    {
        {
            {
                {
                    {{ &setMatrixvpIndex<1, 1, 0>, &setMatrixvpIndex<1, 1, 1>}, { &setMatrixvpIndex<1, 2, 0>, &setMatrixvpIndex<1, 2, 1>}, { &setMatrixvpIndex<1, 3, 0>, &setMatrixvpIndex<1, 3, 1>}, { &setMatrixvpIndex<1, 4, 0>, &setMatrixvpIndex<1, 4, 1>}},
                    {{ &setMatrixvpIndex<2, 1, 0>, &setMatrixvpIndex<2, 1, 1>}, { &setMatrixvpIndex<2, 2, 0>, &setMatrixvpIndex<2, 2, 1>}, { &setMatrixvpIndex<2, 3, 0>, &setMatrixvpIndex<2, 3, 1>}, { &setMatrixvpIndex<2, 4, 0>, &setMatrixvpIndex<2, 4, 1>}},
                    {{ &setMatrixvpIndex<3, 1, 0>, &setMatrixvpIndex<3, 1, 1>}, { &setMatrixvpIndex<3, 2, 0>, &setMatrixvpIndex<3, 2, 1>}, { &setMatrixvpIndex<3, 3, 0>, &setMatrixvpIndex<3, 3, 1>}, { &setMatrixvpIndex<3, 4, 0>, &setMatrixvpIndex<3, 4, 1>}},
                    {{ &setMatrixvpIndex<4, 1, 0>, &setMatrixvpIndex<4, 1, 1>}, { &setMatrixvpIndex<4, 2, 0>, &setMatrixvpIndex<4, 2, 1>}, { &setMatrixvpIndex<4, 3, 0>, &setMatrixvpIndex<4, 3, 1>}, { &setMatrixvpIndex<4, 4, 0>, &setMatrixvpIndex<4, 4, 1>}},
                },
                {
                    {{ &setMatrixfpIndex<1, 1, 0>, &setMatrixfpIndex<1, 1, 1>}, { &setMatrixfpIndex<1, 2, 0>, &setMatrixfpIndex<1, 2, 1>}, { &setMatrixfpIndex<1, 3, 0>, &setMatrixfpIndex<1, 3, 1>}, { &setMatrixfpIndex<1, 4, 0>, &setMatrixfpIndex<1, 4, 1>}},
                    {{ &setMatrixfpIndex<2, 1, 0>, &setMatrixfpIndex<2, 1, 1>}, { &setMatrixfpIndex<2, 2, 0>, &setMatrixfpIndex<2, 2, 1>}, { &setMatrixfpIndex<2, 3, 0>, &setMatrixfpIndex<2, 3, 1>}, { &setMatrixfpIndex<2, 4, 0>, &setMatrixfpIndex<2, 4, 1>}},
                    {{ &setMatrixfpIndex<3, 1, 0>, &setMatrixfpIndex<3, 1, 1>}, { &setMatrixfpIndex<3, 2, 0>, &setMatrixfpIndex<3, 2, 1>}, { &setMatrixfpIndex<3, 3, 0>, &setMatrixfpIndex<3, 3, 1>}, { &setMatrixfpIndex<3, 4, 0>, &setMatrixfpIndex<3, 4, 1>}},
                    {{ &setMatrixfpIndex<4, 1, 0>, &setMatrixfpIndex<4, 1, 1>}, { &setMatrixfpIndex<4, 2, 0>, &setMatrixfpIndex<4, 2, 1>}, { &setMatrixfpIndex<4, 3, 0>, &setMatrixfpIndex<4, 3, 1>}, { &setMatrixfpIndex<4, 4, 0>, &setMatrixfpIndex<4, 4, 1>}},
                },
            },
            { //should be shared
                {
                    {{ &setMatrixSharedvpIndex<1, 1, 0>, &setMatrixSharedvpIndex<1, 1, 1>}, { &setMatrixSharedvpIndex<1, 2, 0>, &setMatrixSharedvpIndex<1, 2, 1>}, { &setMatrixSharedvpIndex<1, 3, 0>, &setMatrixSharedvpIndex<1, 3, 1>}, { &setMatrixSharedvpIndex<1, 4, 0>, &setMatrixSharedvpIndex<1, 4, 1>}},
                    {{ &setMatrixSharedvpIndex<2, 1, 0>, &setMatrixSharedvpIndex<2, 1, 1>}, { &setMatrixSharedvpIndex<2, 2, 0>, &setMatrixSharedvpIndex<2, 2, 1>}, { &setMatrixSharedvpIndex<2, 3, 0>, &setMatrixSharedvpIndex<2, 3, 1>}, { &setMatrixSharedvpIndex<2, 4, 0>, &setMatrixSharedvpIndex<2, 4, 1>}},
                    {{ &setMatrixSharedvpIndex<3, 1, 0>, &setMatrixSharedvpIndex<3, 1, 1>}, { &setMatrixSharedvpIndex<3, 2, 0>, &setMatrixSharedvpIndex<3, 2, 1>}, { &setMatrixSharedvpIndex<3, 3, 0>, &setMatrixSharedvpIndex<3, 3, 1>}, { &setMatrixSharedvpIndex<3, 4, 0>, &setMatrixSharedvpIndex<3, 4, 1>}},
                    {{ &setMatrixSharedvpIndex<4, 1, 0>, &setMatrixSharedvpIndex<4, 1, 1>}, { &setMatrixSharedvpIndex<4, 2, 0>, &setMatrixSharedvpIndex<4, 2, 1>}, { &setMatrixSharedvpIndex<4, 3, 0>, &setMatrixSharedvpIndex<4, 3, 1>}, { &setMatrixSharedvpIndex<4, 4, 0>, &setMatrixSharedvpIndex<4, 4, 1>}},
                },
                {
                    {{ &setMatrixSharedfpIndex<1, 1, 0>, &setMatrixSharedfpIndex<1, 1, 1>}, { &setMatrixSharedfpIndex<1, 2, 0>, &setMatrixSharedfpIndex<1, 2, 1>}, { &setMatrixSharedfpIndex<1, 3, 0>, &setMatrixSharedfpIndex<1, 3, 1>}, { &setMatrixSharedfpIndex<1, 4, 0>, &setMatrixSharedfpIndex<1, 4, 1>}},
                    {{ &setMatrixSharedfpIndex<2, 1, 0>, &setMatrixSharedfpIndex<2, 1, 1>}, { &setMatrixSharedfpIndex<2, 2, 0>, &setMatrixSharedfpIndex<2, 2, 1>}, { &setMatrixSharedfpIndex<2, 3, 0>, &setMatrixSharedfpIndex<2, 3, 1>}, { &setMatrixSharedfpIndex<2, 4, 0>, &setMatrixSharedfpIndex<2, 4, 1>}},
                    {{ &setMatrixSharedfpIndex<3, 1, 0>, &setMatrixSharedfpIndex<3, 1, 1>}, { &setMatrixSharedfpIndex<3, 2, 0>, &setMatrixSharedfpIndex<3, 2, 1>}, { &setMatrixSharedfpIndex<3, 3, 0>, &setMatrixSharedfpIndex<3, 3, 1>}, { &setMatrixSharedfpIndex<3, 4, 0>, &setMatrixSharedfpIndex<3, 4, 1>}},
                    {{ &setMatrixSharedfpIndex<4, 1, 0>, &setMatrixSharedfpIndex<4, 1, 1>}, { &setMatrixSharedfpIndex<4, 2, 0>, &setMatrixSharedfpIndex<4, 2, 1>}, { &setMatrixSharedfpIndex<4, 3, 0>, &setMatrixSharedfpIndex<4, 3, 1>}, { &setMatrixSharedfpIndex<4, 4, 0>, &setMatrixSharedfpIndex<4, 4, 1>}},
                },
            },
        },
        {
            {
                {
                    {{ &setMatrixvpIndexArray<1, 1, 0>, &setMatrixvpIndexArray<1, 1, 1>}, { &setMatrixvpIndexArray<1, 2, 0>, &setMatrixvpIndexArray<1, 2, 1>}, { &setMatrixvpIndexArray<1, 3, 0>, &setMatrixvpIndexArray<1, 3, 1>}, { &setMatrixvpIndexArray<1, 4, 0>, &setMatrixvpIndexArray<1, 4, 1>}},
                    {{ &setMatrixvpIndexArray<2, 1, 0>, &setMatrixvpIndexArray<2, 1, 1>}, { &setMatrixvpIndexArray<2, 2, 0>, &setMatrixvpIndexArray<2, 2, 1>}, { &setMatrixvpIndexArray<2, 3, 0>, &setMatrixvpIndexArray<2, 3, 1>}, { &setMatrixvpIndexArray<2, 4, 0>, &setMatrixvpIndexArray<2, 4, 1>}},
                    {{ &setMatrixvpIndexArray<3, 1, 0>, &setMatrixvpIndexArray<3, 1, 1>}, { &setMatrixvpIndexArray<3, 2, 0>, &setMatrixvpIndexArray<3, 2, 1>}, { &setMatrixvpIndexArray<3, 3, 0>, &setMatrixvpIndexArray<3, 3, 1>}, { &setMatrixvpIndexArray<3, 4, 0>, &setMatrixvpIndexArray<3, 4, 1>}},
                    {{ &setMatrixvpIndexArray<4, 1, 0>, &setMatrixvpIndexArray<4, 1, 1>}, { &setMatrixvpIndexArray<4, 2, 0>, &setMatrixvpIndexArray<4, 2, 1>}, { &setMatrixvpIndexArray<4, 3, 0>, &setMatrixvpIndexArray<4, 3, 1>}, { &setMatrixvpIndexArray<4, 4, 0>, &setMatrixvpIndexArray<4, 4, 1>}},
                },
                {
                    {{ &setMatrixfpIndexArray<1, 1, 0>, &setMatrixfpIndexArray<1, 1, 1>}, { &setMatrixfpIndexArray<1, 2, 0>, &setMatrixfpIndexArray<1, 2, 1>}, { &setMatrixfpIndexArray<1, 3, 0>, &setMatrixfpIndexArray<1, 3, 1>}, { &setMatrixfpIndexArray<1, 4, 0>, &setMatrixfpIndexArray<1, 4, 1>}},
                    {{ &setMatrixfpIndexArray<2, 1, 0>, &setMatrixfpIndexArray<2, 1, 1>}, { &setMatrixfpIndexArray<2, 2, 0>, &setMatrixfpIndexArray<2, 2, 1>}, { &setMatrixfpIndexArray<2, 3, 0>, &setMatrixfpIndexArray<2, 3, 1>}, { &setMatrixfpIndexArray<2, 4, 0>, &setMatrixfpIndexArray<2, 4, 1>}},
                    {{ &setMatrixfpIndexArray<3, 1, 0>, &setMatrixfpIndexArray<3, 1, 1>}, { &setMatrixfpIndexArray<3, 2, 0>, &setMatrixfpIndexArray<3, 2, 1>}, { &setMatrixfpIndexArray<3, 3, 0>, &setMatrixfpIndexArray<3, 3, 1>}, { &setMatrixfpIndexArray<3, 4, 0>, &setMatrixfpIndexArray<3, 4, 1>}},
                    {{ &setMatrixfpIndexArray<4, 1, 0>, &setMatrixfpIndexArray<4, 1, 1>}, { &setMatrixfpIndexArray<4, 2, 0>, &setMatrixfpIndexArray<4, 2, 1>}, { &setMatrixfpIndexArray<4, 3, 0>, &setMatrixfpIndexArray<4, 3, 1>}, { &setMatrixfpIndexArray<4, 4, 0>, &setMatrixfpIndexArray<4, 4, 1>}},
                },
            },
            { //should be shared
                {
                    {{ &setMatrixSharedvpIndexArray<1, 1, 0>, &setMatrixSharedvpIndexArray<1, 1, 1>}, { &setMatrixSharedvpIndexArray<1, 2, 0>, &setMatrixSharedvpIndexArray<1, 2, 1>}, { &setMatrixSharedvpIndexArray<1, 3, 0>, &setMatrixSharedvpIndexArray<1, 3, 1>}, { &setMatrixSharedvpIndexArray<1, 4, 0>, &setMatrixSharedvpIndexArray<1, 4, 1>}},
                    {{ &setMatrixSharedvpIndexArray<2, 1, 0>, &setMatrixSharedvpIndexArray<2, 1, 1>}, { &setMatrixSharedvpIndexArray<2, 2, 0>, &setMatrixSharedvpIndexArray<2, 2, 1>}, { &setMatrixSharedvpIndexArray<2, 3, 0>, &setMatrixSharedvpIndexArray<2, 3, 1>}, { &setMatrixSharedvpIndexArray<2, 4, 0>, &setMatrixSharedvpIndexArray<2, 4, 1>}},
                    {{ &setMatrixSharedvpIndexArray<3, 1, 0>, &setMatrixSharedvpIndexArray<3, 1, 1>}, { &setMatrixSharedvpIndexArray<3, 2, 0>, &setMatrixSharedvpIndexArray<3, 2, 1>}, { &setMatrixSharedvpIndexArray<3, 3, 0>, &setMatrixSharedvpIndexArray<3, 3, 1>}, { &setMatrixSharedvpIndexArray<3, 4, 0>, &setMatrixSharedvpIndexArray<3, 4, 1>}},
                    {{ &setMatrixSharedvpIndexArray<4, 1, 0>, &setMatrixSharedvpIndexArray<4, 1, 1>}, { &setMatrixSharedvpIndexArray<4, 2, 0>, &setMatrixSharedvpIndexArray<4, 2, 1>}, { &setMatrixSharedvpIndexArray<4, 3, 0>, &setMatrixSharedvpIndexArray<4, 3, 1>}, { &setMatrixSharedvpIndexArray<4, 4, 0>, &setMatrixSharedvpIndexArray<4, 4, 1>}},
                },
                {
                    {{ &setMatrixSharedfpIndexArray<1, 1, 0>, &setMatrixSharedfpIndexArray<1, 1, 1>}, { &setMatrixSharedfpIndexArray<1, 2, 0>, &setMatrixSharedfpIndexArray<1, 2, 1>}, { &setMatrixSharedfpIndexArray<1, 3, 0>, &setMatrixSharedfpIndexArray<1, 3, 1>}, { &setMatrixSharedfpIndexArray<1, 4, 0>, &setMatrixSharedfpIndexArray<1, 4, 1>}},
                    {{ &setMatrixSharedfpIndexArray<2, 1, 0>, &setMatrixSharedfpIndexArray<2, 1, 1>}, { &setMatrixSharedfpIndexArray<2, 2, 0>, &setMatrixSharedfpIndexArray<2, 2, 1>}, { &setMatrixSharedfpIndexArray<2, 3, 0>, &setMatrixSharedfpIndexArray<2, 3, 1>}, { &setMatrixSharedfpIndexArray<2, 4, 0>, &setMatrixSharedfpIndexArray<2, 4, 1>}},
                    {{ &setMatrixSharedfpIndexArray<3, 1, 0>, &setMatrixSharedfpIndexArray<3, 1, 1>}, { &setMatrixSharedfpIndexArray<3, 2, 0>, &setMatrixSharedfpIndexArray<3, 2, 1>}, { &setMatrixSharedfpIndexArray<3, 3, 0>, &setMatrixSharedfpIndexArray<3, 3, 1>}, { &setMatrixSharedfpIndexArray<3, 4, 0>, &setMatrixSharedfpIndexArray<3, 4, 1>}},
                    {{ &setMatrixSharedfpIndexArray<4, 1, 0>, &setMatrixSharedfpIndexArray<4, 1, 1>}, { &setMatrixSharedfpIndexArray<4, 2, 0>, &setMatrixSharedfpIndexArray<4, 2, 1>}, { &setMatrixSharedfpIndexArray<4, 3, 0>, &setMatrixSharedfpIndexArray<4, 3, 1>}, { &setMatrixSharedfpIndexArray<4, 4, 0>, &setMatrixSharedfpIndexArray<4, 4, 1>}},
                },
            },
        }
    };

_cgSetArrayIndexFunction getVectorTypeIndexSetterFunction( unsigned short a, unsigned short b, unsigned short c, unsigned short d )
{
    return setVectorTypeIndex[a][b][c][d];
}

_cgSetArrayIndexFunction getMatrixTypeIndexSetterFunction( unsigned short a, unsigned short b, unsigned short c, unsigned short d, unsigned short e, unsigned short f )
{
    return setMatrixTypeIndex[a][b][c][d][e][f];
}

static void setSamplerfp( CgRuntimeParameter*ptr, const void*v, int ) //index
{
    _CGprogram *program = (( CgRuntimeParameter* )ptr )->program;
    const CgParameterResource *parameterResource = _jsGetParameterResource( program, (( CgRuntimeParameter* )ptr )->parameterEntry );

    JS_ASSERT( parameterResource->type == CG_SAMPLER2D || parameterResource->type == CG_SAMPLER3D || parameterResource->type == CG_SAMPLERCUBE );
    JS_ASSERT( ptr->program->header.profile == CG_PROFILE_SCE_FP_TYPEB
               //|| ptr->program->header.profile==CG_PROFILE_SCE_FP_TYPEC
               || ptr->program->header.profile == CG_PROFILE_SCE_FP_RSX );
    JS_ASSERT( parameterResource->resource - CG_TEXUNIT0 < _JS_MAX_TEXTURE_IMAGE_UNITS );
    JS_ASSERT(( ptr->parameterEntry->flags & CGPF_REFERENCED ) && ptr->pushBufferPointer );

    // the value of v == NULL when it is called from cgGLEnableTextureParameter
    // the value of v == NULL when it is called from  cgGLSetTextureParameter
    // this may be called by a connected param to propagate its value
    // the spec says that the set should not cause the bind
    // so only do the bind when the call comes from cgGLEnableTextureParameter
    if ( v )
    {
        *( GLuint* )ptr->pushBufferPointer = *( GLuint* )v;
    }
    else
    {
        jsTextureImageUnit *unit = _CurrentContext->TextureImageUnits + ( parameterResource->resource - CG_TEXUNIT0 );
        _jsBindTextureInternal( unit, *( GLuint* )ptr->pushBufferPointer, ptr->glType );
    }
}

static void setSamplervp( CgRuntimeParameter*ptr, const void*v, int ) //index
{
    _CGprogram *program = (( CgRuntimeParameter* )ptr )->program;
    const CgParameterResource *parameterResource = _jsGetParameterResource( program, (( CgRuntimeParameter* )ptr )->parameterEntry );

    JS_ASSERT( parameterResource->type == CG_SAMPLER2D );
    JS_ASSERT( ptr->program->header.profile == CG_PROFILE_SCE_VP_TYPEB
               //|| ptr->program->header.profile==CG_PROFILE_SCE_VP_TYPEC
               || ptr->program->header.profile == CG_PROFILE_SCE_VP_RSX );
    JS_ASSERT( parameterResource->resource - CG_TEXUNIT0 < _JS_MAX_TEXTURE_IMAGE_UNITS );
    JS_ASSERT(( ptr->parameterEntry->flags & CGPF_REFERENCED ) && ptr->pushBufferPointer );

    // the value of v == NULL when it is called from cgGLEnableTextureParameter
    // the value of v == NULL when it is called from  cgGLSetTextureParameter
    // this may be called by a connected param to propagate its value
    // the spec says that the set should not cause the bind
    // so only do the bind when the call comes from cgGLEnableTextureParameter
    if ( v )
    {
        *( GLuint* )ptr->pushBufferPointer = *( GLuint* )v;
    }
    else
    {
        _jsBindVertexTextureInternal( parameterResource->resource - CG_TEXUNIT0, *( GLuint* )ptr->pushBufferPointer );
    }
}


#undef ROW_MAJOR
#undef COL_MAJOR

//new binary addition


int _jsGcmGenerateProgram( _CGprogram *program, int profileIndex, const CgProgramHeader *programHeader, const void *ucode, const CgParameterTableHeader *parameterHeader,
                           const CgParameterEntry *parameterEntries, const char *stringTable, const float *defaultValues )
{
    // validate the input
    CGprofile profile = ( CGprofile )programHeader->profile;

    int need_swapping = 0;

    //hack to counter removal of TypeC during beta
    if ( profile == ( CGprofile )7005 )
        profile = CG_PROFILE_SCE_VP_RSX;
    if ( profile == ( CGprofile )7006 )
        profile = CG_PROFILE_SCE_FP_RSX;

    // if can't match a known profile, the data may be in wrong endianness
    if (( profile != CG_PROFILE_SCE_FP_TYPEB ) && ( profile != CG_PROFILE_SCE_VP_TYPEB ) &&
            //(profile != CG_PROFILE_SCE_FP_TYPEC) && (profile != CG_PROFILE_SCE_VP_TYPEC) &&
            ( profile != CG_PROFILE_SCE_FP_RSX ) && ( profile != CG_PROFILE_SCE_VP_RSX ) )
    {
        need_swapping = 1;
    }

    // check that this program block is of the right revision
    // i.e. that the cgBinary.h header hasn't changed since it was
    // compiled.
    //TODO: find a way to do the same check
    //if(CG_BINARY_FORMAT_REVISION !=  ENDIAN_32(binary_program->binaryFormatRevision, need_swapping))
    //{
    // it either is a bad binary or an elf file.
    //   _jsCgRaiseError(CG_BINARY_FORMAT_REVISION_MISMATCH_ERROR);
    //   return 0;
    //}

    // validate the profile
    int invalidProfile = 0;
    switch ( ENDIAN_32( profile, need_swapping ) )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            if ( profileIndex != VERTEX_PROFILE_INDEX ) invalidProfile = 1;
            break;
        case CG_PROFILE_SCE_FP_TYPEB:
            if ( profileIndex != FRAGMENT_PROFILE_INDEX ) invalidProfile = 1;
            break;
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            if ( profileIndex != VERTEX_PROFILE_INDEX ) invalidProfile = 1;
            break;
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            if ( profileIndex != FRAGMENT_PROFILE_INDEX ) invalidProfile = 1;
            break;
        default:
            invalidProfile = 1;
            break;
    }
    if ( invalidProfile )
    {
        _jsCgRaiseError( CG_UNKNOWN_PROFILE_ERROR );
        return 0;
    }

    //TODO: reorganize, if we have an continuous buffer as input for the binary program, there's no need to split it again here
    //header
    memcpy( &program->header, programHeader, sizeof( program->header ) );

    //ucode
    //program->ucode = jsMemalign(16,ucodeSize);
    //if (program->ucode == NULL)
    //{
    //   _jsCgRaiseError(CG_MEMORY_ALLOC_ERROR);
    //   return 0;
    //}

#ifdef JS_DEBUG
    size_t ucodeSize = programHeader->instructionCount * 16;
    _ucodeAlloc += ucodeSize;
#endif

    program->ucode = ucode;
    program->loadProgramId = GMM_ERROR;
    program->loadProgramOffset = 0;
	program->inLocalMemory = true; 


    //data file size
    //size_t fileDataSize = parameterHeader->semanticIndexTableOffset + parameterHeader->semanticIndexCount * sizeof(CgParameterSemantic) - sizeof(CgParameterTableHeader);
    //runtime structures
    //size_t parameterSize = fileDataSize + parameterHeader->entryCount*sizeof(CgRuntimeParameter);
    size_t parameterSize = parameterHeader->entryCount * sizeof( CgRuntimeParameter );
    void *memoryBlock;
    if ( parameterSize )
        memoryBlock = jsMemalign( 16, parameterSize );
    else
        memoryBlock = NULL;

#ifdef JS_DEBUG
    _parameterAlloc += parameterSize;
#endif

    program->rtParametersCount = parameterHeader->entryCount;
    program->runtimeParameters = ( CgRuntimeParameter* )memoryBlock;

    if ( parameterEntries == NULL ) // the param entry can be supplied if not right after parameterHeader in memory, it happens when there's a program copy
        parameterEntries = ( CgParameterEntry* )( parameterHeader + 1 );

    //program->parametersEntries = (CgParameterEntry*)(program->runtimeParameters + program->rtParametersCount);
    program->parametersEntries = parameterEntries;
    program->parameterResources = ( char* )( program->parametersEntries + program->rtParametersCount );
    program->resources = ( unsigned short* )(( char* )program->parametersEntries + ( parameterHeader->resourceTableOffset - sizeof( CgParameterTableHeader ) ) );
    program->defaultValuesIndexCount = parameterHeader->defaultValueIndexCount;
    program->defaultValuesIndices = ( CgParameterDefaultValue* )(( char* )program->parametersEntries + ( parameterHeader->defaultValueIndexTableOffset - sizeof( CgParameterTableHeader ) ) );
    program->semanticCount = parameterHeader->semanticIndexCount;
    program->semanticIndices = ( CgParameterSemantic* )( program->defaultValuesIndices + program->defaultValuesIndexCount );

    //TODO: get the pointer on the constant table
    program->defaultValues = NULL;

    //JS_ASSERT(parameterSize == fileDataSize + runtimeParameterSize);
    memset( program->runtimeParameters, 0, parameterHeader->entryCount*sizeof( CgRuntimeParameter ) );

    //memcpy(program->parametersEntries,parameterEntries,fileDataSize);

    //string table
    program->stringTable = stringTable;
    //default values
    program->defaultValues = defaultValues;

    // get the pointer to our image copy
    //CgBinaryProgram* ptr = &result->program;

    // copy the program, parameters and current values into the specified context
    // NOTE: parameters and current values are persistant
    //memcpy(ptr, binary_program, size);
    //_jsGcmInitializeBinaryCgProgram((CGprogram)ptr);

    //need to init the embedded constant...
    //TODO: move that in the tool
    _jsCreatePushBuffer( program );
    if ( profileIndex == FRAGMENT_PROFILE_INDEX )
        _jsSetDefaultValuesFP( program ); // modifies the ucode
    else
        _jsSetDefaultValuesVP( program ); // modifies the push buffer

    // build the program data.
    //if (_jsCgParameterGenerateHandles(program, ptr, profileIndex)==0)
    //{
    // failed something, need to clean up
    //	jsFree(result);
    //	return 0;
    //}

    // not loaded yet
    //result->loadAddress = NULL;
    program->loadProgramId = GMM_ERROR;
    program->loadProgramOffset = 0;
    if ( profileIndex == FRAGMENT_PROFILE_INDEX )
    {
        // always load fragment shaders.
        _jsAllocSetHint( _JS_ALLOCTYPE_FRAGMENT_PROGRAM, ( GLuint ) program->id );
        int loaded = _jsLoadFPShader( program );
        _jsAllocClearHint();
        if ( ! loaded )
        {
            //TODO: what do we need to delete here ?
            //jsFree(program->paramArray);
            //jsFree(result);
            _jsCgRaiseError( CG_MEMORY_ALLOC_ERROR );
            return 0;
        }
    }

    program->programGroup = NULL;
    program->programIndexInGroup = -1;

    return 1;
}


int _jsPlatformGenerateFragmentProgram( _CGprogram *program, const CgProgramHeader *programHeader, const void *ucode,
                                        const CgParameterTableHeader *parameterHeader, const char *stringTable, const float *defaultValues )
{
    // we currently have the same interface for vertex and fragment programs.
    return _jsGcmGenerateProgram( program, FRAGMENT_PROFILE_INDEX, programHeader, ucode, parameterHeader, NULL, stringTable, defaultValues );

}

void _jsPlatformVertexProgramErase( void* platformProgram )
{
    _CGprogram* program = ( _CGprogram* )platformProgram;
    if ( program->runtimeParameters )
        jsFree( program->runtimeParameters );

    //TODO: tmp: clean up shader resources here
    if ( program->memoryBlock )
        jsFree( program->memoryBlock );

    if ( program->samplerIndices )
    {
        jsFree( program->samplerValuesLocation );
        jsFree( program->samplerIndices );
        jsFree( program->samplerUnits );
    }

    if ( program->constantPushBufferPointers )
        jsFree( program->constantPushBufferPointers );

    //jsFree(program->ucode);

    // allocated in the program generation
    //jsFree(platformProgram);
}

// erase a platform fragment program. This undoes _generateProgramFromNvABI
//TODO: fix the signature
void _jsPlatformFragmentProgramErase( void* platformProgram )
{
    _CGprogram* program = ( _CGprogram* )platformProgram;

    // if the shader has video memory assigned, free it.
    _jsUnloadFPShader( program );

    if ( program->runtimeParameters )
        jsFree( program->runtimeParameters );

    if ( program->memoryBlock )
        jsFree( program->memoryBlock );

    if ( program->samplerIndices )
    {
        jsFree( program->samplerValuesLocation );
        jsFree( program->samplerIndices );
        jsFree( program->samplerUnits );
    }

    if ( program->constantPushBufferPointers )
        jsFree( program->constantPushBufferPointers );

    //jsFree(program->ucode);

    // allocated in the program generation
    //jsFree(p);
}


void _jsPlatformProgramErase( void* platformProgram )
{
    _CGprogram* program = ( _CGprogram* )platformProgram;

    if ( program->loadProgramId != GMM_ERROR )
        _jsUnloadFPShader( program );

    //free the runtime parameters
    if ( program->runtimeParameters )
    {
        //need to erase all the program parameter "names"
        int i;
        int count = ( int )program->rtParametersCount;
        for ( i = 0;i < count;i++ )
        {
            //TODO: here we can't check that the current PSGL context is the one used to create the name
            //the good solution would probably to keep a pointer on the PSGL context in the cgContext
            _jsEraseName( &_CurrentContext->cgParameterNameSpace, ( jsName )program->runtimeParameters[i].id );
        }
        jsFree( program->runtimeParameters );
    }

    //free the push buffer block
    if ( program->memoryBlock )
        jsFree( program->memoryBlock );

    //free the samplers lookup tables
    if ( program->samplerIndices )
    {
        jsFree( program->samplerValuesLocation );
        jsFree( program->samplerIndices );
        jsFree( program->samplerUnits );
    }

    //free the "pointers" on the push buffer used for fast access
    if ( program->constantPushBufferPointers )
        jsFree( program->constantPushBufferPointers );
}


CGbool _jsPlatformSupportsFragmentProgram( CGprofile p )
{
    if ( p == CG_PROFILE_SCE_FP_TYPEB )
        return CG_TRUE;
    if ( CG_PROFILE_SCE_FP_RSX == p )
        return CG_TRUE;
    return CG_FALSE;
}

CGprofile _jsPlatformGetLatestProfile( CGGLenum profile_type )
{
    switch ( profile_type )
    {
        case CG_GL_VERTEX:
            return CG_PROFILE_SCE_VP_RSX;
        case CG_GL_FRAGMENT:
            return CG_PROFILE_SCE_FP_RSX;
        default:
            break;
    }
    return CG_PROFILE_UNKNOWN;
}

//TODO: use a ref mechanism for the string table or duplicate it !
int _jsPlatformCopyProgram( _CGprogram* source, _CGprogram* destination )
{
    //extract the layout of the parameter buffers from the source
    CgParameterTableHeader parameterHeader;
    parameterHeader.entryCount = source->rtParametersCount;
    parameterHeader.resourceTableOffset = ( uintptr_t )(( char* )source->resources - ( char* )source->parametersEntries + sizeof( CgParameterTableHeader ) );
    parameterHeader.defaultValueIndexCount = source->defaultValuesIndexCount;
    parameterHeader.defaultValueIndexTableOffset = ( uintptr_t )(( char* )source->defaultValuesIndices - ( char* )source->parametersEntries + sizeof( CgParameterTableHeader ) );
    parameterHeader.semanticIndexCount = source->semanticCount;
    parameterHeader.semanticIndexTableOffset = ( uintptr_t )(( char* )source->defaultValuesIndices - ( char* )source->parametersEntries + sizeof( CgParameterTableHeader ) );

    int profileIndex;

    //allocate the copy of the program
    switch ( source->header.profile )
    {
        case CG_PROFILE_SCE_VP_TYPEB:
            //case CG_PROFILE_SCE_VP_TYPEC:
        case CG_PROFILE_SCE_VP_RSX:
            profileIndex = VERTEX_PROFILE_INDEX;
            break;

        case CG_PROFILE_SCE_FP_TYPEB:
            //case CG_PROFILE_SCE_FP_TYPEC:
        case CG_PROFILE_SCE_FP_RSX:
            profileIndex = FRAGMENT_PROFILE_INDEX;
            break;
        default:
            return 0;
    }
    return _jsGcmGenerateProgram( destination, profileIndex, &source->header, source->ucode, &parameterHeader, source->parametersEntries, source->stringTable, source->defaultValues );
}


// Previously from Shader.cpp

//---------------------------------------------------------------------------------------------------------
#define ROW_MAJOR 0
#define COL_MAJOR 1

//This function creates the push buffer and the related structures
void _jsCreatePushBuffer( _CGprogram *program )
{
    //first pass to compute the space needed
    int bufferSize = 0;
    int programPushBufferPointersSize = 0;
    int extraStorageInWords = 0;
    int offsetCount = 0;
    int samplerCount = 0;
    int profileIndex = ( program->header.profile == CG_PROFILE_SCE_FP_TYPEB || //program->header.profile==CG_PROFILE_SCE_FP_TYPEC ||
                         program->header.profile == CG_PROFILE_SCE_FP_RSX ) ? FRAGMENT_PROFILE_INDEX : VERTEX_PROFILE_INDEX;
#ifdef JS_DEBUG
    unsigned int maxTextureUnits = ( profileIndex == VERTEX_PROFILE_INDEX ) ? _JS_MAX_VERTEX_TEXTURE_IMAGE_UNITS : _JS_MAX_TEXTURE_IMAGE_UNITS;
#endif

    bool hasSharedParams = false;
    int arrayCount = 1;
    for ( int i = 0;i < program->rtParametersCount;i++ )
    {
        const CgParameterEntry *parameterEntry = program->parametersEntries + i;

        //skip the unrolled arrays and the structures
        if (( parameterEntry->flags & CGP_STRUCTURE ) || ( parameterEntry->flags & CGP_UNROLLED ) )
        {
            arrayCount = 1;
            continue;
        }

        if (( parameterEntry->flags & CGPF_REFERENCED ) )
        {
            if ( parameterEntry->flags & CGP_ARRAY )
            {
                const CgParameterArray *parameterArray = _jsGetParameterArray( program, parameterEntry );
                arrayCount = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
                continue;
            }
            if (( parameterEntry->flags & CGPV_MASK ) == CGPV_UNIFORM )
            {
                const CgParameterResource *parameterResource = _jsGetParameterResource( program, parameterEntry );
                if ( parameterResource->type >= CG_SAMPLER1D &&  parameterResource->type <= CG_SAMPLERCUBE )
                {
                    JS_ASSERT( parameterResource->resource >= CG_TEXUNIT0 && parameterResource->resource < CG_TEXUNIT0 + ( int )maxTextureUnits );
                    JS_ASSERT( arrayCount == 1 && "array of samplers: implementation not tested" );
                    // store 1 sampler and 1 offset for texture samplers.
                    offsetCount += arrayCount;
                    samplerCount += arrayCount;
                }
                else if ( profileIndex == VERTEX_PROFILE_INDEX )
                {
                    if ( parameterResource->type == CGP_SCF_BOOL )
                    {
                        //do nothing
                    }
                    else if ( !( parameterEntry->flags & CGPF_SHARED ) )
                    {
                        int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
                        if ( parameterEntry->flags & CGP_CONTIGUOUS )
                            bufferSize += 3 + 4 * arrayCount * registerStride;
                        else
                        {
                            programPushBufferPointersSize += arrayCount;
                            int resourceIndex = parameterResource->resource;
                            int referencedSize = 3 + 4 * registerStride;
                            int notReferencedSize = 4 * registerStride;
                            for ( int j = 0;j < arrayCount;j++, resourceIndex += registerStride )
                            {
                                //I use the programPushBuffer pointer so it's valid to have element in an array without any affectation
                                if ( program->resources[resourceIndex] != 0xffff )
                                    bufferSize += referencedSize; //referenced: push buffer
                                else
                                    extraStorageInWords += notReferencedSize; //not referenced , extra storage location
                            }
                        }
                    }
                    else
                    {
                        hasSharedParams = true;
                        if ( !( parameterEntry->flags & CGP_CONTIGUOUS ) )
                        {
                            programPushBufferPointersSize += arrayCount;
                        }
                    }
                }
                else //profileIndex == FRAGMENT_PROFILE_INDEX
                {
                    int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
                    if ( !( parameterEntry->flags & CGPF_SHARED ) )
                    {
                        //TODO: check this case
                        extraStorageInWords += 4 * arrayCount * registerStride;
                    }
                    else
                    {
                        hasSharedParams = true;
                        unsigned short *resource = program->resources + parameterResource->resource;
                        for ( int j = 0;j < arrayCount*registerStride;j++ )
                        {
                            resource++;
                            unsigned short count = *resource++;
                            bufferSize += 24 * count;
                            resource += count;
                        }
                    }
                }
            }
        }
        arrayCount = 1;
    }

    if (( profileIndex == FRAGMENT_PROFILE_INDEX ) && ( hasSharedParams ) )
    {
        bufferSize += 8 + 3 + 2; // GCM_PORT_TESTED [CEDRIC] +3 for the channel switch that gcm does + 2 for the OUT end
    }

    bufferSize = _jsPad( bufferSize, 4 );

    //allocate the buffer(s)
    unsigned int storageSizeInWords = bufferSize + extraStorageInWords;
    if ( storageSizeInWords )
        program->memoryBlock = ( unsigned int* )jsMemalign( 16, storageSizeInWords * 4 );
    else
        program->memoryBlock = NULL;

    //TODO: this is tmp
    program->samplerCount = samplerCount;
    if ( samplerCount )
    {
        program->samplerValuesLocation = ( GLuint* )jsMalloc( samplerCount * sizeof( GLuint ) );
        program->samplerIndices = ( GLuint* )jsMalloc( samplerCount * sizeof( GLuint ) );
        program->samplerUnits = ( GLuint* )jsMalloc( samplerCount * sizeof( GLuint ) );
    }
    else
    {
        program->samplerValuesLocation = NULL;
        program->samplerIndices = NULL;
        program->samplerUnits = NULL;
    }

    GLuint *samplerValuesLocation = program->samplerValuesLocation;
    GLuint *samplerIndices = program->samplerIndices;
    GLuint *samplerUnits = program->samplerUnits;

    if ( programPushBufferPointersSize )
        program->constantPushBufferPointers = ( unsigned int** )jsMalloc( programPushBufferPointersSize * 4 );
    else
        program->constantPushBufferPointers = NULL;

    jsFifoData *jsGcmCurrent = ( jsFifoData * )program->memoryBlock;
    program->constantPushBuffer = ( bufferSize > 0 ) ? ( unsigned int * )jsGcmCurrent : NULL;
    unsigned int **programPushBuffer = program->constantPushBufferPointers;
    program->constantPushBufferWordSize = bufferSize;
    GLuint *currentStorage = ( GLuint * )( jsGcmCurrent + bufferSize );

    int outOfMemory = 0;
    //second pass to fill the buffer
    arrayCount = 1;
    const CgParameterEntry *containerEntry = NULL;
    for ( int i = 0;i < program->rtParametersCount;i++ )
    {
        CgRuntimeParameter *rtParameter = program->runtimeParameters + i;
        const CgParameterEntry *parameterEntry = program->parametersEntries + i;
        if ( containerEntry == NULL )
            containerEntry = parameterEntry;

        //rtParameter->setter = _cgRaiseInvalidParam;
        //rtParameter->setterr = _cgRaiseNotMatrixParam;
        //rtParameter->setterc = _cgRaiseNotMatrixParam;
        rtParameter->samplerSetter = _cgRaiseInvalidParamIndex;

        //tentative
        rtParameter->setterIndex = _cgRaiseInvalidParamIndex;
        rtParameter->setterrIndex = _cgRaiseNotMatrixParamIndex;
        rtParameter->settercIndex = _cgRaiseNotMatrixParamIndex;

        CGparameter id = ( CGparameter )_jsCreateName( &_CurrentContext->cgParameterNameSpace, ( void* )rtParameter );
        if ( !id )
        {
            outOfMemory = 1;
            break;
        }

        rtParameter->id = id;
        rtParameter->parameterEntry = parameterEntry;
        rtParameter->program = program;

        //skip the unrolled arrays and the structures
        if (( parameterEntry->flags & CGP_STRUCTURE ) || ( parameterEntry->flags & CGP_UNROLLED ) )
        {
            arrayCount = 1;
            containerEntry = NULL;
            continue;
        }

        if ( parameterEntry->flags & CGPF_REFERENCED )
        {
            if ( parameterEntry->flags & CGP_ARRAY )
            {
                const CgParameterArray *parameterArray = _jsGetParameterArray( program, parameterEntry );
                arrayCount = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
                //continue to the next item
                continue;
            }
            if (( parameterEntry->flags & CGPV_MASK ) == CGPV_UNIFORM )
            {
                //TODO: rtParameter->defaultNormalize = CG_FALSE;
                rtParameter->glType = GL_NONE;
                //TODO: needed ? rtParameter->flags = 0;
                const CgParameterResource *parameterResource = _jsGetParameterResource( program, parameterEntry );
                if ( parameterResource->type >= CG_SAMPLER1D && parameterResource->type <= CG_SAMPLERCUBE )
                {
                    //TODO
                    rtParameter->pushBufferPointer = samplerValuesLocation;
                    // initialize the texture name to zero, used by the setSamplerfp call to _jsBindTextureInternal
                    *samplerValuesLocation = 0;
                    samplerValuesLocation++;

                    // store the texture unit indices.
                    *samplerIndices = i;
                    samplerIndices++;
                    *samplerUnits = parameterResource->resource - CG_TEXUNIT0;
                    samplerUnits++;

                    // XXX the setter is called when validating vertex programs.
                    // this would cause a CG error.
                    // the parameters should have a "validate" function instead
                    if ( profileIndex == VERTEX_PROFILE_INDEX )
                    {
                        rtParameter->setterIndex = _cgIgnoreSetParamIndex;
                        rtParameter->samplerSetter = setSamplervp;
                    }
                    else
                    {
                        rtParameter->samplerSetter = setSamplerfp;
                    }
                    rtParameter->glType = _jsCgGetSamplerGLTypeFromCgType(( CGtype )( parameterResource->type ) );
                }
                else
                {
                    if ( profileIndex == VERTEX_PROFILE_INDEX )
                    {
                        if ( parameterResource->type == CGP_SCF_BOOL )
                        {
                            //do nothing
                        }
                        else if ( !( parameterEntry->flags & CGPF_SHARED ) )
                        {
                            int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
                            int registerCount = arrayCount * registerStride;
                            if ( parameterEntry->flags & CGP_CONTIGUOUS )
                            {
                                memset( jsGcmCurrent, 0, 4*( 4*registerCount + 3 ) );
                                GCM_FUNC_BUFFERED( cellGcmSetVertexProgramParameterBlock, jsGcmCurrent, parameterResource->resource, registerCount, ( float* )jsGcmCurrent );
                                rtParameter->pushBufferPointer = jsGcmCurrent - 4 * registerCount;
                            }
                            else
                            {
                                rtParameter->pushBufferPointer = programPushBuffer;
                                int resourceIndex = parameterResource->resource;
                                for ( int j = 0;j < arrayCount;j++, resourceIndex += registerStride )
                                {
                                    //I use the programPushBuffer pointer so it's valid to have element in an array without any affectation
                                    if ( program->resources[resourceIndex] != 0xffff )
                                    {
                                        memset( jsGcmCurrent, 0, 4*( 4*registerStride + 3 ) );
                                        GCM_FUNC_BUFFERED( cellGcmSetVertexProgramParameterBlock, jsGcmCurrent, program->resources[resourceIndex], registerStride, ( float* )jsGcmCurrent ); // GCM_PORT_TESTED [KHOFF]
                                        *( programPushBuffer++ ) = ( unsigned int* )( jsGcmCurrent - 4 * registerStride );
                                    }
                                    else
                                    {
                                        //This case is when there is an array item which is not referenced
                                        //we still call tbe setter function, so we have to store the info somewhere...
                                        //and we need to return the value previously set in case of the user asks for a get
                                        *( programPushBuffer++ ) = ( unsigned int* )currentStorage;
                                        currentStorage += 4 * registerStride;
                                    }
                                }
                            }
                        }
                        else
                        {
                            jsGcmDriver *driver = _jsGetGcmDriver();
                            if ( parameterEntry->flags & CGP_CONTIGUOUS )
                            {
                                rtParameter->pushBufferPointer = driver->sharedVPConstants + parameterResource->resource * 4 * sizeof( float );
                            }
                            else
                            {
                                int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
                                int registerCount = arrayCount * registerStride;
                                for ( int j = 0;j < registerCount;j += registerStride )
                                {
                                    *programPushBuffer = ( unsigned int* )driver->sharedVPConstants + program->resources[parameterResource->resource+j] * 4 * sizeof( float );
                                    rtParameter->pushBufferPointer = programPushBuffer++;
                                }
                            }
                        }
                    }
                    else //if (profileIndex == FRAGMENT_PROFILE_INDEX)
                    {
                        if ( parameterEntry->flags & CGPF_SHARED )
                        {
                            // XXX needs an offset for the get
                            rtParameter->pushBufferPointer = NULL;
                        }
                        else
                        {
                            int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
                            int registerCount = arrayCount * registerStride;
                            rtParameter->pushBufferPointer = currentStorage;
                            currentStorage += 4 * registerCount;
                        }
                    }

                    switch ( parameterResource->type )
                    {
                        case CG_FLOAT:
                        case CG_FLOAT1: case CG_FLOAT2: case CG_FLOAT3: case CG_FLOAT4:
			    // if this gets updated, don't forget the halfs below
                        {
                            unsigned int floatCount = _jsCountFloatsInCgType(( CGtype )parameterResource->type );
                            rtParameter->setterIndex = setVectorTypeIndex[( containerEntry->flags&CGP_ARRAY ) ? 1 : 0][( containerEntry->flags&CGPF_SHARED ) ? 1 : 0][profileIndex][floatCount - 1];
                        }
                        break;
                        case CG_FLOAT1x1: case CG_FLOAT1x2: case CG_FLOAT1x3: case CG_FLOAT1x4:
                        case CG_FLOAT2x1: case CG_FLOAT2x2: case CG_FLOAT2x3: case CG_FLOAT2x4:
                        case CG_FLOAT3x1: case CG_FLOAT3x2: case CG_FLOAT3x3: case CG_FLOAT3x4:
                        case CG_FLOAT4x1: case CG_FLOAT4x2: case CG_FLOAT4x3: case CG_FLOAT4x4:
			    // if this gets updated, don't forget the halfs below
                            rtParameter->setterrIndex = setMatrixTypeIndex[( containerEntry->flags&CGP_ARRAY ) ? 1 : 0][( containerEntry->flags&CGPF_SHARED ) ? 1 : 0][profileIndex][_jsGetTypeRowCount(( CGtype )parameterResource->type ) - 1][_jsGetTypeColCount(( CGtype )parameterResource->type ) - 1][ROW_MAJOR];
                            rtParameter->settercIndex = setMatrixTypeIndex[( containerEntry->flags&CGP_ARRAY ) ? 1 : 0][( containerEntry->flags&CGPF_SHARED ) ? 1 : 0][profileIndex][_jsGetTypeRowCount(( CGtype )parameterResource->type ) - 1][_jsGetTypeColCount(( CGtype )parameterResource->type ) - 1][COL_MAJOR];
                            break;
                        case CG_SAMPLER1D: case CG_SAMPLER2D: case CG_SAMPLER3D: case CG_SAMPLERRECT: case CG_SAMPLERCUBE:
                            // A used sampler that does not have a TEXUNIT resource ?
                            // not sure if we ever go here.
                            break;
                        case CGP_SCF_BOOL:
                            break;
                        case CG_HALF:
                        case CG_HALF1: case CG_HALF2: case CG_HALF3: case CG_HALF4:
                        case CG_INT:
                        case CG_INT1: case CG_INT2: case CG_INT3: case CG_INT4:
                        case CG_BOOL:
                        case CG_BOOL1: case CG_BOOL2: case CG_BOOL3: case CG_BOOL4:
                        case CG_FIXED:
                        case CG_FIXED1: case CG_FIXED2: case CG_FIXED3: case CG_FIXED4:
                        {
                            unsigned int floatCount = _jsCountFloatsInCgType(( CGtype )parameterResource->type );
                            rtParameter->setterIndex = setVectorTypeIndex[( containerEntry->flags&CGP_ARRAY ) ? 1 : 0][( containerEntry->flags&CGPF_SHARED ) ? 1 : 0][profileIndex][floatCount - 1];
                        }
                        break;
                        case CG_HALF1x1: case CG_HALF1x2: case CG_HALF1x3: case CG_HALF1x4:
                        case CG_HALF2x1: case CG_HALF2x2: case CG_HALF2x3: case CG_HALF2x4:
                        case CG_HALF3x1: case CG_HALF3x2: case CG_HALF3x3: case CG_HALF3x4:
                        case CG_HALF4x1: case CG_HALF4x2: case CG_HALF4x3: case CG_HALF4x4:
                        case CG_INT1x1: case CG_INT1x2: case CG_INT1x3: case CG_INT1x4:
                        case CG_INT2x1: case CG_INT2x2: case CG_INT2x3: case CG_INT2x4:
                        case CG_INT3x1: case CG_INT3x2: case CG_INT3x3: case CG_INT3x4:
                        case CG_INT4x1: case CG_INT4x2: case CG_INT4x3: case CG_INT4x4:
                        case CG_BOOL1x1: case CG_BOOL1x2: case CG_BOOL1x3: case CG_BOOL1x4:
                        case CG_BOOL2x1: case CG_BOOL2x2: case CG_BOOL2x3: case CG_BOOL2x4:
                        case CG_BOOL3x1: case CG_BOOL3x2: case CG_BOOL3x3: case CG_BOOL3x4:
                        case CG_BOOL4x1: case CG_BOOL4x2: case CG_BOOL4x3: case CG_BOOL4x4:
                        case CG_FIXED1x1: case CG_FIXED1x2: case CG_FIXED1x3: case CG_FIXED1x4:
                        case CG_FIXED2x1: case CG_FIXED2x2: case CG_FIXED2x3: case CG_FIXED2x4:
                        case CG_FIXED3x1: case CG_FIXED3x2: case CG_FIXED3x3: case CG_FIXED3x4:
                        case CG_FIXED4x1: case CG_FIXED4x2: case CG_FIXED4x3: case CG_FIXED4x4:
                            rtParameter->setterrIndex = setMatrixTypeIndex[( containerEntry->flags&CGP_ARRAY ) ? 1 : 0][( containerEntry->flags&CGPF_SHARED ) ? 1 : 0][profileIndex][_jsGetTypeRowCount(( CGtype )parameterResource->type ) - 1][_jsGetTypeColCount(( CGtype )parameterResource->type ) - 1][ROW_MAJOR];
                            rtParameter->settercIndex = setMatrixTypeIndex[( containerEntry->flags&CGP_ARRAY ) ? 1 : 0][( containerEntry->flags&CGPF_SHARED ) ? 1 : 0][profileIndex][_jsGetTypeRowCount(( CGtype )parameterResource->type ) - 1][_jsGetTypeColCount(( CGtype )parameterResource->type ) - 1][COL_MAJOR];
                            break;
						// addition to be compatible with cgc 2.0 
						case CG_STRING:
							break;
                        default:
                            JS_ASSERT( 0 );
                            break;
                    }
                }
            }
            else if (( parameterEntry->flags & CGPV_MASK ) == CGPV_VARYING )
            {
                if (( parameterEntry->flags & CGPD_MASK ) == CGPD_IN && profileIndex == VERTEX_PROFILE_INDEX )
                {
                    rtParameter->setterIndex = setAttribConstantIndex;
                }
            }
        }
        else
        {
            if (( parameterEntry->flags & CGPV_MASK ) == CGPV_UNIFORM )
            {
                if ( parameterEntry->flags & CGP_ARRAY )
                    continue;

                const CgParameterResource *parameterResource = _jsGetParameterResource( program, parameterEntry );
                // we silently ignore valid sets on unused parameters.
                switch ( parameterResource->type )
                {
                    case CG_FLOAT:
                    case CG_FLOAT1: case CG_FLOAT2: case CG_FLOAT3: case CG_FLOAT4:
                        //rtParameter->setter = _cgIgnoreSetParam;
                        rtParameter->setterIndex = _cgIgnoreSetParamIndex;
                        break;
                    case CG_FLOAT1x1: case CG_FLOAT1x2: case CG_FLOAT1x3: case CG_FLOAT1x4:
                    case CG_FLOAT2x1: case CG_FLOAT2x2: case CG_FLOAT2x3: case CG_FLOAT2x4:
                    case CG_FLOAT3x1: case CG_FLOAT3x2: case CG_FLOAT3x3: case CG_FLOAT3x4:
                    case CG_FLOAT4x1: case CG_FLOAT4x2: case CG_FLOAT4x3: case CG_FLOAT4x4:
                        //rtParameter->setterr = _cgIgnoreSetParam;
                        //rtParameter->setterc = _cgIgnoreSetParam;
                        rtParameter->setterrIndex = _cgIgnoreSetParamIndex;
                        rtParameter->settercIndex = _cgIgnoreSetParamIndex;
                        break;
                    case CG_SAMPLER1D: case CG_SAMPLER2D: case CG_SAMPLER3D: case CG_SAMPLERRECT: case CG_SAMPLERCUBE:
                        rtParameter->samplerSetter = _cgIgnoreSetParamIndex;
                        break;
                    case CGP_SCF_BOOL:
                        break;
		    case CG_HALF:
                    case CG_HALF1: case CG_HALF2: case CG_HALF3: case CG_HALF4:
		    case CG_INT:
                    case CG_INT1: case CG_INT2: case CG_INT3: case CG_INT4:
		    case CG_BOOL:
                    case CG_BOOL1: case CG_BOOL2: case CG_BOOL3: case CG_BOOL4:
		    case CG_FIXED:
                    case CG_FIXED1: case CG_FIXED2: case CG_FIXED3: case CG_FIXED4:
                        //rtParameter->setter = _cgIgnoreSetParam;
                        rtParameter->setterIndex = _cgIgnoreSetParamIndex;
                        break;
                    case CG_HALF1x1: case CG_HALF1x2: case CG_HALF1x3: case CG_HALF1x4:
                    case CG_HALF2x1: case CG_HALF2x2: case CG_HALF2x3: case CG_HALF2x4:
                    case CG_HALF3x1: case CG_HALF3x2: case CG_HALF3x3: case CG_HALF3x4:
                    case CG_HALF4x1: case CG_HALF4x2: case CG_HALF4x3: case CG_HALF4x4:
                    case CG_INT1x1: case CG_INT1x2: case CG_INT1x3: case CG_INT1x4:
                    case CG_INT2x1: case CG_INT2x2: case CG_INT2x3: case CG_INT2x4:
                    case CG_INT3x1: case CG_INT3x2: case CG_INT3x3: case CG_INT3x4:
                    case CG_INT4x1: case CG_INT4x2: case CG_INT4x3: case CG_INT4x4:
                    case CG_BOOL1x1: case CG_BOOL1x2: case CG_BOOL1x3: case CG_BOOL1x4:
                    case CG_BOOL2x1: case CG_BOOL2x2: case CG_BOOL2x3: case CG_BOOL2x4:
                    case CG_BOOL3x1: case CG_BOOL3x2: case CG_BOOL3x3: case CG_BOOL3x4:
                    case CG_BOOL4x1: case CG_BOOL4x2: case CG_BOOL4x3: case CG_BOOL4x4:
                    case CG_FIXED1x1: case CG_FIXED1x2: case CG_FIXED1x3: case CG_FIXED1x4:
                    case CG_FIXED2x1: case CG_FIXED2x2: case CG_FIXED2x3: case CG_FIXED2x4:
                    case CG_FIXED3x1: case CG_FIXED3x2: case CG_FIXED3x3: case CG_FIXED3x4:
                    case CG_FIXED4x1: case CG_FIXED4x2: case CG_FIXED4x3: case CG_FIXED4x4:
                        //rtParameter->setterr = _cgIgnoreSetParam;
                        //rtParameter->setterc = _cgIgnoreSetParam;
                        rtParameter->setterrIndex = _cgIgnoreSetParamIndex;
                        rtParameter->settercIndex = _cgIgnoreSetParamIndex;
                        break;
						// addition to be compatible with cgc 2.0 
						case CG_STRING:
							break;
                    default:
                        JS_ASSERT( 0 );
                        break;
                }
            }
            else if (( parameterEntry->flags & CGPV_MASK ) == CGPV_VARYING )
            {
                if (( parameterEntry->flags & CGPD_MASK ) == CGPD_IN && profileIndex == VERTEX_PROFILE_INDEX )
                {
                    rtParameter->setterIndex = setAttribConstantIndex;
                }
            }
        }
        arrayCount = 1;
        containerEntry = NULL;
    }
    JS_ASSERT( programPushBuffer == program->constantPushBufferPointers + programPushBufferPointersSize );

    //add padding
    if ( bufferSize > 0 )
    {
      int nopCount = ( program->constantPushBuffer + bufferSize ) - ( unsigned int * )jsGcmCurrent;
      JS_ASSERT( nopCount >= 0 );
      GCM_FUNC_BUFFERED( cellGcmSetNopCommand, jsGcmCurrent, nopCount ); // GCM_PORT_TESTED [KHOFF]
    }
}

//this function sets the embedded constant to their default value in the ucode of a fragment shader
//it's called at setup time right after loading the program. this function could be removed if the
//default values were already in the shader code
void _jsSetDefaultValuesVP( _CGprogram *program )
{

#ifdef JS_DEBUG
    int profileIndex = ( program->header.profile == CG_PROFILE_SCE_FP_TYPEB || //program->header.profile==CG_PROFILE_SCE_FP_TYPEC ||
                         program->header.profile == CG_PROFILE_SCE_FP_RSX ) ? FRAGMENT_PROFILE_INDEX : VERTEX_PROFILE_INDEX;
    JS_ASSERT( profileIndex == VERTEX_PROFILE_INDEX );
#endif

    int count = program->defaultValuesIndexCount;
    for ( int i = 0;i < count;i++ )
    {
        int index = ( int )program->defaultValuesIndices[i].entryIndex;
        CgRuntimeParameter *rtParameter = program->runtimeParameters + index;

        int arrayCount = 1;
        const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
        bool isArray = false;
        if ( parameterEntry->flags & CGP_ARRAY )
        {
            isArray = true;
            const CgParameterArray *parameterArray = _jsGetParameterArray( program, parameterEntry );
            arrayCount = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
            parameterEntry++;
            rtParameter++;
        }

        if ( rtParameter->pushBufferPointer ) //unreferenced might have default values
        {
            const CgParameterResource *parameterResource = _jsGetParameterResource( program, parameterEntry );
            const float *itemDefaultValues = program->defaultValues + program->defaultValuesIndices[i].defaultValueIndex;
            int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
            if ( parameterEntry->flags & CGP_CONTIGUOUS )
                memcpy( rtParameter->pushBufferPointer, itemDefaultValues, arrayCount * registerStride *4*sizeof( float ) );
            else
            {
                unsigned int *pushBufferPointer = (( unsigned int * )rtParameter->pushBufferPointer );
                for ( int j = 0;j < arrayCount;j++ )
                {
                    unsigned int *pushBufferAddress = isArray ? ( *( unsigned int** )pushBufferPointer ) : pushBufferPointer;
                    memcpy( pushBufferAddress, itemDefaultValues, registerStride*4*sizeof( float ) );
                    pushBufferPointer += isArray ? 1 : 3 + registerStride * 4;
                    itemDefaultValues += 4 * registerStride;
                }
            }
        }
    }
}

void _jsSetDefaultValuesFP( _CGprogram *program )
{

#ifdef JS_DEBUG
    int profileIndex = ( program->header.profile == CG_PROFILE_SCE_FP_TYPEB || //program->header.profile==CG_PROFILE_SCE_FP_TYPEC ||
                         program->header.profile == CG_PROFILE_SCE_FP_RSX ) ? FRAGMENT_PROFILE_INDEX : VERTEX_PROFILE_INDEX;
    JS_ASSERT( profileIndex == FRAGMENT_PROFILE_INDEX );
#endif

    int count = program->defaultValuesIndexCount;
    for ( int i = 0;i < count;i++ )
    {
        const void * __restrict pItemDefaultValues = program->defaultValues + program->defaultValuesIndices[i].defaultValueIndex;
        const unsigned int * itemDefaultValues = ( const unsigned int * )pItemDefaultValues;
        int index = ( int )program->defaultValuesIndices[i].entryIndex;

        CgRuntimeParameter *rtParameter = program->runtimeParameters + index;
        float *hostMemoryCopy = ( float * )rtParameter->pushBufferPointer;

        if ( hostMemoryCopy ) //certain parameter are not referenced but still have a default value.
        {
            const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;
            int arrayCount = 1;
            if ( parameterEntry->flags & CGP_ARRAY )
            {
                const CgParameterArray *parameterArray = _jsGetParameterArray( program, parameterEntry );
                arrayCount = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
                i++;
                parameterEntry++;
            }
            const CgParameterResource *parameterResource = _jsGetParameterResource( program, parameterEntry );
            unsigned short *resource = program->resources + parameterResource->resource + 1; //+1 to skip the register
            int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
            int registerCount = arrayCount * registerStride;
            int j;
            for ( j = 0;j < registerCount;j++ )
            {
                unsigned short embeddedConstCount = *( resource++ );
                int k;
                for ( k = 0;k < embeddedConstCount;k++ )
                {
                    unsigned short ucodePatchOffset = *( resource )++;
                    unsigned int *dst = ( unsigned int* )(( char* )program->ucode + ucodePatchOffset );
                    dst[0] = SWAP_IF_BIG_ENDIAN( itemDefaultValues[0] );
                    dst[1] = SWAP_IF_BIG_ENDIAN( itemDefaultValues[1] );
                    dst[2] = SWAP_IF_BIG_ENDIAN( itemDefaultValues[2] );
                    dst[3] = SWAP_IF_BIG_ENDIAN( itemDefaultValues[3] );
                }
                memcpy(( void* )hostMemoryCopy, ( void* )itemDefaultValues, sizeof( float )*4 );
                hostMemoryCopy += 4;
                itemDefaultValues += 4;
                resource++; //skip the register of the next item
            }
        }
    }
}

// IMPORTANT: before calling here, the subchannel's context DMAs must already be configured.
void _jsPrepareFPSharedPushBuffer( _CGprogram *program, jsFifoData * &jsGcmCurrent )
{
    jsGcmDriver *driver = _jsGetGcmDriver();
    for ( int i = 0;i < program->rtParametersCount;i++ )
    {
        CgRuntimeParameter *rtParameter = program->runtimeParameters + i;
        const CgParameterEntry *parameterEntry = rtParameter->parameterEntry;

        if (( parameterEntry->flags & CGPF_REFERENCED ) && ( parameterEntry->flags & CGPF_SHARED ) && ( parameterEntry->flags & CGPV_MASK ) == CGPV_UNIFORM
                && !( parameterEntry->flags & CGP_STRUCTURE ) && !( parameterEntry->flags & CGP_UNROLLED ) )
        {
            int arrayCount = 1;
            if ( parameterEntry->flags & CGP_ARRAY )
            {
                const CgParameterArray *parameterArray = _jsGetParameterArray( program, parameterEntry );
                arrayCount = _jsGetSizeofSubArray( parameterArray->dimensions, parameterArray->dimensionCount );
                i++;
                parameterEntry++;
            }
            const CgParameterResource *parameterResource = _jsGetParameterResource( program, parameterEntry );
            unsigned short *ec = program->resources + parameterResource->resource;
            int registerStride = isMatrix(( CGtype )parameterResource->type ) ? _jsGetTypeRowCount(( CGtype )parameterResource->type ) : 1;
            int registerCount = arrayCount * registerStride;
            for ( int j = 0;j < registerCount;j++ )
            {
                unsigned int resIndex = ( unsigned int ) * ( ec++ ); //here's there is no contiguous bit set for the fp so we have the register for each item
                unsigned short embeddedConstCount = *( ec++ );
                if ( embeddedConstCount )
                {
					uint32_t srcOffset = gmmIdToOffset( driver->sharedFPConstantsId ) + 4*sizeof( float )*resIndex;
					for ( int k = 0;k < embeddedConstCount;k++ )
                    {
                        unsigned short embeddedConstantOffset = *( ec++ );
                        GCM_FUNC_BUFFERED( cellGcmSetTransferDataOffset, jsGcmCurrent, gmmIdToOffset( program->loadProgramId ) + program->loadProgramOffset + embeddedConstantOffset, srcOffset);
                    }
                }
            }
        }
    }
}
