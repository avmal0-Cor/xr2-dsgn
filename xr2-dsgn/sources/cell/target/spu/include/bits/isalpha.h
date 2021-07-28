/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( int , isalpha, (int x))
{
return((x>64 &&  x<91) || (x>96 && x<123));
}
