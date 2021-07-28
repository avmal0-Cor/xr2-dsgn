/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_H__
#define __CELL_SPURS_H__

/* SPURS */
#include <cell/spurs/types.h>
#include <cell/spurs/trace_types.h>
#include <cell/spurs/error.h>
#include <cell/spurs/workload.h>
#include <cell/spurs/trace.h>
#include <cell/spurs/exception_common_types.h>
#include <cell/spurs/system_workload_types.h>
#ifdef __SPU__
#include <cell/spurs/common.h>
#else /* __SPU__ */
#include <cell/spurs/control.h>
#include <cell/spurs/lv2_event_queue.h>
#include <cell/spurs/exception.h>
#include <cell/spurs/system_workload.h>
#endif /* #ifdef __SPU__ */


#ifdef __cplusplus
#include <cell/spurs/types_cpp.h>
#endif /* __cplusplus */



/* SPURS policy module */
#include <cell/spurs/ready_count.h>
#include <cell/spurs/policy_module.h>

/* SPURS task */
#include <cell/spurs/task.h>
#include <cell/spurs/barrier.h>
#include <cell/spurs/event_flag.h>
#include <cell/spurs/queue.h>
#include <cell/spurs/lfqueue.h>
#include <cell/spurs/semaphore.h>
#include <cell/spurs/task_exit_code.h>

#ifdef __cplusplus
#include <cell/spurs/task_types_cpp.h>
#endif /* __cplusplus */


/* SPURS job */
#include <cell/spurs/job_commands.h>
#include <cell/spurs/job_descriptor.h>
#include <cell/spurs/job_guard.h>
#include <cell/spurs/job_chain.h>
#ifdef __SPU__
#include <cell/spurs/job_context.h>
#else
#include <cell/spurs/job_context_types.h>
#endif /* #ifdef __SPU__ */

#ifdef __cplusplus
#include <cell/spurs/job_chain_types_cpp.h>
#endif /* __cplusplus */


/* SPURS job queue */
#include <cell/spurs/job_queue.h>
#include <cell/spurs/job_queue_semaphore.h>
#include <cell/spurs/job_queue_port.h>


/* Utility macro for reading PPU symbols from SPU program*/
#ifdef __SPU__
#define CELL_SPURS_PPU_SYM(ppu_sym) \
        ((unsigned int)* \
          (__extension__ \
            ({ \
              extern char _cell_spurs_##ppu_sym[] __asm__ (#ppu_sym "@ppu"); \
              static char *ptr = _cell_spurs_##ppu_sym; \
              &ptr; \
            }) \
          ) \
        )
#endif /* __SPU__ */


#endif /* __CELL_SPURS_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
