/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF(
      float,
      ceilf,
      ( float x ))
{
   return spu_extract( ceilf4( spu_promote( x, 0 ) ), 0 );
}

