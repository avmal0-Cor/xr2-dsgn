/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Jetstream graphics API matrix operations
 */

#ifndef _JSMatrix_h
#define _JSMatrix_h

#ifndef _JSTypes_h
#include <PSGL/Types.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif


//----------------------------------------------------------------
// Global variables
//----------------------------------------------------------------

// Suffixes for input and output arguments
//
// I : implicit (mX, mY, mZ declared and set by the caller)
// V : values   (InX, InY, InZ)
// S : struct  (In.X, In.Y, In.Z)
// P : struct pointer  (In->X, In->Y, In->Z)
//
// If there is only one suffix, the input and output will be the same variable
//



//================================================================
// Transform [X, Y, Z] with a Matrix's top-left 3x3 part
//================================================================
#define _jsMatrix3x3TransformVf(mMatrix,  mXIO, mYIO, mZIO)\
{\
 GLfloat mX=mXIO, mY=mYIO, mZ=mZIO;\
 mXIO = mX*(mMatrix)[M00]+mY*(mMatrix)[M01]+mZ*(mMatrix)[M02];\
 mYIO = mX*(mMatrix)[M10]+mY*(mMatrix)[M11]+mZ*(mMatrix)[M12];\
 mZIO = mX*(mMatrix)[M20]+mY*(mMatrix)[M21]+mZ*(mMatrix)[M22];\
}

//================================================================
// Transform [X, Y, Z] with a Matrix's top-left 3x3 part
//================================================================
#define _jsMatrix3x3TransformVVf(mMatrix,  mXIn, mYIn, mZIn,  mXOut, mYOut, mZOut)\
{\
 GLfloat mX=mXIn, mY=mYIn, mZ=mZIn;\
 mXOut = mX*(mMatrix)[M00]+mY*(mMatrix)[M01]+mZ*(mMatrix)[M02];\
 mYOut = mX*(mMatrix)[M10]+mY*(mMatrix)[M11]+mZ*(mMatrix)[M12];\
 mZOut = mX*(mMatrix)[M20]+mY*(mMatrix)[M21]+mZ*(mMatrix)[M22];\
}

//========================================================================
// Transform [X, Y, Z] with a Matrix's left 3x4 part
//========================================================================
#define _jsMatrix3x3TransformSSf(mMatrix,  mIn,  mOut)\
{\
 GLfloat mX=mIn.X, mY=mIn.Y, mZ=mIn.Z;\
 mOut.X = mX*(mMatrix)[M00]+mY*(mMatrix)[M01]+mZ*(mMatrix)[M02];\
 mOut.Y = mX*(mMatrix)[M10]+mY*(mMatrix)[M11]+mZ*(mMatrix)[M12];\
 mOut.Z = mX*(mMatrix)[M20]+mY*(mMatrix)[M21]+mZ*(mMatrix)[M22];\
}


//========================================================================
// Transform [X, Y, Z] with a Matrix's left 3x4 part
//========================================================================
#define _jsMatrix3x4TransformVVf(mMatrix,  mXIn, mYIn, mZIn,  mXOut, mYOut, mZOut)\
{\
 GLfloat mX=mXIn, mY=mYIn, mZ=mZIn;\
 mXOut = mX*(mMatrix)[M00]+mY*(mMatrix)[M01]+mZ*(mMatrix)[M02]+(mMatrix)[M03];\
 mYOut = mX*(mMatrix)[M10]+mY*(mMatrix)[M11]+mZ*(mMatrix)[M12]+(mMatrix)[M13];\
 mZOut = mX*(mMatrix)[M20]+mY*(mMatrix)[M21]+mZ*(mMatrix)[M22]+(mMatrix)[M23];\
}

//========================================================================
// Transform [X, Y, Z] with a Matrix's left 3x4 part
//========================================================================
#define _jsMatrix3x4TransformSSf(mMatrix,  mIn,  mOut)\
{\
 GLfloat mX=mIn.X, mY=mIn.Y, mZ=mIn.Z;\
 mOut.X = mX*(mMatrix)[M00]+mY*(mMatrix)[M01]+mZ*(mMatrix)[M02]+(mMatrix)[M03];\
 mOut.Y = mX*(mMatrix)[M10]+mY*(mMatrix)[M11]+mZ*(mMatrix)[M12]+(mMatrix)[M13];\
 mOut.Z = mX*(mMatrix)[M20]+mY*(mMatrix)[M21]+mZ*(mMatrix)[M22]+(mMatrix)[M23];\
}


//========================================================================
// Transform a homogenous 3D position [X, Y, Z, W] with a 4x4 Matrix
//========================================================================
#define _jsMatrix4x4TransformHVVf(mMatrix,  mXIn, mYIn, mZIn, mWIn, mXOut,  mYOut, mZOut, mWOut)\
{\
 GLfloat mX=mXIn, mY=mYIn, mZ=mZIn, mW=mWIn;\
\
 mXOut = mX*mMatrix[M00]+mY*mMatrix[M01]+mZ*mMatrix[M02]+mW*mMatrix[M03];\
 mYOut = mX*mMatrix[M10]+mY*mMatrix[M11]+mZ*mMatrix[M12]+mW*mMatrix[M13];\
 mZOut = mX*mMatrix[M20]+mY*mMatrix[M21]+mZ*mMatrix[M22]+mW*mMatrix[M23];\
 mWOut = mX*mMatrix[M30]+mY*mMatrix[M31]+mZ*mMatrix[M32]+mW*mMatrix[M33];\
}


#define _jsMatrix4x4TransformSSf(mMatrix,  mIn,  mOut)\
{\
 GLfloat mX=mIn.X, mY=mIn.Y, mZ=mIn.Z, mW=mIn.W;\
 mOut.X = mX*(mMatrix)[M00]+mY*(mMatrix)[M01]+mZ*(mMatrix)[M02]+mW*(mMatrix)[M03];\
 mOut.Y = mX*(mMatrix)[M10]+mY*(mMatrix)[M11]+mZ*(mMatrix)[M12]+mW*(mMatrix)[M13];\
 mOut.Z = mX*(mMatrix)[M20]+mY*(mMatrix)[M21]+mZ*(mMatrix)[M22]+mW*(mMatrix)[M23];\
 mOut.W = mX*(mMatrix)[M30]+mY*(mMatrix)[M31]+mZ*(mMatrix)[M32]+mW*(mMatrix)[M33];\
}

    extern PSGL_EXPORT const GLfloat _jsIdentityMatrixf[jsELEMENTS_IN_MATRIX];

    void _jsMatrixStackReset( jsMatrixStack* LMatrixStack );
    extern void _jsMatrixStackInit( jsMatrixStack* LMatrixStack, GLuint depth );
    extern void _jsMatrixStackClear( jsMatrixStack* LMatrixStack );
    void _jsMatrixMult4f( const GLfloat *LMatrix1, const GLfloat *LMatrix2, GLfloat *LMatrix );


#define jsMATRIX_SIZEf  (sizeof(GLfloat)*jsELEMENTS_IN_MATRIX)


// Matrix addressing
//

#define M00 0 // This one is separate, so we can do an '#ifdef M00'...
    enum
    {
        M10 = 1, M20, M30,
        M01, M11, M21, M31,
        M02, M12, M22, M32,
        M03, M13, M23, M33
    };


#ifdef __cplusplus
} // Close scope of 'extern "C"' declaration which encloses file.
#endif

#endif // _JSMatrix_h
