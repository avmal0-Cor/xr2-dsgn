/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * Image functions, pixel format/type conversions
 */
#include <math.h>
#include <PSGL/psgl.h>
#include <PSGL/Debug.h>
#include <PSGL/Utils.h>
#include <PSGL/TypeUtils.h>
#include <PSGL/private.h>
#include <string.h>
#include <PSGL/PerfCounters.h>

// create GL_UNSIGNED_INT_24_8
//  In order to use the macro mechanism, a symbol for a packed type needs to
//  be constructed from its type and bit field specification.  But we can't
//  simply add a new base type to GL without the extension suffix.  We work
//  around this by defining the symbol only locally.
//
//  This type is intended for 24-bit depth buffer transfer.
#define GL_UNSIGNED_INT_24_8      GL_UNSIGNED_INT_24_8_SCE
#define GL_UNSIGNED_INT_8_24_REV  GL_UNSIGNED_INT_8_24_REV_SCE

#define GL_UNSIGNED_SHORT_8_8		GL_UNSIGNED_SHORT_8_8_SCE
#define GL_UNSIGNED_SHORT_8_8_REV	GL_UNSIGNED_SHORT_8_8_REV_SCE
#define GL_UNSIGNED_INT_16_16		GL_UNSIGNED_INT_16_16_SCE
#define GL_UNSIGNED_INT_16_16_REV	GL_UNSIGNED_INT_16_16_REV_SCE

// half floats are declared separately because they are not handled by the
// C language

// The GL_UNSIGNED_INT maximum value is specified as a double constant
// because float precision is not sufficient (int conversion produces 0).
#define DECLARE_C_TYPES \
DECLARE_TYPE(GL_BYTE,GLbyte,127.f) \
DECLARE_TYPE(GL_UNSIGNED_BYTE,GLubyte,255.f) \
DECLARE_TYPE(GL_SHORT,GLshort,32767.f) \
DECLARE_TYPE(GL_UNSIGNED_SHORT,GLushort,65535.f) \
DECLARE_TYPE(GL_INT,GLint,2147483647.f) \
DECLARE_TYPE(GL_UNSIGNED_INT,GLuint,4294967295.0) \
DECLARE_TYPE(GL_FIXED,GLfixed,65535.f)

#define DECLARE_UNPACKED_TYPES \
DECLARE_UNPACKED_TYPE(GL_BYTE) \
DECLARE_UNPACKED_TYPE(GL_UNSIGNED_BYTE) \
DECLARE_UNPACKED_TYPE(GL_SHORT) \
DECLARE_UNPACKED_TYPE(GL_UNSIGNED_SHORT) \
DECLARE_UNPACKED_TYPE(GL_INT) \
DECLARE_UNPACKED_TYPE(GL_UNSIGNED_INT) \
DECLARE_UNPACKED_TYPE(GL_HALF_FLOAT_ARB) \
DECLARE_UNPACKED_TYPE(GL_FLOAT) \
DECLARE_UNPACKED_TYPE(GL_FIXED)


#define DECLARE_PACKED_TYPES \
DECLARE_PACKED_TYPE_AND_REV_2(UNSIGNED_BYTE,4,4) \
DECLARE_PACKED_TYPE_AND_REV_2(UNSIGNED_BYTE,6,2) \
DECLARE_PACKED_TYPE_AND_REV_3(UNSIGNED_BYTE,3,3,2) \
DECLARE_PACKED_TYPE_AND_REV_4(UNSIGNED_BYTE,2,2,2,2) \
DECLARE_PACKED_TYPE_AND_REV_2(UNSIGNED_SHORT,12,4) \
DECLARE_PACKED_TYPE_AND_REV_2(UNSIGNED_SHORT,8,8) \
DECLARE_PACKED_TYPE_AND_REV_3(UNSIGNED_SHORT,5,6,5) \
DECLARE_PACKED_TYPE_AND_REV_4(UNSIGNED_SHORT,4,4,4,4) \
DECLARE_PACKED_TYPE_AND_REV_4(UNSIGNED_SHORT,5,5,5,1) \
DECLARE_PACKED_TYPE_AND_REV_2(UNSIGNED_INT,16,16) \
DECLARE_PACKED_TYPE_AND_REV_2(UNSIGNED_INT,24,8) \
DECLARE_PACKED_TYPE_AND_REV_4(UNSIGNED_INT,8,8,8,8) \
DECLARE_PACKED_TYPE_AND_REV_4(UNSIGNED_INT,10,10,10,2)

#define DECLARE_FORMATS \
DECLARE_FORMAT(GL_RGB,3) \
DECLARE_FORMAT(GL_BGR,3) \
DECLARE_FORMAT(GL_RGBA,4) \
DECLARE_FORMAT(GL_BGRA,4) \
DECLARE_FORMAT(GL_ABGR,4) \
DECLARE_FORMAT(GL_ARGB_SCE,4) \
DECLARE_FORMAT(GL_RED,1) \
DECLARE_FORMAT(GL_GREEN,1) \
DECLARE_FORMAT(GL_BLUE,1) \
DECLARE_FORMAT(GL_ALPHA,1) \
DECLARE_FORMAT(GL_LUMINANCE,1) \
DECLARE_FORMAT(GL_LUMINANCE_ALPHA,2) \
DECLARE_FORMAT(GL_ALPHA_LUMINANCE_SCE,2) \
DECLARE_FORMAT(GL_DEPTH_COMPONENT,1)

#define DECLARE_TYPE(TYPE,CTYPE,MAXVAL) \
typedef CTYPE type_##TYPE; \
static inline type_##TYPE _jsFloatTo_##TYPE(float v) { return (type_##TYPE)(_jsClampf(v)*MAXVAL); } \
static inline float _jsFloatFrom_##TYPE(type_##TYPE v) { return ((float)v)/MAXVAL; }
DECLARE_C_TYPES
#undef DECLARE_TYPE

typedef GLfloat type_GL_FLOAT;
static inline type_GL_FLOAT _jsFloatTo_GL_FLOAT( float v ) {return v;}
static inline float _jsFloatFrom_GL_FLOAT( type_GL_FLOAT v ) {return v;}

typedef GLhalfARB type_GL_HALF_FLOAT_ARB;
static inline type_GL_HALF_FLOAT_ARB _jsFloatTo_GL_HALF_FLOAT_ARB( float x ) {return _jsFloatToHalf( x );}
static inline float _jsFloatFrom_GL_HALF_FLOAT_ARB( type_GL_HALF_FLOAT_ARB x ) {return _jsHalfToFloat( x );}

#define DECLARE_PACKED_TYPE_AND_REV_2(REALTYPE,S1,S2) \
DECLARE_PACKED_TYPE(GL_##REALTYPE,GL_##REALTYPE##_##S1##_##S2,2,S1,S2,0,0,) \
DECLARE_PACKED_TYPE(GL_##REALTYPE,GL_##REALTYPE##_##S2##_##S1##_REV,2,S2,S1,0,0,_REV)

#define DECLARE_PACKED_TYPE_AND_REV_3(REALTYPE,S1,S2,S3) \
DECLARE_PACKED_TYPE(GL_##REALTYPE,GL_##REALTYPE##_##S1##_##S2##_##S3,3,S1,S2,S3,0,) \
DECLARE_PACKED_TYPE(GL_##REALTYPE,GL_##REALTYPE##_##S3##_##S2##_##S1##_REV,3,S3,S2,S1,0,_REV)

#define DECLARE_PACKED_TYPE_AND_REV_4(REALTYPE,S1,S2,S3,S4) \
DECLARE_PACKED_TYPE(GL_##REALTYPE,GL_##REALTYPE##_##S1##_##S2##_##S3##_##S4,4,S1,S2,S3,S4,) \
DECLARE_PACKED_TYPE(GL_##REALTYPE,GL_##REALTYPE##_##S4##_##S3##_##S2##_##S1##_REV,4,S4,S3,S2,S1,_REV)

#define DECLARE_PACKED_TYPE_AND_REALTYPE(REALTYPE,N,S1,S2,S3,S4,REV) \
DECLARE_PACKED_TYPE(GL_##REALTYPE,PACKED_TYPE(REALTYPE,N,S1,S2,S3,S4,REV),N,S1,S2,S3,S4,REV)

#define INDEX(N,X) (X)
#define INDEX_REV(N,X) (N-1-X)

#define GET_BITS(to,from,first,count) if ((count)>0) to=((GLfloat)(((from)>>(first))&((1<<(count))-1)))/(GLfloat)((1<<((count==0)?1:count))-1)
#define PUT_BITS(from,to,first,count) if ((count)>0) to|=((unsigned int)((from)*((GLfloat)((1<<((count==0)?1:count))-1))))<<(first);

GLboolean _jsIsType( GLenum type )
{
    switch ( type )
    {
#define DECLARE_UNPACKED_TYPE(TYPE) \
		case TYPE:
            DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE
#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
		case TYPE:
            DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE
            return GL_TRUE;
        default:
            return GL_FALSE;
    }
}

GLboolean _jsIsFormat( GLenum format )
{
    switch ( format )
    {
#define DECLARE_FORMAT(FORMAT,COUNT) \
		case FORMAT:
            DECLARE_FORMATS
#undef DECLARE_FORMAT
            return GL_TRUE;
        default:
            return GL_FALSE;
    }
}

inline static int _jsGetComponentCount( GLenum format )
{
    switch ( format )
    {
#define DECLARE_FORMAT(FORMAT,COUNT) \
		case FORMAT: \
			return COUNT;
            DECLARE_FORMATS
#undef DECLARE_FORMAT
        case GL_INTENSITY:	// only an internal format
            return 1;
        case GL_PALETTE8_RGB8_OES:
        case GL_PALETTE8_RGBA8_OES:
        case GL_PALETTE8_R5_G6_B5_OES:
        case GL_PALETTE8_RGBA4_OES:
        case GL_PALETTE8_RGB5_A1_OES:
            return 1;
        default:
            return 0;
    }
}

int _jsGetMaxBitSize( GLenum type )
{
    switch ( type )
    {

#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
		case TYPE: \
			return MAX(S1,MAX(S2,MAX(S3,S4)));
            DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

#define DECLARE_UNPACKED_TYPE(TYPE) \
		case TYPE: \
			return sizeof(type_##TYPE)*8;
            DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE

        default:
            return 1;
    }
}

int _jsGetTypeSize( GLenum type )
{
    switch ( type )
    {

#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
		case TYPE: \
			return sizeof(type_##REALTYPE);
            DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

#define DECLARE_UNPACKED_TYPE(TYPE) \
		case TYPE: \
			return sizeof(type_##TYPE);
            DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE

        default:
            JS_ASSERT( 0 );
            return 0;
    }
}
int _jsGetPixelSize( GLenum format, GLenum type )
{
    int componentSize;
    switch ( type )
    {

#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
		case TYPE: \
			return sizeof(type_##REALTYPE);
            DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

#define DECLARE_UNPACKED_TYPE(TYPE) \
		case TYPE: \
			componentSize=sizeof(type_##TYPE); \
			break;
            DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE

        default:
            return 0;
    }
    return _jsGetComponentCount( format )*componentSize;
}

GLboolean _jsIsValidPair( GLenum format, GLenum type )
{
    int componentCount = 0;
    switch ( type )
    {
#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
		case TYPE:\
			componentCount=N;\
			break;
            DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

#define DECLARE_UNPACKED_TYPE(TYPE) \
		case TYPE:
            DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE
            return GL_TRUE;
        default:
            JS_ASSERT( 0 );
            return GL_FALSE;
    }
    return ( componentCount == _jsGetComponentCount( format ) );
}

typedef void( GetComponentsFunction_t )( const unsigned char *bytes, GLfloat *values, int count );
typedef void( PutComponentsFunction_t )( unsigned char *bytes, GLfloat *values, int count );
typedef void( ColorConvertFunction_t )( jsColorRGBAf *color, GLfloat *values );

#define DECLARE_UNPACKED_TYPE(TYPE) \
static void _jsGetComponents_##TYPE(const unsigned char *bytes, GLfloat *values, int count) \
{ \
	int i; \
	for (i=0;i<count;++i) \
	{ \
		const type_##TYPE data=*(const type_##TYPE *)bytes; \
		values[i]=_jsFloatFrom_##TYPE(data); \
		bytes+=sizeof(type_##TYPE); \
	} \
}
DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE

#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
static void _jsGetComponents_##TYPE(const unsigned char *bytes, GLfloat *values, int count) \
{ \
	const type_##REALTYPE data=*(const type_##REALTYPE *)bytes; \
	GET_BITS(values[INDEX##REV(N,0)],data,S2+S3+S4,S1); \
	GET_BITS(values[INDEX##REV(N,1)],data,S3+S4,S2); \
	GET_BITS(values[INDEX##REV(N,2)],data,S4,S3); \
	GET_BITS(values[INDEX##REV(N,3)],data,0,S4); \
}
DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

#define DECLARE_UNPACKED_TYPE(TYPE) \
static void _jsPutComponents_##TYPE(unsigned char *bytes, GLfloat *values, int count) \
{ \
	int i; \
	for (i=0;i<count;++i) \
	{ \
		type_##TYPE *data=(type_##TYPE *)bytes; \
		*data=_jsFloatTo_##TYPE(values[i]); \
		bytes+=sizeof(type_##TYPE); \
	} \
}
DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE

#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
static void _jsPutComponents_##TYPE(unsigned char *bytes, GLfloat *values, int count) \
{ \
	type_##REALTYPE *data=(type_##REALTYPE *)bytes; \
	*data=0; \
	PUT_BITS(values[INDEX##REV(N,0)],*data,S2+S3+S4,S1); \
	PUT_BITS(values[INDEX##REV(N,1)],*data,S3+S4,S2); \
	PUT_BITS(values[INDEX##REV(N,2)],*data,S4,S3); \
	PUT_BITS(values[INDEX##REV(N,3)],*data,0,S4); \
}
DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

static inline GetComponentsFunction_t *_jsFindGetComponentsFunction( GLenum type )
{
    switch ( type )
    {

#define DECLARE_UNPACKED_TYPE(TYPE) \
		case TYPE: \
			return &_jsGetComponents_##TYPE;
            DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE

#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
		case TYPE: \
			return &_jsGetComponents_##TYPE;
            DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

        default:
            JS_ASSERT( 0 );
            return NULL;
    }
}

static inline PutComponentsFunction_t *_jsFindPutComponentsFunction( GLenum type )
{
    switch ( type )
    {
#define DECLARE_UNPACKED_TYPE(TYPE) \
		case TYPE: \
			return &_jsPutComponents_##TYPE;
            DECLARE_UNPACKED_TYPES
#undef DECLARE_UNPACKED_TYPE

#define DECLARE_PACKED_TYPE(REALTYPE,TYPE,N,S1,S2,S3,S4,REV) \
		case TYPE: \
			return &_jsPutComponents_##TYPE;
            DECLARE_PACKED_TYPES
#undef DECLARE_PACKED_TYPE

        default:
            JS_ASSERT( 0 );
            return NULL;
    }
}

static void _jsValuesToColor_GL_RGB( jsColorRGBAf *c, GLfloat *v ) { c->R = v[0]; c->G = v[1]; c->B = v[2]; c->A = 1.f; }
static void _jsValuesToColor_GL_BGR( jsColorRGBAf *c, GLfloat *v ) { c->B = v[0]; c->G = v[1]; c->R = v[2]; c->A = 1.f; }
static void _jsValuesToColor_GL_RGBA( jsColorRGBAf *c, GLfloat *v ) { c->R = v[0]; c->G = v[1]; c->B = v[2]; c->A = v[3]; }
static void _jsValuesToColor_GL_BGRA( jsColorRGBAf *c, GLfloat *v ) { c->B = v[0]; c->G = v[1]; c->R = v[2]; c->A = v[3]; }
static void _jsValuesToColor_GL_ABGR( jsColorRGBAf *c, GLfloat *v ) { c->A = v[0]; c->B = v[1]; c->G = v[2]; c->R = v[3]; }
static void _jsValuesToColor_GL_ARGB_SCE( jsColorRGBAf *c, GLfloat *v ) { c->A = v[0]; c->R = v[1]; c->G = v[2]; c->B = v[3]; }
static void _jsValuesToColor_GL_RED( jsColorRGBAf *c, GLfloat *v ) { c->R = v[0]; c->G = 0.f; c->B = 0.f; c->A = 1.f; }
static void _jsValuesToColor_GL_GREEN( jsColorRGBAf *c, GLfloat *v ) { c->R = 0.f; c->G = v[0]; c->B = 0.f; c->A = 1.f; }
static void _jsValuesToColor_GL_BLUE( jsColorRGBAf *c, GLfloat *v ) { c->R = 0.f; c->G = 0.f; c->B = v[0]; c->A = 1.f; }
static void _jsValuesToColor_GL_ALPHA( jsColorRGBAf *c, GLfloat *v ) { c->R = 0.f; c->G = 0.f; c->B = 0.f; c->A = v[0]; }
static void _jsValuesToColor_GL_INTENSITY( jsColorRGBAf *c, GLfloat *v ) { c->R = v[0]; c->G = v[0]; c->B = v[0]; c->A = 1.f; }
static void _jsValuesToColor_GL_LUMINANCE( jsColorRGBAf *c, GLfloat *v ) { c->R = v[0]; c->G = v[0]; c->B = v[0]; c->A = 1.f; }
static void _jsValuesToColor_GL_LUMINANCE_ALPHA( jsColorRGBAf *c, GLfloat *v ) { c->R = v[0]; c->G = v[0]; c->B = v[0]; c->A = v[1]; }
static void _jsValuesToColor_GL_ALPHA_LUMINANCE_SCE( jsColorRGBAf *c, GLfloat *v ) { c->R = v[1]; c->G = v[1]; c->B = v[1]; c->A = v[0]; }
static void _jsValuesToColor_GL_DEPTH_COMPONENT( jsColorRGBAf *c, GLfloat *v ) { c->R = v[0]; c->G = 0.0f; c->B = 0.0f; c->A = 0.0f; }

static void _jsColorToValues_GL_RGB( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->R; v[1] = c->G; v[2] = c->B; }
static void _jsColorToValues_GL_BGR( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->B; v[1] = c->G; v[2] = c->R; }
static void _jsColorToValues_GL_RGBA( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->R; v[1] = c->G; v[2] = c->B; v[3] = c->A; }
static void _jsColorToValues_GL_BGRA( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->B; v[1] = c->G; v[2] = c->R; v[3] = c->A; }
static void _jsColorToValues_GL_ABGR( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->A; v[1] = c->B; v[2] = c->G; v[3] = c->R; }
static void _jsColorToValues_GL_ARGB_SCE( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->A; v[1] = c->R; v[2] = c->G; v[3] = c->B; }
static void _jsColorToValues_GL_RED( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->R; }
static void _jsColorToValues_GL_GREEN( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->G; }
static void _jsColorToValues_GL_BLUE( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->B; }
static void _jsColorToValues_GL_ALPHA( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->A; }
static void _jsColorToValues_GL_INTENSITY( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->R; }
static void _jsColorToValues_GL_LUMINANCE( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->R; }
static void _jsColorToValues_GL_LUMINANCE_ALPHA( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->R; v[1] = c->A; }
static void _jsColorToValues_GL_ALPHA_LUMINANCE_SCE( jsColorRGBAf *c, GLfloat *v ) { v[1] = c->R; v[0] = c->A; }
static void _jsColorToValues_GL_DEPTH_COMPONENT( jsColorRGBAf *c, GLfloat *v ) { v[0] = c->R; v[1] = 0.0f; }

static inline ColorConvertFunction_t *_jsFindValuesToColorFunction( GLenum format )
{
    switch ( format )
    {
#define DECLARE_FORMAT(FORMAT,COUNT) \
		case FORMAT: \
			return &_jsValuesToColor_##FORMAT;
            DECLARE_FORMATS
#undef DECLARE_FORMAT
        case GL_INTENSITY:	// only an internal format
            return &_jsValuesToColor_GL_INTENSITY;
        default:
            JS_ASSERT( 0 );
            return NULL;
    }
}

static inline ColorConvertFunction_t *_jsFindColorToValuesFunction( GLenum format )
{
    switch ( format )
    {
#define DECLARE_FORMAT(FORMAT,COUNT) \
		case FORMAT: \
			return &_jsColorToValues_##FORMAT;
            DECLARE_FORMATS
#undef DECLARE_FORMAT
        case GL_INTENSITY:	// only an internal format
            return &_jsColorToValues_GL_INTENSITY;
        default:
            JS_ASSERT( 0 );
            return NULL;
    }
}


// This function transfers an entire jsRaster into a jsImage at the
// indicated position without performing color conversion.  It is
// assumed that the pixel format and type are identical.
//
// This function is used as a fast path helper for _jsRasterToImage(),
// and to implement transfers of palette data.
void _jsRawRasterToImage(
    const jsRaster* raster,
    jsImage* image,
    GLuint x, GLuint y, GLuint z )
{
    JS_ASSERT( raster->format == image->format ||
               ( raster->format == GL_LUMINANCE && image->format == GL_INTENSITY ) );
    JS_ASSERT( raster->type == image->type );
    JS_ASSERT( image->data );
    const int pixelBits = _jsGetPixelSize( image->format, image->type ) * 8;

    const GLuint size = pixelBits / 8;

    // test for fast path copying
    if ( image->storageType == _JS_IMAGE_STORAGE_RASTER )
    {
        if ( raster->xstride == image->xstride &&
                raster->ystride == image->ystride &&
                raster->zstride == image->zstride )
        {
            // can memcpy entire raster
            JS_ASSERT( x*image->xstride + y*image->ystride + z*image->zstride + raster->depth*raster->zstride <= ( GLuint )image->storageSize );
            fast_memcpy(
                ( char * )image->data +
                x*image->xstride + y*image->ystride + z*image->zstride,
                raster->data,
                raster->depth*raster->zstride );

            // early exit
            return;
        }
        else if ( raster->xstride == image->xstride )
        {
            // can memcpy line by line
            const GLuint lineBytes = raster->width * raster->xstride;
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // set line pointers and copy
                    const char *src = ( const char * )raster->data +
                                      i * raster->zstride + j * raster->ystride;
                    char *dst = ( char * )image->data +
                                ( i + z ) * image->zstride +
                                ( j + y ) * image->ystride +
                                x * image->xstride;
                    JS_ASSERT(( i + z )*image->zstride + ( j + y )*image->ystride + x*image->xstride + lineBytes <= ( GLuint )image->storageSize );
                    fast_memcpy( dst, src, lineBytes );
                }
            }

            // early exit
            return;
        }
    } // possible fast path

    // XXX This function does not currently support fractional byte
    // pixels off the fast path.  This *may* be needed for native 4-bit
    // paletted support.
    JS_ASSERT( pixelBits != 0 && ( pixelBits % 8 ) == 0 );

    switch ( image->storageType )
    {
        case _JS_IMAGE_STORAGE_RASTER:
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // initialize line pointers
                    const char *src = ( const char * )raster->data +
                                      i * raster->zstride + j * raster->ystride;
                    char *dst = ( char * )image->data +
                                ( i + z ) * image->zstride +
                                ( j + y ) * image->ystride +
                                x * image->xstride;

                    for ( int k = 0; k < raster->width; ++k )
                    {
                        // copy a pixel
                        JS_ASSERT(( i + z )*image->zstride + ( j + y )*image->ystride + ( x + k )*image->xstride + size <= ( GLuint )image->storageSize );
                        memcpy( dst, src, size );

                        // update line pointers
                        src += raster->xstride;
                        dst += image->xstride;
                    }
                }
            }
            break;
        case _JS_IMAGE_STORAGE_BLOCK:
            for ( int zz = z; zz - z < ( GLuint )raster->depth; /* null */ )
            {
                // determine sub-block start position
                //  The sub-block start position is usually 0, i.e.
                //  aligned with a block.  When it is not, at an edge of
                //  the region to be transferred, the address is altered
                //  to be block-aligned and the flag for a partial block
                //  is set.
                GLboolean zpartial = GL_FALSE;
                GLuint zdelta0 = zz % image->zblk;
                if ( zdelta0 != 0 )
                {
                    zz -= zdelta0;
                    zpartial = GL_TRUE;
                }

                // determine sub-block end position
                //  The sub-block end position is usually the block
                //  size.  When it is not, at an edge of the region,
                //  the flag for a partial block must be set.
                GLuint zdelta1 = image->zblk;
                if ( zz + zdelta1 > z + raster->depth )
                {
                    zdelta1 = z + raster->depth - zz;
                    zpartial = GL_TRUE;
                }

                int yy = y;
                while ( yy - y < ( GLuint )raster->height )
                {
                    // determine sub-block start position
                    //  See comment for z.
                    GLboolean ypartial = GL_FALSE;
                    GLuint ydelta0 = yy % image->yblk;
                    if ( ydelta0 != 0 )
                    {
                        yy -= ydelta0;
                        ypartial = GL_TRUE;
                    }

                    // determine sub-block end position
                    //  See comment for z.
                    GLuint ydelta1 = image->yblk;
                    if ( yy + ydelta1 > y + raster->height )
                    {
                        ydelta1 = y + raster->height - yy;
                        ypartial = GL_TRUE;
                    }

                    int xx = x;
                    while ( xx - x < ( GLuint )raster->width )
                    {
                        // determine sub-block start position
                        //  See comment for z.
                        GLboolean xpartial = GL_FALSE;
                        GLuint xdelta0 = xx % image->xblk;
                        if ( xdelta0 != 0 )
                        {
                            xx -= xdelta0;
                            xpartial = GL_TRUE;
                        }

                        // determine sub-block end position
                        //  See comment for z.
                        GLuint xdelta1 = image->xblk;
                        if ( xx + xdelta1 > x + raster->width )
                        {
                            xdelta1 = x + raster->width - xx;
                            xpartial = GL_TRUE;
                        }

                        // one block buffer
                        char data[image->xblk*image->yblk*image->zblk*size];

                        // write values to block, with conversion
                        for ( GLuint i = zdelta0; i < zdelta1; ++i )
                        {
                            for ( GLuint j = ydelta0; j < ydelta1; ++j )
                            {
                                // set up line pointers
                                const char *src = ( const char * )raster->data +
                                                  ( zz + i - z ) * raster->zstride +
                                                  ( yy + j - y ) * raster->ystride +
                                                  ( xx + xdelta0 - x ) * raster->xstride;
                                char *dst = data +
                                            (( i * image->yblk + j ) * image->xblk + xdelta0 ) * size;

                                for ( GLuint k = xdelta0; k < xdelta1; ++k )
                                {
                                    // convert a pixel
                                    memcpy( dst, src, size );

                                    // update pointers
                                    src += raster->xstride;
                                    dst += size;
                                }
                            }
                        }

                        xx += image->xblk;
                    }

                    yy += image->yblk;
                }

                zz += image->zblk;
            }
            break;
        default:
            JS_ASSERT( 0 );
    }
}

void _jsFastNVToRGBA(
    const jsRaster* raster,
    jsImage* image,
    GLuint x, GLuint y, GLuint z )
{
    JS_ASSERT(( raster->format == GL_BGRA ) || ( raster->format == GL_BGR ) );
    JS_ASSERT(( raster->type == GL_UNSIGNED_INT_8_8_8_8_REV ) || ( raster->type == GL_UNSIGNED_SHORT_5_6_5_REV ) ||
              ( raster->type == GL_UNSIGNED_SHORT_1_5_5_5_REV ) );
    JS_ASSERT(( image->format == GL_RGBA ) || ( image->format == GL_RGB ) );
    JS_ASSERT(( image->type == GL_UNSIGNED_BYTE ) ||
              ( image->type == GL_UNSIGNED_SHORT_1_5_5_5_REV ) || ( image->type == GL_UNSIGNED_SHORT_5_6_5_REV ) );
    JS_ASSERT( image->storageType == _JS_IMAGE_STORAGE_RASTER );
    JS_ASSERT(( raster->xstride == 4 ) || ( raster->xstride == 2 ) );
    JS_ASSERT(( image->xstride == 4 ) || ( image->xstride == 2 ) );
    JS_ASSERT( image->data );

    switch ( raster->type )
    {
        case GL_UNSIGNED_INT_8_8_8_8_REV:
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // initialize line pointers
                    const unsigned int *src = ( const unsigned int * )(( const char * )raster->data +
                                              i * raster->zstride + j * raster->ystride );
                    unsigned char *dst = ( unsigned char * )image->data +
                                         ( i + z ) * image->zstride + ( j + y ) * image->ystride +
                                         x * image->xstride;

                    for ( int k = 0; k < raster->width; ++k )
                    {
                        unsigned int v = src[k];
                        dst[4*k] = ( v >> 16 ) & 0xff;
                        dst[4*k+1] = ( v >> 8 ) & 0xff;
                        dst[4*k+2] = v & 0xff;
                        dst[4*k+3] = ( v >> 24 ) & 0xff;
                    }
                }
            }
            break;
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // initialize line pointers
                    const unsigned short *src = ( const unsigned short * )(( const char * )raster->data +
                                                i * raster->zstride + j * raster->ystride );
                    unsigned char *dst = ( unsigned char * )image->data +
                                         ( i + z ) * image->zstride + ( j + y ) * image->ystride +
                                         x * image->xstride;

                    for ( int k = 0; k < raster->width; ++k )
                    {
                        unsigned short v = src[k];
                        dst[2*k]  = v >> 8 & 0xff;
                        dst[2*k+1] = v & 0xff;
                    }
                }
            }
            break;
        case GL_UNSIGNED_SHORT_5_6_5_REV:
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // initialize line pointers
                    const unsigned short *src = ( const unsigned short * )(( const char * )raster->data +
                                                i * raster->zstride + j * raster->ystride );
                    unsigned char *dst = ( unsigned char * )image->data +
                                         ( i + z ) * image->zstride + ( j + y ) * image->ystride +
                                         x * image->xstride;

                    for ( int k = 0; k < raster->width; ++k )
                    {
                        unsigned short v = src[k];
                        dst[2*k]  = v >> 8 & 0xff;
                        dst[2*k+1] = v & 0xff;
                    }
                }
            }
            break;
    }
}

void _jsGenericRasterToImage(
    const jsRaster* raster,
    jsImage* image,
    GLuint x, GLuint y, GLuint z )
{
    JS_ASSERT( image->data );
    const int imagePixelSize = _jsGetPixelSize( image->format, image->type );

    // get type-specific parameters and handlers
    const int srcComponents = _jsGetComponentCount( raster->format );
    const int dstComponents = _jsGetComponentCount( image->format );
    GetComponentsFunction_t* getComponents = _jsFindGetComponentsFunction( raster->type );
    PutComponentsFunction_t* putComponents = _jsFindPutComponentsFunction( image->type );
    ColorConvertFunction_t* valuesToColor = _jsFindValuesToColorFunction( raster->format );
    ColorConvertFunction_t* colorToValues = _jsFindColorToValuesFunction( image->format );

    switch ( image->storageType )
    {
        case _JS_IMAGE_STORAGE_RASTER:
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // initialize line pointers
                    const unsigned char *src = ( const unsigned char * )raster->data +
                                               i * raster->zstride + j * raster->ystride;
                    unsigned char *dst = ( unsigned char * )image->data +
                                         ( i + z ) * image->zstride +
                                         ( j + y ) * image->ystride +
                                         x * image->xstride;

                    for ( int k = 0; k < raster->width; ++k )
                    {
                        // convert a pixel
                        GLfloat values[4];
                        jsColorRGBAf color;
                        getComponents( src, values, srcComponents );
                        valuesToColor( &color, values );
                        colorToValues( &color, values );

                        // if converting to any "integer" format, be sure to clamp before converting to destination type
                        if (image->type!=GL_FLOAT && image->type!=GL_HALF_FLOAT_ARB)
                        {
                          values[0]=_jsClampf(values[0]);
                          values[1]=_jsClampf(values[1]);
                          values[2]=_jsClampf(values[2]);
                          values[3]=_jsClampf(values[3]);
                        }

                        putComponents( dst, values, dstComponents );

                        // update line pointers
                        src += raster->xstride;
                        dst += image->xstride;
                    }
                }
            }
            break;
        case _JS_IMAGE_STORAGE_BLOCK:
            for ( int zz = z; zz - z < ( GLuint )raster->depth; /* null */ )
            {
                // determine sub-block start position
                //  The sub-block start position is usually 0, i.e.
                //  aligned with a block.  When it is not, at an edge of
                //  the region to be transferred, the address is altered
                //  to be block-aligned and the flag for a partial block
                //  is set.
                GLboolean zpartial = GL_FALSE;
                GLuint zdelta0 = zz % image->zblk;
                if ( zdelta0 != 0 )
                {
                    zz -= zdelta0;
                    zpartial = GL_TRUE;
                }

                // determine sub-block end position
                //  The sub-block end position is usually the block
                //  size.  When it is not, at an edge of the region,
                //  the flag for a partial block must be set.
                GLuint zdelta1 = image->zblk;
                if ( zz + zdelta1 > z + raster->depth )
                {
                    zdelta1 = z + raster->depth - zz;
                    zpartial = GL_TRUE;
                }

                int yy = y;
                while ( yy - y < ( GLuint )raster->height )
                {
                    // determine sub-block start position
                    //  See comment for z.
                    GLboolean ypartial = GL_FALSE;
                    GLuint ydelta0 = yy % image->yblk;
                    if ( ydelta0 != 0 )
                    {
                        yy -= ydelta0;
                        ypartial = GL_TRUE;
                    }

                    // determine sub-block end position
                    //  See comment for z.
                    GLuint ydelta1 = image->yblk;
                    if ( yy + ydelta1 > y + raster->height )
                    {
                        ydelta1 = y + raster->height - yy;
                        ypartial = GL_TRUE;
                    }

                    int xx = x;
                    while ( xx - x < ( GLuint )raster->width )
                    {
                        // determine sub-block start position
                        //  See comment for z.
                        GLboolean xpartial = GL_FALSE;
                        GLuint xdelta0 = xx % image->xblk;
                        if ( xdelta0 != 0 )
                        {
                            xx -= xdelta0;
                            xpartial = GL_TRUE;
                        }

                        // determine sub-block end position
                        //  See comment for z.
                        GLuint xdelta1 = image->xblk;
                        if ( xx + xdelta1 > x + raster->width )
                        {
                            xdelta1 = x + raster->width - xx;
                            xpartial = GL_TRUE;
                        }

                        // one block buffer
                        unsigned char data[image->xblk*image->yblk*image->zblk*imagePixelSize];

                        // write values to block, with conversion
                        for ( GLuint i = zdelta0; i < zdelta1; ++i )
                        {
                            for ( GLuint j = ydelta0; j < ydelta1; ++j )
                            {
                                // set up line pointers
                                const unsigned char *src = ( const unsigned char * )raster->data +
                                                           ( zz + i - z ) * raster->zstride +
                                                           ( yy + j - y ) * raster->ystride +
                                                           ( xx + xdelta0 - x ) * raster->xstride;
                                unsigned char *dst = data +
                                                     (( i * image->yblk + j ) * image->xblk + xdelta0 ) * imagePixelSize;

                                for ( GLuint k = xdelta0; k < xdelta1; ++k )
                                {
                                    // convert a pixel
                                    GLfloat values[4];
                                    jsColorRGBAf color;
                                    getComponents( src, values, srcComponents );
                                    valuesToColor( &color, values );
                                    colorToValues( &color, values );
                                    putComponents( dst, values, dstComponents );

                                    // update pointers
                                    src += raster->xstride;
                                    dst += imagePixelSize;
                                }
                            }
                        }

                        xx += image->xblk;
                    }

                    yy += image->yblk;
                }

                zz += image->zblk;
            }
            break;
        default:
            JS_ASSERT( 0 );
    }
}

// This function transfers an entire jsRaster into a jsImage at the
// indicated position.
void _jsRasterToImage(
    const jsRaster* raster,
    jsImage* image,
    GLuint x, GLuint y, GLuint z )
{
    _JS_PERFORMANCE_START( RasterToImageCounter );
    JS_ASSERT( raster->width + ( int )x <= image->width );
    JS_ASSERT( raster->height + ( int )y <= image->height );
    JS_ASSERT( raster->depth + ( int )z <= image->depth );
    // test if color conversion can be avoided
    // we skip conversion of GL_LUMINANCE->GL_INTENSITY since they are both single scalar values. 
    if ( ( raster->format == image->format || 
           ( raster->format == GL_LUMINANCE && image->format == GL_INTENSITY ) ) && 
           raster->type == image->type )
    {
        // transfer color data without interpretation
        //  The pixel size argument is in bits.
        //_JS_REPORT_EXTRA(PSGL_REPORT_TEXTURE_CONVERSION,"Transferring texture color data without interpretation.");  // Commented out to fix bug #18327
        _jsRawRasterToImage( raster, image, x, y, z );
        return;
    }
    else if ((( raster->format == GL_BGRA ) || ( raster->format == GL_BGR ) ) &&
             (( raster->type == GL_UNSIGNED_INT_8_8_8_8_REV ) || ( raster->type == GL_UNSIGNED_SHORT_5_6_5_REV ) ||
              ( raster->type == GL_UNSIGNED_SHORT_1_5_5_5_REV ) ) &&
             (( raster->xstride == 4 ) || ( raster->xstride == 2 ) ) &&
             (( image->format == GL_RGBA ) || ( image->format == GL_RGB ) ) &&
             (( image->type == GL_UNSIGNED_BYTE ) ||
              ( image->type == GL_UNSIGNED_SHORT_1_5_5_5_REV ) || ( image->type == GL_UNSIGNED_SHORT_5_6_5_REV ) ) &&
             ( image->storageType == _JS_IMAGE_STORAGE_RASTER ) &&
             (( image->xstride == 4 ) || ( image->xstride == 2 ) ) )
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_CONVERSION, "Fast texture color format conversion for each texel: format %s -> %s, type %s -> %s",
                          _jsGetGLEnumName( raster->format ), _jsGetGLEnumName( image->format ), _jsGetGLEnumName( raster->type ), _jsGetGLEnumName( image->type ) );
        _jsFastNVToRGBA( raster, image, x, y, z );
    }
    else
    {
        _JS_REPORT_EXTRA( PSGL_REPORT_TEXTURE_CONVERSION, "Slow texture color format conversion for each texel: format %s -> %s, type %s -> %s",
                          _jsGetGLEnumName( raster->format ), _jsGetGLEnumName( image->format ), _jsGetGLEnumName( raster->type ), _jsGetGLEnumName( image->type ) );
        _jsGenericRasterToImage( raster, image, x, y, z );
    }
    _JS_PERFORMANCE_STOP( RasterToImageCounter );

}

// This function transfers the pixels from a jsImage at the indicated
// position to fill a jsRaster.
void _jsImageToRaster( const jsImage* image, jsRaster* raster, GLuint x, GLuint y, GLuint z )
{
    _JS_PERFORMANCE_START( ImageToRasterCounter );
    JS_ASSERT( image->data );
    JS_ASSERT( raster->width + ( int )x <= image->width );
    JS_ASSERT( raster->height + ( int )y <= image->height );
    JS_ASSERT( raster->depth + ( int )z <= image->depth );
    // test for fast path copying
    if ( image->storageType == _JS_IMAGE_STORAGE_RASTER &&
            raster->format == image->format && raster->type == image->type )
    {
        if ( raster->xstride == image->xstride &&
                raster->ystride == image->ystride &&
                raster->zstride == image->zstride )
        {
            // can memcpy entire raster
            fast_memcpy(
                raster->data,
                ( const char * )image->data +
                x*image->xstride + y*image->ystride + z*image->zstride,
                raster->depth*raster->zstride );

            // early exit
            return;
        }
        else if ( raster->xstride == image->xstride )
        {
            // can memcpy line by line
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // set line pointers and copy
                    const char *src = ( char * )image->data +
                                      ( i + z ) * image->zstride +
                                      ( j + y ) * image->ystride +
                                      x * image->xstride;
                    char *dst = ( char * )raster->data +
                                i * raster->zstride + j * raster->ystride;
                    fast_memcpy( dst, src, raster->width*raster->xstride );
                }
            }

            // early exit
            return;
        }
    } // possible fast path

    // get type-specific parameters and handlers
    const int srcComponents = _jsGetComponentCount( image->format );
    const int dstComponents = _jsGetComponentCount( raster->format );
    GetComponentsFunction_t* getComponents = _jsFindGetComponentsFunction( image->type );
    PutComponentsFunction_t* putComponents = _jsFindPutComponentsFunction( raster->type );
    ColorConvertFunction_t* valuesToColor = _jsFindValuesToColorFunction( image->format );
    ColorConvertFunction_t* colorToValues = _jsFindColorToValuesFunction( raster->format );
    const int imagePixelSize = _jsGetPixelSize( image->format, image->type );

    switch ( image->storageType )
    {
        case _JS_IMAGE_STORAGE_RASTER:
            for ( int i = 0; i < raster->depth; ++i )
            {
                for ( int j = 0; j < raster->height; ++j )
                {
                    // initialize line pointers
                    const unsigned char *src = ( const unsigned char * )image->data +
                                               ( i + z ) * image->zstride +
                                               ( j + y ) * image->ystride +
                                               x * image->xstride;
                    unsigned char *dst = ( unsigned char * )raster->data +
                                         i * raster->zstride + j * raster->ystride;

                    for ( int k = 0; k < raster->width; ++k )
                    {
                        // convert a pixel
                        GLfloat values[4];
                        jsColorRGBAf color;
                        getComponents( src, values, srcComponents );
                        valuesToColor( &color, values );
                        colorToValues( &color, values );
                        putComponents( dst, values, dstComponents );

                        // update line pointers
                        src += image->xstride;
                        dst += raster->xstride;
                    }
                }
            }
            break;
        case _JS_IMAGE_STORAGE_BLOCK:
            for ( int zz = z; zz - z < ( GLuint )raster->depth; /* null */ )
            {
                // determine sub-block start position
                //  The sub-block start position is usually 0, i.e.
                //  aligned with a block.  When it is not, at an edge of
                //  the region to be transferred, the address is altered
                //  to be block-aligned and the flag for a partial block
                //  is set.
                GLuint zdelta0 = zz % image->zblk;
                if ( zdelta0 != 0 )
                    zz -= zdelta0;

                // determine sub-block end position
                //  The sub-block end position is usually the block
                //  size.  When it is not, at an edge of the region,
                //  the flag for a partial block must be set.
                GLuint zdelta1 = image->zblk;
                if ( zz + zdelta1 > z + raster->depth )
                    zdelta1 = z + raster->depth - zz;

                int yy = y;
                while ( yy - y < ( GLuint )raster->height )
                {
                    // determine sub-block start position
                    //  See comment for z.
                    GLuint ydelta0 = yy % image->yblk;
                    if ( ydelta0 != 0 )
                        yy -= ydelta0;

                    // determine sub-block end position
                    //  See comment for z.
                    GLuint ydelta1 = image->yblk;
                    if ( yy + ydelta1 > y + raster->height )
                        ydelta1 = y + raster->height - yy;

                    int xx = x;
                    while ( xx - x < ( GLuint )raster->width )
                    {
                        // determine sub-block start position
                        //  See comment for z.
                        GLuint xdelta0 = xx % image->xblk;
                        if ( xdelta0 != 0 )
                            xx -= xdelta0;

                        // determine sub-block end position
                        //  See comment for z.
                        GLuint xdelta1 = image->xblk;
                        if ( xx + xdelta1 > x + raster->width )
                            xdelta1 = x + raster->width - xx;

                        // one block buffer
                        unsigned char data[image->xblk*image->yblk*image->zblk*imagePixelSize];

                        // write values to block, with conversion
                        for ( GLuint i = zdelta0; i < zdelta1; ++i )
                        {
                            for ( GLuint j = ydelta0; j < ydelta1; ++j )
                            {
                                // set up line pointers
                                const unsigned char *src = data +
                                                           (( i * image->yblk + j ) * image->xblk + xdelta0 ) * imagePixelSize;
                                unsigned char *dst = ( unsigned char * )raster->data +
                                                     ( zz + i - z ) * raster->zstride +
                                                     ( yy + j - y ) * raster->ystride +
                                                     ( xx + xdelta0 - x ) * raster->xstride;

                                for ( GLuint k = xdelta0; k < xdelta1; ++k )
                                {
                                    // convert a pixel
                                    GLfloat values[4];
                                    jsColorRGBAf color;
                                    getComponents( src, values, srcComponents );
                                    valuesToColor( &color, values );
                                    colorToValues( &color, values );
                                    putComponents( dst, values, dstComponents );

                                    // update pointers
                                    src += imagePixelSize;
                                    dst += raster->xstride;
                                }
                            }
                        }

                        xx += image->xblk;
                    }

                    yy += image->yblk;
                }

                zz += image->zblk;
            }
            break;
        default:
            JS_ASSERT( 0 );
    }
    _JS_PERFORMANCE_STOP( ImageToRasterCounter );
}

void _jsImageAllocCPUStorage( jsImage *image )
{
    if (( image->storageSize > image->mallocStorageSize ) || ( !image->mallocData ) )
    {
        // allocate storage (free previous)
        if ( image->mallocData ) jsFree( image->mallocData );
        image->mallocData = ( char * )jsMalloc( image->storageSize + 128 );
#ifndef JS_NO_ERROR_CHECK
        if ( image->mallocData == NULL )
        {
            image->data = NULL;
            _jsSetError( GL_OUT_OF_MEMORY );
            return;
        }
#endif
        image->mallocStorageSize = image->storageSize;
    }
    JS_ASSERT( image->mallocData );
    image->data = _jsPadPtr( image->mallocData, 128 );
}

void _jsImageFreeCPUStorage( jsImage *image )
{
    if ( !image->mallocData ) return;
    jsFree( image->mallocData );
    image->mallocStorageSize = 0;
    image->data = NULL;
    image->mallocData = NULL;
    image->dataState &= ~_JS_IMAGE_DATASTATE_HOST;
}

void _jsSetImage( jsImage *image, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei alignment, GLenum format, GLenum type, const GLvoid *pixels )
{
    // initialize image instance
    image->width = width;
    image->height = height;
    image->depth = depth;
    image->alignment = alignment;

    image->storageType = _JS_IMAGE_STORAGE_RASTER;
    image->xblk = 0;
    image->yblk = 0;
    image->zblk = 0;

    image->xstride = 0;
    image->ystride = 0;
    image->zstride = 0;

    // choose storage format
    image->format = 0;
    image->type = 0;
    image->internalFormat = 0;
    const GLenum status = _jsPlatformChooseInternalStorage( image, internalFormat );
#ifndef JS_NO_ERROR_CHECK
    if ( status != GL_NO_ERROR )
    {
        _jsSetError( status );
        return;
    }
#else
    (( void )status );		// unused variable is ok
#endif

    image->data = NULL;
    image->mallocData = NULL;
    image->mallocStorageSize = 0;

    image->isSet = GL_TRUE;

    // compute default strides if necessary
    if ( image->storageType == _JS_IMAGE_STORAGE_RASTER )
    {
        if ( image->xstride == 0 )
            image->xstride = _jsGetPixelSize( image->format, image->type );
        if ( image->ystride == 0 )
            image->ystride = image->width * image->xstride;
        if ( image->zstride == 0 )
            image->zstride = image->height * image->ystride;
    }

    if ( pixels )
    {
        _jsImageAllocCPUStorage( image );
        if ( !image->data )
            return;

        // create raster instance
        jsRaster raster;
        raster.format = format;
        raster.type = type;
        raster.width = width;
        raster.height = height;
        raster.depth = depth;
        raster.data = ( void * )pixels;

        // set raster strides
        raster.xstride = _jsGetPixelSize( raster.format, raster.type );
        raster.ystride = ( raster.width * raster.xstride + alignment - 1 ) / alignment * alignment;
        raster.zstride = raster.height * raster.ystride;

        // apply conversion
        _jsRasterToImage( &raster, image, 0, 0, 0 );
        image->dataState = _JS_IMAGE_DATASTATE_HOST;
    }
    else
        image->dataState = _JS_IMAGE_DATASTATE_UNSET;
}

void _jsSetSubImage( GLenum target, GLint level, jsTexture *texture, jsImage *image, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, GLsizei alignment, GLenum format, GLenum type, const GLvoid *pixels )
{
#ifndef JS_NO_ERROR_CHECK
    if (( x < 0 ) || ( x + width > image->width ) ||
            ( y < 0 ) || ( y + height > image->height ) ||
            ( z < 0 ) || ( z + depth > image->depth ) )
    {
        _jsSetError( GL_INVALID_VALUE );
        return;
    }
#endif

    if ( image->dataState == _JS_IMAGE_DATASTATE_GPU )
    {
        _jsPlatformGetImageData( target, level, texture, image );
    }
    else if ( image->dataState == _JS_IMAGE_DATASTATE_UNSET )
    {
        _jsImageAllocCPUStorage( image );
        /*        memset(image->data,0,image->storageSize);*/
    }

#ifndef JS_NO_ERROR_CHECK
    if ( !image->data )
    {
        _jsSetError( GL_INVALID_OPERATION );
        return;
    }
#endif

    // create raster instance
    jsRaster raster;
    raster.format = format;
    raster.type = type;
    raster.width = width;
    raster.height = height;
    raster.depth = depth;
    raster.data = ( void * )pixels;

    // set raster strides
    raster.xstride = _jsGetPixelSize( raster.format, raster.type );
    raster.ystride = ( raster.width * raster.xstride + alignment - 1 ) / alignment * alignment;
    raster.zstride = raster.height * raster.ystride;

    // apply conversion
    _jsRasterToImage( &raster, image, x, y, z );
    image->dataState = _JS_IMAGE_DATASTATE_HOST;
}

void _jsResampleImage3D( jsImage* src, jsImage* dst )
{
    JS_ASSERT( src != NULL );
    JS_ASSERT( src->width > 0 && src->height > 0 && src->depth > 0 );
    JS_ASSERT( dst->width > 0 && dst->height > 0 && dst->depth > 0 );
    JS_ASSERT( src->format == dst->format );

    // get type-specific parameters and handlers
    const int srcComponents = _jsGetComponentCount( src->format );
    const int dstComponents = _jsGetComponentCount( dst->format );
    JS_ASSERT( srcComponents == dstComponents );
    GetComponentsFunction_t* getComponents = _jsFindGetComponentsFunction( src->type );
    PutComponentsFunction_t* putComponents = _jsFindPutComponentsFunction( dst->type );

    // iterate over destination samples
    //  We first map from destination sample space (dstx,dsty,dstz), which
    //  is 0 to N-1 in each dimension, into a normalized [0,1] space
    //  (x,y,z).  Then we map from the normalized space into the source
    //  sample space (srcx,srcy,srcz).  We sample the source image at that
    //  location and write it to the destination.
    for ( int dstz = 0; dstz < dst->depth; ++dstz )
    {
        const float z = ( 2.0f * dstz + 1.0f ) / ( 2.0f * dst->depth );
        const float srcz = src->depth * z - 0.5f;

        // compute sample offsets
        const int index = ( int )floorf( srcz );
        int zoff[2];
        zoff[0] = ( index > 0 ) ? index * src->zstride : 0;
        zoff[1] = ( index + 1 ) < src->depth ? ( index + 1 ) * src->zstride : zoff[0];

        // compute sample weights
        float zweight[2];
        zweight[0] = srcz - floorf( srcz );
        zweight[1] = 1.0f - zweight[0];

        for ( int dsty = 0; dsty < dst->height; ++dsty )
        {
            const float y = ( 2.0f * dsty + 1.0f ) / ( 2.0f * dst->height );
            const float srcy = src->height * y - 0.5f;

            // compute sample offsets
            const int index = ( int )floorf( srcy );
            int yoff[2];
            yoff[0] = ( index > 0 ) ? index * src->ystride : 0;
            yoff[1] = ( index + 1 ) < src->height ? ( index + 1 ) * src->ystride : yoff[0];

            // compute sample weights
            float yweight[2];
            yweight[0] = srcy - floorf( srcy );
            yweight[1] = 1.0f - yweight[0];

            for ( int dstx = 0; dstx < dst->width; ++dstx )
            {
                const float x = ( 2.0f * dstx + 1.0f ) / ( 2.0f * dst->width );
                const float srcx = src->width * x - 0.5f;

                // compute sample offsets
                const int index = ( int )floorf( srcx );
                int xoff[2];
                xoff[0] = ( index > 0 ) ? index * src->xstride : 0;
                xoff[1] = ( index + 1 ) < src->width ? ( index + 1 ) * src->xstride : xoff[0];

                // compute sample weights
                float xweight[2];
                xweight[0] = srcx - floorf( srcx );
                xweight[1] = 1.0f - xweight[0];

                // accumulate weighted average
                GLfloat values[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                for ( int i = 0; i < 2; ++i )
                {
                    for ( int j = 0; j < 2; ++j )
                    {
                        for ( int k = 0; k < 2; ++k )
                        {
                            GLfloat v[4];
                            getComponents(
                                ( const unsigned char * )src->data + zoff[i] + yoff[j] + xoff[k],
                                v,
                                srcComponents );
                            const GLfloat w = zweight[i] * yweight[j] * xweight[k];
                            values[0] += w * v[0];
                            values[1] += w * v[1];
                            values[2] += w * v[2];
                            values[3] += w * v[3];
                        }
                    }
                } // sample neighborhood loop

                putComponents(
                    ( unsigned char * )dst->data + dstz*dst->zstride + dsty*dst->ystride + dstx*dst->xstride,
                    values,
                    dstComponents );
            } // dstx
        } // dsty
    } // dstz
}
