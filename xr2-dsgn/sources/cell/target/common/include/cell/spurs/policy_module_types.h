/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_POLICY_MODULE_TYPES_H__
#define __CELL_SPURS_POLICY_MODULE_TYPES_H__

typedef enum CellSpursModulePollStatus {
	CELL_SPURS_MODULE_POLL_STATUS_READYCOUNT	= (1<<0),
	CELL_SPURS_MODULE_POLL_STATUS_SIGNAL		= (1<<1),
	CELL_SPURS_MODULE_POLL_STATUS_FLAG			= (1<<2)
} CellSpursModulePollStatus;

#endif /* __CELL_SPURS_POLICY_MODULE_TYPES_H__ */
