/*   SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
*   Copyright (C) 2005 Sony Computer Entertainment Inc.
*   All Rights Reserved.
*/

#ifndef __CELL_CELLSTATUS_H__
#define __CELL_CELLSTATUS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CELL_OK 0

#define CELL_STATUS_IS_FAILURE(status) ((status) & 0x80000000)

#define CELL_STATUS_IS_SUCCESS(status) (!((status) & 0x80000000))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CELL_CELLSTATUS_H__ */

