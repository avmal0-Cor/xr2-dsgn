/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_SPURS_LV2_EVENT_QUEUE_CONFIG_H__
#define __CELL_SPURS_LV2_EVENT_QUEUE_CONFIG_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* Lv2 */
#include <sys/event.h>

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpurs */
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursAttachLv2EventQueue(CellSpurs* spurs, sys_event_queue_t queue, uint8_t* port, int isDynamic);
int cellSpursDetachLv2EventQueue(CellSpurs* spurs, uint8_t port);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_LV2_EVENT_QUEUE_CONFIG_H__ */

/*
 * Local Variables:
 * mode: C
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
