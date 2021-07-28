/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
#ifndef __ATAN_F_H__
#define __ATAN_F_H__

_FUNC_DEF(float, atanf, (float x))
{
    return spu_extract(atanf4(spu_promote(x,0)),0); 
}

#endif /* __ATAN_F_H__ */
