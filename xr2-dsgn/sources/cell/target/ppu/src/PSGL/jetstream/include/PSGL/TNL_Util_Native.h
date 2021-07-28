/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// DESCRIPTION:	Fixed function Pipeline TNL Functions

#ifndef TNL_Util_NATIVE_H
#define TNL_Util_NATIVE_H

typedef struct
{
    GLfloat	x, y, z, w;
}
vec4;

typedef struct
{
    GLfloat x, y, z;
}
vec3;

static inline GLfloat NA_slowSqrt( GLfloat f )
{
    return sqrt( f );
}

static inline GLfloat NA_Dot3( vec3 * v1, vec3 * v2 )
{
    return (( v1->x*v2->x ) + ( v1->y*v2->y ) + ( v1->z*v2->z ) );
}

static inline void NA_Normalize( vec3 * v )
{
    // get the magnitude
    float len = NA_slowSqrt( NA_Dot3( v, v ) );

    if ( len == 0.f ) return;

    float div = 1 / len;

    v->x *= div;
    v->y *= div;
    v->z *= div;

}

// Very fast integer power function
// THANKS ROBIN!

static inline GLfloat NA_Powi( GLfloat x, GLint N )
{

    GLfloat y, w;

    if ( N&1 )
        y = x;
    else
    {
        y = 1.0f;
    }

    w = x;
    N >>= 1;

    while ( N )
    {
        w = w * w;
        if ( N&1 )
            y *= w;
        N >>= 1;
    }
    return y;


    /*	w = x;
    	for ( int i = 0; i < N; i ++)
    	{
    		w *= x;
    	}

    	return w; */
}

#endif
