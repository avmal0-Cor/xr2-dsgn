/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/*                 -*- mode: c; tab-width: 4; indent-tabs-mode: nil; -*- */
/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *
 *      Copyright (C) 2008 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 */
/*
 * $Id: exportcplusplus.h 30522 2005-12-15 04:15:55Z tei $
 *
 * module - exportcplusplus.h
 *   pickup export symbol macros
 *
 *      Date            Design      Log
 *  ----------------------------------------------------
 *      2005-11-26      isii        initial implementation
 *      2008-10-16      isii        C support & ELF visibility support
 */

#ifndef _SYS_EXPORTCPLUSPLUS_H
#define _SYS_EXPORTCPLUSPLUS_H

#if ! defined(__SYS_LIB_DECLARE_BY_ASM) && ! defined(__SYS_LIB_DECLARE_BY_C)
#if defined(__GNUC__)
#define __SYS_LIB_DECLARE_BY_ASM 1
#endif
#endif

#if defined(__GNUC__)
#define __SYS_MODULE_LINK_TRIGGER_BY_ASM 1
#endif

#define __SYS_LIB_EP_STRING0(a) # a
#define __SYS_LIB_EP_STRING(a) __SYS_LIB_EP_STRING0(a)

#if defined(__SYS_LIB_DECLARE_BY_ASM)
#define __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, type, openlevel, flag, pattern) \
__asm__(                                                                 \
".pushsection   .psp_lib_markpickup_" #libname ",\"aw\",@progbits\n"     \
"        .align 0\n"                                                     \
"        .asciz \"" __FILE__ ":" __SYS_LIB_EP_STRING(__LINE__) ";"       \
        # type "(" # openlevel "," # flag ");" pattern "\"\n"            \
"        .popsection\n")
#else
#define __SYS_LIB_EP_CONCAT0(a,b,c,d) a ## b ## c ## d
#define __SYS_LIB_EP_CONCAT(a,b,c,d) __SYS_LIB_EP_CONCAT0(a,b,c,d)
#define __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, type, openlevel, flag, pattern) \
    static char __SYS_LIB_EP_CONCAT(__PSPPICK_,libname,_ ,__LINE__) []       \
    __attribute__((aligned(1),section(".psp_lib_markpickup_" #libname),unused))\
    = __FILE__ ":" __SYS_LIB_EP_STRING(__LINE__) ";" \
        # type "(" # openlevel "," # flag ");" pattern
#endif

#define SYS_LIB_EXPORTPICKUP_CPP_FUNC( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPF, 0, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_FUNC_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPF, opnlv, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_FUNC_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPF, 0, 2, pattern)

#define SYS_LIB_EXPORTPICKUP_CPP_LOCAL_FUNC( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPF, 0, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_LOCAL_FUNC_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPF, opnlv, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_LOCAL_FUNC_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPF, 0, 3, pattern)

#define SYS_LIB_EXPORTPICKUP_CPP_VAR( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPV, 0, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_VAR_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPV, opnlv, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_VAR_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPV, 0, 2, pattern)

#define SYS_LIB_EXPORTPICKUP_CPP_LOCAL_VAR( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPV, 0, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_LOCAL_VAR_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPV, opnlv, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_CPP_LOCAL_VAR_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CPV, 0, 3, pattern)

#define SYS_LIB_EXPORTPICKUP_CLASS( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CLS, 0, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_CLASS_IGNORE( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CLS, 0, 2, pattern)

#define SYS_LIB_EXPORTPICKUP_NAMESPACE( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, NSP, 0, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_NAMESPACE_IGNORE( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, NSP, 0, 2, pattern)

#define SYS_LIB_EXPORTPICKUP_ELF_VISIBILITY_DEFAULT( libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, ELF, 0, 0, "STV_DEFAULT")
#define SYS_LIB_EXPORTPICKUP_ELF_VISIBILITY_LOCAL_DEFAULT( libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, ELF, 0, 1, "STV_DEFAULT")

#define SYS_LIB_EXPORTPICKUP_ELF_VISIBILITY_HIDDEN_IGNORE( libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, ELF, 0, 2, "STV_HIDDEN")
#define SYS_LIB_EXPORTPICKUP_ELF_VISIBILITY_LOCAL_HIDDEN_IGNORE( libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, ELF, 0, 3, "STV_HIDDEN")

#define SYS_LIB_EXPORTPICKUP_C_FUNC( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CFN, 0, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_C_FUNC_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CFN, opnlv, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_C_FUNC_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CFN, 0, 2, pattern)

#define SYS_LIB_EXPORTPICKUP_C_LOCAL_FUNC( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CFN, 0, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_C_LOCAL_FUNC_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CFN, opnlv, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_C_LOCAL_FUNC_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CFN, 0, 3, pattern)

#define SYS_LIB_EXPORTPICKUP_C_VAR( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CVR, 0, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_C_VAR_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CVR, opnlv, 0, pattern)
#define SYS_LIB_EXPORTPICKUP_C_VAR_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CVR, 0, 2, pattern)

#define SYS_LIB_EXPORTPICKUP_C_LOCAL_VAR( pattern, libname ) \
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CVR, 0, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_C_LOCAL_VAR_WITH_OPENLEVEL( pattern, libname, opnlv)\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CVR, opnlv, 1, pattern)
#define SYS_LIB_EXPORTPICKUP_C_LOCAL_VAR_IGNORE( pattern, libname )\
    __SYS_LIB_EXPORTPICKUP_BASE_MACRO(libname, CVR, 0, 3, pattern)

#endif /* _SYS_EXPORTCPLUSPLUS_H */
