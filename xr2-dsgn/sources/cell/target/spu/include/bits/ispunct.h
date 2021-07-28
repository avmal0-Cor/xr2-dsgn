/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( int , ispunct, (int x))
{
return((x>32 &&  x<48) || (x>57 && x<65) || (x>90 && x<97) || (x>122 && x<127));
}
