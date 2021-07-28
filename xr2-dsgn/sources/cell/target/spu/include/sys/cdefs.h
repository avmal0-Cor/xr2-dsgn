/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2005 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SYS_SYS_CDEFS_H__
#define __SYS_SYS_CDEFS_H__

#if defined(__cplusplus)
#define							CDECL_BEGIN   extern "C" {
#define							CDECL_END }
#define NAMESPACE_LV2_BEGIN	namespace lv2 {
#define NAMESPACE_LV2_END	}
#else /* defined(__cplusplus) */
#define							CDECL_BEGIN
#define							CDECL_END
#define NAMESPACE_LV2_BEGIN
#define NAMESPACE_LV2_END
#endif /* defined(__cplusplus) */

#if defined(__cplusplus)
#define __C_STD_BEGIN	namespace std {
#define __C_STD_END	}
#define __CSTD		::std::
#define __STD_USING
#else /* defined(__cplusplus) */
#define __C_STD_BEGIN
#define __C_STD_END
#define __CSTD
#undef __STD_USING
#endif /* defined(__cplusplus) */

#endif // __SYS_SYS_CDEFS_H__
