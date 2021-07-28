/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __CELL_OVIS_UTIL_H__
#define __CELL_OVIS_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/spu_thread.h>

void 
cellOvisFixSpuSegments(sys_spu_image_t *image);


void 
cellOvisInvalidateOverlappedSegments(sys_spu_segment_t *segs, int *nsegs);


#ifdef __cplusplus
}
#endif

#endif /* __CELL_OVIS_UTIL_H__ */


