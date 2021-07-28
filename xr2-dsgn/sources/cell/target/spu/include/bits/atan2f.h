/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(float, atan2f, (float y, float x))
{
    return spu_extract(atan2f4(spu_promote(y,0),spu_promote(x,0)),0); 
}
