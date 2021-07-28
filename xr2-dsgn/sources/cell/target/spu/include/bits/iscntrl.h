/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2005 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/
_FUNC_DEF( int , iscntrl, (int x))
{
return(x==127 || (x>=0   &&  x<32));
}
