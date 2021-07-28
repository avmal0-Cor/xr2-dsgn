/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(Flush)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(Flush)(CELL_GCM_THIS);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(Finish)(CELL_GCM_ARGS(uint32_t referenceValue))
{
	return CELL_GCM_CALLEE(Finish)(CELL_GCM_THIS, referenceValue);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(ResetDefaultCommandBuffer)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(ResetDefaultCommandBuffer)(CELL_GCM_THIS);
}

#if (CELL_GCM_INLINE==0)
#if (CELL_GCM_UNSAFE==0)
CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE_UINT32 CELL_GCM_FUNC(SetPrepareFlip)(CELL_GCM_ARGS(const uint8_t id))
{
	return CELL_GCM_CALLEE(SetPrepareFlip)(CELL_GCM_THIS, id);
}
#endif
#endif

#ifndef CELL_GCM_BUILD_SYS
#if (CELL_GCM_INLINE==0)
CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE_UINT32 CELL_GCM_FUNC(SetFlip)(CELL_GCM_ARGS(const uint8_t id))
{
	return CELL_GCM_CALLEE(SetFlip)(CELL_GCM_THIS, id);
}
#endif
#endif

#if (CELL_GCM_INLINE==0)
CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE_UINT32 CELL_GCM_FUNC(SetFlipWithWaitLabel)(CELL_GCM_ARGS(const uint8_t id, const uint8_t labelindex, const uint32_t labelvalue))
{
	return CELL_GCM_CALLEE(SetFlipWithWaitLabel)(CELL_GCM_THIS, id, labelindex, labelvalue);
}
#endif
