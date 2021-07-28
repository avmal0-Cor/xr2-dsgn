/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#define	CELL_GCM_FUNCTION_MACROS
#include <cell/gcm/gcm_function_macros.h>


CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPerfMonMarker)(CELL_GCM_ARGS(const char *szString))
{
	return CELL_GCM_CALLEE(SetPerfMonMarker)(CELL_GCM_THIS, szString);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPerfMonPushMarker)(CELL_GCM_ARGS(const char *szString))
{
	return CELL_GCM_CALLEE(SetPerfMonPushMarker)(CELL_GCM_THIS, szString);
}

CELL_GCM_GLOBAL_DECL CELL_GCM_FUNC_TYPE CELL_GCM_FUNC(SetPerfMonPopMarker)(CELL_GCM_NO_ARGS())
{
	return CELL_GCM_CALLEE(SetPerfMonPopMarker)(CELL_GCM_THIS);
}


#undef	CELL_GCM_FUNCTION_MACROS
#include <cell/gcm/gcm_function_macros.h>
