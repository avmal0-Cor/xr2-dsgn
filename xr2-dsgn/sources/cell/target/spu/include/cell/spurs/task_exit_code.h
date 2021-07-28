/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_TASK_EXIT_CODE_H__
#define __CELL_SPURS_TASK_EXIT_CODE_H__ 1

/* The C Standard Library */
#include <stdint.h>

/* SPURS */
#include <cell/spurs/task_exit_code_types.h>
#include <cell/spurs/error.h>

#ifdef __cplusplus
extern "C" {
#endif

int cellSpursTaskExitCodeInitialize(uint64_t);
int cellSpursTaskExitCodeGet(uint64_t, int *exitCode);
int cellSpursTaskExitCodeTryGet(uint64_t, int *exitCode);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* __CELL_SPURS_TASK_EXIT_CODE_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
