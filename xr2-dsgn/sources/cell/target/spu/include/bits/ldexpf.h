/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(float, ldexpf, (float x, int exp))
{
  return spu_extract(ldexpf4(spu_promote((x), 0), spu_promote((exp),0)), 0);
}
