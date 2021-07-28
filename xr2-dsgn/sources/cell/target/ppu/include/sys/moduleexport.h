/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
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
 * $Id: moduleexport.h 33871 2006-05-30 18:46:04Z tei $
 *
 * module - moduleexport.h
 *   module export macros
 *
 *      Date            Design      Log
 *  -------------------------------------------------------------------------
 *      2004-05-10      isii        initial implementation
 *      2004-09-22      isii        change '__asm__' to variable declare
 *      2004-10-14      isii        add variable export
 *      2004-11-03      isii        separate GCC and other
 *      2004-12-22      isii        refine __asm__ and defined(__GNUC__)
 *      2005-01-10      kono        suppress warning
 *      2005-07-22      isii        add nidname for calculate NID
 *                                  SCE_LIB_EXPORT_OTHER_NID(), etc
 *                                  SCE_LIB_EXPORT_VAR_OTHER_NID(), etc
 *                                  refine __asm__ and defined(__GNUC__)
 *      2005-08-12      isii        add SCE_LIB_DECLARE_WITH_STUB_NSUFFIX()
 *                                  add SCE_LIB_DECLARE_WITH_NID_SUFFIX()
 *      2006-02-13      isii        for PS3/PPU
 *      2006-05-12      isii        macro prefix 'SCE' change to 'SYS'
 *      2007-11-28      ten         add SYS_MODULE_EXIT
 */

#ifndef _SYS_MODULEEXPORT_H
#define _SYS_MODULEEXPORT_H

#define SYS_MODULE_NAME_LEN             27
#define SYS_MODULE_MAX_SEGMENTS         4

#define SYS_MODINFO_SECTION_NAME        ".rodata.sceModuleInfo"
#define SYS_RESIDENT_SECTION_NAME       ".rodata.sceResident"
#define SYS_FUNCTION_STUB_SECTION_NAME  ".sceStub.text"
#define SYS_VARIABLE_STUB_SECTION_NAME  ".rodata.sceVstub"

/* ------------ basic macros ------------ */
#define SYS_LIB_AUTO_EXPORT    (0x0001)
#define SYS_LIB_WEAK_EXPORT    (0x0002)
#define SYS_LIB_NOLINK_EXPORT  (0x0004)
#define SYS_LIB_WEAK_IMPORT    (0x0008)

#if ! defined(__SCE_LIB_DECLARE_BY_ASM) && ! defined(__SCE_LIB_DECLARE_BY_C)
#if defined(__GNUC__)
#define __SCE_LIB_DECLARE_BY_ASM 1
#endif
#endif

#if ! defined(__SCE_MODULE_LINK_TRIGGER_BY_ASM) && ! defined(__SCE_MODULE_LINK_TRIGGER_BY_C)
#if defined(__GNUC__)
#define __SCE_MODULE_LINK_TRIGGER_BY_ASM 1
#endif
#endif

#if defined(__SCE_LIB_DECLARE_BY_ASM)
#define __SCE_LIB_DECLARE_BASE_MACRO( libname, attribute )                \
__asm__(                                                                  \
".pushsection   .psp_lib_mark_" #libname ",\"aw\",@progbits\n"            \
"        .align 2\n"                                                      \
"        .type __PSPLIB_" #libname ", @object\n"                          \
"        .size __PSPLIB_" #libname ", 8\n"                                \
"__PSPLIB_" #libname ":  .long  1 \n"                                     \
"           .long  " #attribute "\n"                                      \
"        .popsection\n")
#else
#define __SCE_LIB_DECLARE_BASE_MACRO( libname, attribute )                \
static unsigned int __PSPLIB_##libname []                                 \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused)) \
    = { 1, attribute }
#endif

#if defined(__SCE_LIB_DECLARE_BY_ASM)
#define __SCE_LIB_STUB_BASE_MACRO( libname, stubfile )                    \
__asm__(                                                                  \
".pushsection   .psp_lib_mark_" #libname ",\"aw\",@progbits\n"            \
"        .align 2\n"                                                      \
"        .type __PSPSFN_" #libname "_" #stubfile ", @object\n"            \
"        .size __PSPSFN_" #libname "_" #stubfile ", 4\n"                  \
"__PSPSFN_" #libname "_" #stubfile ":  .long  0 \n"                       \
"        .popsection\n")
#else
#define __SCE_LIB_STUB_BASE_MACRO( libname, stubfile )                    \
static unsigned int __PSPSFN_##libname##_##stubfile []                    \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused)) \
    = { 0 }
#endif

#if defined(__SCE_LIB_DECLARE_BY_ASM)
#define __SCE_LIB_DECLARE_BASE_MACRO2( libname, attribute )             \
__asm__(                                                                \
".pushsection   .psp_lib_mark_" #libname ",\"aw\",@progbits\n"          \
"        .align 2\n"                                                    \
"        .type __PSPLIB2_" #libname ", @object\n"                       \
"        .size __PSPLIB2_" #libname ", 12\n"                            \
"__PSPLIB2_" #libname ":  .long  2 \n"                                  \
"           .long  " #attribute "\n"                                    \
"           .long  1\n"                                                 \
"        .popsection\n")
#else
#define __SCE_LIB_DECLARE_BASE_MACRO2( libname, attribute )              \
static unsigned int __PSPLIB2_##libname []                               \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused))\
    = { 2, attribute, 1 }
#endif

#if defined(__SCE_LIB_DECLARE_BY_ASM)
#define __SCE_LIB_NIDSUF_BASE_MACRO( libname, nidsuffixstr )                \
__asm__(                                                                    \
".pushsection   .psp_lib_marknid__" #libname ",\"aw\",@progbits\n"          \
"        .align 0\n"                                                        \
"        .type __PSPLIBNID__" #libname ", @object\n"                        \
"__PSPLIBNID__" #libname ": .asciz \"" nidsuffixstr "\"\n"                  \
"        .size __PSPLIBNID__" #libname ", . - __PSPLIBNID__" #libname "\n"  \
"        .popsection\n")
#else
#define __SCE_LIB_NIDSUF_BASE_MACRO( libname, nidsuffixstr )                 \
static unsigned char __PSPLIBNID__##libname []                               \
    __attribute__((aligned(1),section(".psp_lib_marknid__" #libname),unused))\
    = nidsuffixstr
#endif


#define SYS_LIB_DECLARE_WITH_STUB( libname, attribute, stubfile ) \
        __SCE_LIB_DECLARE_BASE_MACRO( libname, attribute );       \
        __SCE_LIB_STUB_BASE_MACRO( libname, stubfile )

#define SYS_LIB_DECLARE( libname, attribute )  \
    SYS_LIB_DECLARE_WITH_STUB( libname, attribute, libname ## _stub )

#define SYS_LIB_DECLARE_WITH_STUB_NSUFFIX( libname, attribute, stubfile, nidsuffixstr ) \
        __SCE_LIB_DECLARE_BASE_MACRO2( libname, attribute );                \
        __SCE_LIB_STUB_BASE_MACRO( libname, stubfile );                     \
        __SCE_LIB_NIDSUF_BASE_MACRO( libname, nidsuffixstr )

#define SYS_LIB_DECLARE_WITH_NID_SUFFIX( libname, attribute, nidsuffixstr ) \
    SYS_LIB_DECLARE_WITH_STUB_NSUFFIX( libname, attribute,                  \
                                       libname ## _stub, nidsuffixstr )


#if defined(__SCE_MODULE_LINK_TRIGGER_BY_ASM)
#define __SCE_MODULE_LINK_FTRIGGER(exportname,funcname,libname)            \
__asm__(                                                                   \
".pushsection   .psp_lib_markimport_" #libname ",\"aw\",@progbits\n"       \
"        .align 3\n"                                                       \
"        .quad  " #funcname "\n"                                           \
"        .popsection\n")
#define __SCE_MODULE_LINK_VTRIGGER(exportname,varname,libname)             \
__asm__(                                                                   \
".pushsection   .psp_lib_markimport_" #libname ",\"aw\",@progbits\n"       \
"        .align 3\n"                                                       \
"        .quad  " #varname "\n"                                            \
"        .popsection\n")
#else
#define __SCE_MODULE_LINK_FTRIGGER(exportname,funcname,libname)            \
   static unsigned int __PSPimp_##libname##_##exportname##_##funcname []   \
   __attribute__ ((section(".psp_lib_markimport_" #libname )))             \
    = { (unsigned int)funcname }
#define __SCE_MODULE_LINK_VTRIGGER(exportname,varname,libname)             \
   static unsigned int __PSPimp_##libname##_##exportname##_##varname []    \
   __attribute__ ((section(".psp_lib_markimport_" #libname )))             \
    = { (unsigned int)&varname }
#endif

#if defined(__SCE_LIB_DECLARE_BY_ASM)
#define __SCE_LIB_EXPORT_BASE_MACRO(vfflag,exportname,fvname,libname,openlevel)\
__asm__(                                                                      \
".pushsection   .psp_lib_mark_" #libname ",\"aw\",@progbits\n"                \
"        .align 2\n"                                                          \
"        .type __PSPEXP_" #libname "_" #exportname ", @object\n"              \
"        .size __PSPEXP_" #libname "_" #exportname ", 8\n"                    \
"__PSPEXP_" #libname "_" #exportname ":\n"                                    \
"          .long  1, " #vfflag "\n"                                           \
"        .type __PSPREN_" #libname "_" #exportname "_" #fvname ", @object\n"  \
"        .size __PSPREN_" #libname "_" #exportname "_" #fvname ", 8\n"        \
"__PSPREN_" #libname "_" #exportname "_" #fvname ":\n"                        \
"          .long  1, " #openlevel "\n"                                        \
"        .popsection\n")
#else
#define __SCE_LIB_EXPORT_BASE_MACRO(vfflag,exportname,fvname,libname,openlevel)\
static unsigned int __PSPEXP_##libname##_##exportname []                    \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused))   \
    = { 1, vfflag };                                                        \
static unsigned int __PSPREN_##libname##_##exportname##_##fvname []         \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused))   \
    = { 1, openlevel }
#endif

#if defined(__SCE_LIB_DECLARE_BY_ASM)
#define __SCE_LIB_EXPORT_BASE_MACRO_WITH_NID(vfflag,exportname,fvname,nidname,libname,openlevel)\
__asm__(                                                                      \
".pushsection   .psp_lib_mark_" #libname ",\"aw\",@progbits\n"                \
"        .align 2\n"                                                          \
"        .type __PSPEXP_" #libname "_" #exportname ", @object\n"              \
"        .size __PSPEXP_" #libname "_" #exportname ", 8\n"                    \
"__PSPEXP_" #libname "_" #exportname ":\n"                                    \
"          .long  1, " #vfflag "\n"                                           \
"        .type __PSPNID_" #libname "_" #exportname "_" #nidname ", @object\n" \
"        .size __PSPNID_" #libname "_" #exportname "_" #nidname ", 4\n"       \
"__PSPNID_" #libname "_" #exportname "_" #nidname ":\n"                       \
"          .long  0 \n"                                                       \
"        .type __PSPREN_" #libname "_" #exportname "_" #fvname ", @object\n"  \
"        .size __PSPREN_" #libname "_" #exportname "_" #fvname ", 8\n"        \
"__PSPREN_" #libname "_" #exportname "_" #fvname ":\n"                        \
"          .long  1, " #openlevel "\n"                                        \
"        .popsection\n")
#else
#define __SCE_LIB_EXPORT_BASE_MACRO_WITH_NID(vfflag,exportname,fvname,nidname,libname,openlevel)\
static unsigned int __PSPEXP_##libname##_##exportname []                    \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused))   \
    = { 1, vfflag };                                                        \
static unsigned int __PSPNID_##libname##_##exportname##_##nidname []        \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused))   \
    = { 0 };                                                                \
static unsigned int __PSPREN_##libname##_##exportname##_##fvname []         \
    __attribute__((aligned(4),section(".psp_lib_mark_" #libname),unused))   \
    = { 1, openlevel }
#endif


#define SYS_LIB_EXPORT_FUNC_WITH_FULLOPTION(exportname,funcname,libname,openlevel) \
  __SCE_MODULE_LINK_FTRIGGER(exportname,funcname,libname); \
  __SCE_LIB_EXPORT_BASE_MACRO(0,exportname,funcname,libname,openlevel)

#define SYS_LIB_EXPORT_WITH_FULLOPTION( exportname, funcname, libname, openlevel) \
    SYS_LIB_EXPORT_FUNC_WITH_FULLOPTION(exportname,funcname,libname,openlevel)

#define SYS_LIB_EXPORT_WITH_OPENLEVEL( funcname, libname, openlevel ) \
    SYS_LIB_EXPORT_FUNC_WITH_FULLOPTION( funcname,funcname,libname,openlevel)

#define SYS_LIB_EXPORT_OTHER_NAME( exportname, funcname, libname ) \
    SYS_LIB_EXPORT_FUNC_WITH_FULLOPTION( exportname, funcname, libname, 0 )

#define SYS_LIB_EXPORT( funcname, libname ) \
    SYS_LIB_EXPORT_WITH_OPENLEVEL( funcname, libname, 0 )


#define SYS_LIB_EXPORT_FUNC_OTHER_NIDNAME_WITH_FULLOPTION(exportname,funcname,nidname,libname,openlevel) \
  __SCE_MODULE_LINK_FTRIGGER(exportname,funcname,libname); \
  __SCE_LIB_EXPORT_BASE_MACRO_WITH_NID(0,exportname,funcname,nidname,libname,openlevel)

#define SYS_LIB_EXPORT_OTHER_NID_WITH_OPENLEVEL( funcname, nidname, libname, openlevel ) \
    SYS_LIB_EXPORT_FUNC_OTHER_NIDNAME_WITH_FULLOPTION(funcname,funcname,nidname,libname,openlevel)

#define SYS_LIB_EXPORT_OTHER_NAME_OTHER_NID( exportname, funcname, nidname, libname ) \
    SYS_LIB_EXPORT_FUNC_OTHER_NIDNAME_WITH_FULLOPTION(exportname,funcname,nidname,libname,0)

#define SYS_LIB_EXPORT_OTHER_NID( funcname, nidname, libname )  \
    SYS_LIB_EXPORT_OTHER_NID_WITH_OPENLEVEL( funcname, nidname, libname, 0 )


#define SYS_LIB_EXPORT_VAR_WITH_FULLOPTION(exportname,varname,libname,openlevel) \
  __SCE_MODULE_LINK_VTRIGGER(exportname,varname,libname); \
  __SCE_LIB_EXPORT_BASE_MACRO(1,exportname,varname,libname,openlevel)

#define SYS_LIB_EXPORT_VAR_WITH_OPENLEVEL( varname, libname, openlevel ) \
    SYS_LIB_EXPORT_VAR_WITH_FULLOPTION( varname, varname, libname, openlevel )

#define SYS_LIB_EXPORT_VAR_OTHER_NAME( exportname, varname, libname ) \
    SYS_LIB_EXPORT_VAR_WITH_FULLOPTION( exportname, varname, libname, 0 )

#define SYS_LIB_EXPORT_VAR( varname, libname ) \
    SYS_LIB_EXPORT_VAR_WITH_OPENLEVEL( varname, libname, 0 )


#define SYS_LIB_EXPORT_VAR_OTHER_NIDNAME_WITH_FULLOPTION(exportname,varname,nidname,libname,openlevel)\
  __SCE_MODULE_LINK_VTRIGGER(exportname,varname,libname); \
  __SCE_LIB_EXPORT_BASE_MACRO_WITH_NID(1,exportname,varname,nidname,libname,openlevel)

#define SYS_LIB_EXPORT_VAR_OTHER_NID_WITH_OPENLEVEL( varname, nidname, libname, openlevel ) \
    SYS_LIB_EXPORT_VAR_OTHER_NIDNAME_WITH_FULLOPTION( varname, varname, nidname, libname, openlevel )

#define SYS_LIB_EXPORT_VAR_OTHER_NAME_OTHER_NID( exportname, varname, nidname, libname ) \
    SYS_LIB_EXPORT_VAR_OTHER_NIDNAME_WITH_FULLOPTION( exportname, varname, nidname, libname, 0 )

#define SYS_LIB_EXPORT_VAR_OTHER_NID( varname, nidname, libname ) \
    SYS_LIB_EXPORT_VAR_OTHER_NID_WITH_OPENLEVEL( varname, nidname, libname, 0 )


/* ------------ module entry macros ------------ */
typedef struct _scemoduleinfo {
	unsigned short modattribute;
	unsigned char modversion[2];	/* minor, major, etc... */
	char modname[SYS_MODULE_NAME_LEN];
	char terminal;
	void *gp_value;
	const void *ent_top;
	const void *ent_end;
	const void *stub_top;
	const void *stub_end;
} _sceModuleInfo;

typedef const _sceModuleInfo SceModuleInfo;

#define SYS_MODULE_INFO_SECTION                                       \
       __attribute__((aligned(4),section(SYS_MODINFO_SECTION_NAME,"a"),unused))
#define SYS_IMPORT_VARIABLE_ATTR \
       __attribute__((section(SYS_VARIABLE_STUB_SECTION_NAME)))

#define SYS_LIB_TABEL_ADDRESS_DEFINE() \
static const int __begin_of_section_lib_ent[1]                             \
       __attribute__((aligned(4),section(".lib.ent.top"),unused)) = {0};   \
static const int __end_of_section_lib_ent[1]                               \
       __attribute__((aligned(4),section(".lib.ent.btm"),unused)) = {0};   \
static const int __begin_of_section_lib_stub[1]                            \
       __attribute__((aligned(4),section(".lib.stub.top"),unused)) = {0};  \
static const int __end_of_section_lib_stub[1]                              \
       __attribute__((aligned(4),section(".lib.stub.btm"),unused)) = {0}

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
#define SYS_MODULE_INFO( name, attribute, major, minor )             \
    SYS_LIB_TABEL_ADDRESS_DEFINE();                                  \
    extern                                                           \
    SceModuleInfo __psp_moduleinfo SYS_MODULE_INFO_SECTION = {       \
        attribute, { minor, major }, #name, 0, static_cast<void*>(0), \
        static_cast<const void*>(__begin_of_section_lib_ent+1),      \
        static_cast<const void*>(__end_of_section_lib_ent),          \
        static_cast<const void*>(__begin_of_section_lib_stub+1),     \
        static_cast<const void*>(__end_of_section_lib_stub) };       \
    SYS_LIB_EXPORT_VAR_OTHER_NAME( module_info, __psp_moduleinfo, )
#else
#define SYS_MODULE_INFO( name, attribute, major, minor )             \
    SYS_LIB_TABEL_ADDRESS_DEFINE();                                  \
    SceModuleInfo __psp_moduleinfo SYS_MODULE_INFO_SECTION = {       \
        attribute, { minor, major }, #name, 0, (void*)0,             \
        (const void*)(__begin_of_section_lib_ent+1),                 \
        (const void*)(__end_of_section_lib_ent),                     \
        (const void*)(__begin_of_section_lib_stub+1),                \
        (const void*)(__end_of_section_lib_stub) };                  \
    SYS_LIB_EXPORT_VAR_OTHER_NAME( module_info, __psp_moduleinfo, )
#endif

#define SYS_MODULE_ATTR_CANT_STOP       (0x0001)
#define SYS_MODULE_ATTR_EXCLUSIVE_LOAD  (0x0002)
#define SYS_MODULE_ATTR_EXCLUSIVE_START (0x0004)

#define SYS_MODULE_PROLOGUE( funcname ) \
    SYS_LIB_EXPORT_OTHER_NAME( module_prologue, funcname, )

#define SYS_MODULE_EPILOGUE( funcname ) \
    SYS_LIB_EXPORT_OTHER_NAME( module_epilogue, funcname, )

#define SYS_MODULE_START( funcname ) \
    SYS_LIB_EXPORT_OTHER_NAME( module_start, funcname, )

#define SYS_MODULE_STOP( funcname ) \
    SYS_LIB_EXPORT_OTHER_NAME( module_stop, funcname, )

#define SYS_MODULE_EXIT( funcname ) \
    SYS_LIB_EXPORT_OTHER_NAME( module_exit, funcname, )

#define SYS_MODULE_REBOOT_BEFORE( funcname ) \
    SYS_LIB_EXPORT_OTHER_NAME( module_reboot_before, funcname, )

#define SYS_MODULE_STOP_LEVEL_USER	0x00000000
#define SYS_MODULE_STOP_LEVEL_SYSTEM	0x00004000

#define SYS_MODULE_STOP_LEVEL( level ) \
    SYS_LIB_EXPORT_VAR_OTHER_NAME( module_stop_level,          \
                               _sce_module_stop_level, );                 \
    int _sce_module_stop_level = (level)

#define SYS_MODULE_START_THREAD_PARAMETER( initPriority, stackSize, attr ) \
    SYS_LIB_EXPORT_VAR_OTHER_NAME( module_start_thread_parameter,          \
                               _sce_module_startthpara, );                 \
    int _sce_module_startthpara[] = {3, initPriority, stackSize, attr}

#define SYS_MODULE_STOP_THREAD_PARAMETER( initPriority, stackSize, attr )   \
    SYS_LIB_EXPORT_VAR_OTHER_NAME( module_stop_thread_parameter,           \
                               _sce_module_stopthpara, );                  \
    int _sce_module_stopthpara[] = {3, initPriority, stackSize, attr}

#define SYS_MODULE_REBOOT_BEFORE_THREAD_PARAMETER( initPriority, stackSize, attr )  \
    SYS_LIB_EXPORT_VAR_OTHER_NAME( module_reboot_before_thread_parameter,  \
                               _sce_module_stopthpara, );                  \
    int _sce_module_reboot_beforethpara[] = {3, initPriority, stackSize, attr}

/* for compatbility */

#ifndef CONFIG_MODULEEXPORT_DISABLE_BACKWORD_COMPATIBILITY
#define SCE_MODULE_INFO         SYS_MODULE_INFO
#define SCE_MODULE_START        SYS_MODULE_START
#define SCE_MODULE_STOP         SYS_MODULE_STOP

#define SCE_LIB_AUTO_EXPORT     SYS_LIB_AUTO_EXPORT
#define SCE_LIB_WEAK_IMPORT     SYS_LIB_WEAK_IMPORT

#define SCE_LIB_DECLARE         SYS_LIB_DECLARE

#define SCE_LIB_EXPORT_FUNC_WITH_FULLOPTION     SYS_LIB_EXPORT_FUNC_WITH_FULLOPTION
#define SCE_LIB_EXPORT_WITH_FULLOPTION          SYS_LIB_EXPORT_WITH_FULLOPTION
#define SCE_LIB_EXPORT_WITH_OPENLEVEL           SYS_LIB_EXPORT_WITH_OPENLEVEL
#define SCE_LIB_EXPORT_OTHER_NAME               SYS_LIB_EXPORT_OTHER_NAME
#define SCE_LIB_EXPORT                          SYS_LIB_EXPORT

#define SCE_LIB_EXPORT_VAR_OTHER_NAME           SYS_LIB_EXPORT_VAR_OTHER_NAME
#define SCE_LIB_EXPORT_VAR                      SYS_LIB_EXPORT_VAR
#endif /* CONFIG_MODULEEXPORT_DISABLE_BACKWORD_COMPATIBILITY */

#endif /* _SYS_MODULEEXPORT_H */
