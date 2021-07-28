/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#undef _FUNC_DEF
#ifdef __GNUC__
#define _FUNC_DEF(retval,name,arg) \
retval name arg __attribute__ ((__weak__, __alias__("_" #name))); \
retval _##name arg 
#else
#error "Require GCC to build because we use attributes."
#endif
