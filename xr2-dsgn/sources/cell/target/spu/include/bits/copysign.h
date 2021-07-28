/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(
      double,
      copysign,
      ( double x, double y ))
{
   return spu_extract( copysignd2( spu_promote(x, 0), spu_promote(y, 0) ), 0 );
}

