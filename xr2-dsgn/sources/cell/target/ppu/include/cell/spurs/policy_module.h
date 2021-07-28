/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef __CELL_SPURS_POLICY_MODULE_H__
#define __CELL_SPURS_POLICY_MODULE_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/types.h>			/* CellSpurs, CellSpursWorkloadId */
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursRequestIdleSpu(CellSpurs*, CellSpursWorkloadId, unsigned int nIdleSpu);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __CELL_SPURS_POLICY_MODULE_H__ */
/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
