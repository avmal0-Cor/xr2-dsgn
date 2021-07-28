/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( int , isalnum, (int x))
{
return((x>47 &&  x<58) || (x>64 && x<91) || (x>96 && x<123));
}
