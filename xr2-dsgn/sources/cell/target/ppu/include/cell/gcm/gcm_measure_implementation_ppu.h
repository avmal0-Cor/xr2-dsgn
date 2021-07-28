/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(Flush)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_RESERVE( 0 );
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(Finish)(CELL_GCM_ARGS(uint32_t referenceValue))
{
    CELL_GCM_FUNC_CALL(SetReferenceCommand)(CELL_GCM_ARGS_FUNC(referenceValue));
    CELL_GCM_FUNC_CALL(Flush)(CELL_GCM_NO_ARGS_FUNC());
	CELL_GCM_FUNC_RETURN;
}

CELL_GCM_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(ResetDefaultCommandBuffer)(CELL_GCM_NO_ARGS())
{
	CELL_GCM_FUNC_CALL(SetJumpCommand)(CELL_GCM_ARGS_FUNC(CELL_GCM_INIT_STATE_OFFSET));
    CELL_GCM_FUNC_CALL(Flush)(CELL_GCM_NO_ARGS_FUNC());
	CELL_GCM_FUNC_RETURN;
}

#ifndef CELL_GCM_BUILD_SYS
#if (CELL_GCM_INLINE==0)
CELL_GCM_DECL CELL_GCM_FUNC_TYPE_INT32 CELL_GCM_FUNC(SetFlip)(CELL_GCM_ARGS(const uint8_t id))
{
	(void) id;
	CELL_GCM_RESERVE_RET(19, CELL_GCM_ERROR_FAILURE);
	CELL_GCM_FUNC_RETURN_CELL_OK;
}
#endif
#endif // CELL_GCM_BUILD_SYS

#if (CELL_GCM_INLINE==0)
CELL_GCM_DECL CELL_GCM_FUNC_TYPE_INT32 CELL_GCM_FUNC(SetFlipWithWaitLabel)(CELL_GCM_ARGS(const uint8_t id, const uint8_t labelindex, const uint32_t labelvalue))
{
	(void) id;
	(void) labelindex;
	(void) labelvalue;

	CELL_GCM_RESERVE_RET(23, CELL_GCM_ERROR_FAILURE);
	CELL_GCM_FUNC_RETURN_CELL_OK;
}
#endif
