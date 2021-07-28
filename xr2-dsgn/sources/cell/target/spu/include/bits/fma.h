/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(
      double,
      fma,
      ( double x, double y, double z ))
{
   return spu_extract( spu_madd( spu_promote( x, 0 ), spu_promote( y, 0),
			      spu_promote( z, 0 )), 0 );
}

