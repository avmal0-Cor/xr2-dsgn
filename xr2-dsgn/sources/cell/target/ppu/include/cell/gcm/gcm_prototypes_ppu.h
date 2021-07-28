/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(Flush)(CELL_GCM_NO_ARGS());
CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(Finish)(CELL_GCM_ARGS(uint32_t referenceValue));
CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(ResetDefaultCommandBuffer)(CELL_GCM_NO_ARGS());

#if (CELL_GCM_INLINE==0)
#if (CELL_GCM_UNSAFE==0)
CELL_GCM_DECL CELL_GCM_FUNC_TYPE_INT32 CELL_GCM_FUNC(SetPrepareFlip)(CELL_GCM_ARGS(const uint8_t id));
#endif
#endif

#ifdef CELL_GCM_BUILD_SYS
#if (CELL_GCM_INLINE==0)
#if (CELL_GCM_UNSAFE==0)
CELL_GCM_DECL CELL_GCM_FUNC_TYPE_INT32 CELL_GCM_FUNC(SetFlip)(CELL_GCM_ARGS(const uint8_t id));
#endif
#endif
#else // #ifndef CELL_GCM_BUILD_SYS
#if (CELL_GCM_INLINE==0)
CELL_GCM_DECL CELL_GCM_FUNC_TYPE_INT32 CELL_GCM_FUNC(SetFlip)(CELL_GCM_ARGS(const uint8_t id));
#endif
#endif // CELL_GCM_BUILD_SYS

#if (CELL_GCM_INLINE==0)
CELL_GCM_DECL CELL_GCM_FUNC_TYPE_INT32 CELL_GCM_FUNC(SetFlipWithWaitLabel)(CELL_GCM_ARGS(const uint8_t id, const uint8_t labelindex, const uint32_t labelvalue));
#endif
