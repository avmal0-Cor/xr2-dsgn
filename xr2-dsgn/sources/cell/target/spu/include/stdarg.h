/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2004 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _STDARG_H_
#define _STDARG_H_

/* internally known to gcc */
typedef __builtin_va_list	va_list ;
#define va_start(ap,last) 		__builtin_va_start((ap),(last))
#define va_arg				__builtin_va_arg
#define va_copy				__builtin_va_copy
#define va_end(ap)			__builtin_va_end(ap)

#endif /* _STDARG_H_  */
