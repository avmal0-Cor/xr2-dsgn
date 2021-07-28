/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_GPAD_H__
#define __CELL_GCM_GPAD_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum{
	CELL_GCM_GPAD_STATUS_NONE 			= 0,
	CELL_GCM_GPAD_STATUS_IS_CAPTURING	= 2,
	CELL_GCM_GPAD_STATUS_UI_ENABLED		= 4,
	CELL_GCM_GPAD_STATUS_LIVE_ENABLED	= 8
}CellGcmGpadStatus;

int32_t cellGcmGpadGetStatus(uint32_t *status);
int32_t cellGcmGpadNotifyCaptureSurface(CellGcmSurface *surface);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* __CELL_GCM_GPAD_H__ */
