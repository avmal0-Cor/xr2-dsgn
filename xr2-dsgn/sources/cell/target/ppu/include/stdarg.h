/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef _STDARG_H_
#define _STDARG_H_

#ifdef __SNC__
typedef char *   va_list;

  #ifdef  __cplusplus
  extern  "C"
  {
  #endif
    extern char  *__va_funcx(int, va_list, ...) ; /* expanded by compiler */
  #ifdef __cplusplus
  }
  #endif

  #define va_start(ap,N)  ((ap) = __va_funcx (2, (char *) __builtin_addrof((N))))
  #define va_end(ap)      ((void) 0)
  #define va_arg(ap,T)    (*((T *) __va_funcx (3, (ap),((T *) 0))))

#else  /* __SNC__ */

/* internally known to gcc */
  typedef __builtin_va_list	va_list;
  #define va_start(ap,last) 	__builtin_va_start((ap),(last))
  #define va_arg		__builtin_va_arg
  #define va_copy		__builtin_va_copy
  #define va_end(ap)		__builtin_va_end(ap)

#endif	/* __SNC__ */

#endif /* _STDARG_H_  */
