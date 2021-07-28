/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2009 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef LV2_DBG_INCLUDED
#define LV2_DBG_INCLUDED

#include <sys/system_types.h>
#include <sys/process.h>
#include <sys/ppu_thread.h>
#include <sys/spu_thread_group.h>
#include <sys/spu_thread.h>
#include <sys/synchronization.h>
#include <sys/event.h>

#ifdef __cplusplus
#define CDECL extern "C"
#else // __cplusplus
#define CDECL extern
#endif // __cplusplus

/* PPU core*/
#define PPU_GPR_NUM	32	/* Number of PPU GPRs */
#define PPU_FPR_NUM	32	/* Number of PPU FPRs */
#define PPU_VR_NUM	32	/* Number of PPU VRs (Vector registers) */
#define PPU_VR_WIDTH	16	/* VR width 16 bytes (128bit) */

/* SPU core */
#define SPU_GPR_NUM		128	/* Number of SPU GPRs */
#define SPU_MFC_CQ_SR_NUM	96	/* Number of SPU MFC CQ SR */
#define SPU_GPR_WIDTH		16	/* SPU GPR width 16 bytes (128bit) */
#define SPU_MB_NUM		4	/* Number of SPU_MB */
#define SPU_FPSCR_WIDTH		16	/* SPU FPSCR width 16 bytes (128bit) */


/*
 * CELL_ERROR_FACILITY_SYSTEM_SERVICE	0x001
 * 	liblv2dbg	: 0x8001_0401- 0x8001_04ff
 *
 */
/* #define CELL_ERROR_FACILITY_SYSTEM_SERVICE	0x001	*/

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0401) */
#define CELL_LV2DBG_ERROR_DEINVALIDPROCESSID		(0x80010401)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0402) */
#define CELL_LV2DBG_ERROR_DEINVALIDTHREADID		(0x80010402)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0403) */
#define CELL_LV2DBG_ERROR_DEILLEGALREGISTERTYPE		(0x80010403)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0404) */
#define CELL_LV2DBG_ERROR_DEILLEGALREGISTERNUMBER	(0x80010404)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0405) */
#define CELL_LV2DBG_ERROR_DEILLEGALTHREADSTATE		(0x80010405)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0406) */
#define CELL_LV2DBG_ERROR_DEINVALIDEFFECTIVEADDRESS	(0x80010406)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0407) */
#define CELL_LV2DBG_ERROR_DENOTFOUNDPROCESSID		(0x80010407)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0408) */
#define CELL_LV2DBG_ERROR_DENOMEM			(0x80010408)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0409) */
#define CELL_LV2DBG_ERROR_DEINVALIDARGUMENTS		(0x80010409)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x040a) */
#define CELL_LV2DBG_ERROR_DENOTFOUNDFILE		(0x8001040a)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x040b) */
#define CELL_LV2DBG_ERROR_DEINVALIDFILETYPE		(0x8001040b)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x040c) */
#define CELL_LV2DBG_ERROR_DENOTFOUNDTHREADID		(0x8001040c)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x040d) */
#define CELL_LV2DBG_ERROR_DEINVALIDTHREADSTATUS		(0x8001040d)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x040e) */
#define CELL_LV2DBG_ERROR_DENOAVAILABLEPROCESSID	(0x8001040e)	

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x040f) */
#define CELL_LV2DBG_ERROR_DENOTFOUNDEVENTHANDLER	(0x8001040f)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0410) */
#define CELL_LV2DBG_ERROR_DESPNOROOM			(0x80010410)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0411) */
#define CELL_LV2DBG_ERROR_DESPNOTFOUND			(0x80010411)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0412) */
#define CELL_LV2DBG_ERROR_DESPINPROCESS			(0x80010412)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0413) */
#define CELL_LV2DBG_ERROR_DEINVALIDPRIMARYSPUTHREADID	(0x80010413)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0414) */
#define CELL_LV2DBG_ERROR_DETHREADSTATEISNOTSTOPPED	(0x80010414)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0415) */
#define CELL_LV2DBG_ERROR_DEINVALIDTHREADTYPE		(0x80010415)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0416) */
#define CELL_LV2DBG_ERROR_DECONTINUEFAILED		(0x80010416)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0417) */
#define CELL_LV2DBG_ERROR_DESTOPFAILED			(0x80010417)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0418) */
#define CELL_LV2DBG_ERROR_DENOEXCEPTION			(0x80010418)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0419) */
#define CELL_LV2DBG_ERROR_DENOMOREEVENTQUE		(0x80010419)	

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x041a) */
#define CELL_LV2DBG_ERROR_DEEVENTQUENOTCREATED		(0x8001041a)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x041b) */
#define CELL_LV2DBG_ERROR_DEEVENTQUEOVERFLOWED		(0x8001041b)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x041c) */
#define CELL_LV2DBG_ERROR_DENOTIMPLEMENTED		(0x8001041c)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x041d) */
#define CELL_LV2DBG_ERROR_DEQUENOTREGISTERED		(0x8001041d)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x041e) */
#define CELL_LV2DBG_ERROR_DENOMOREEVENTPROCESS		(0x8001041e)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x041f) */
#define CELL_LV2DBG_ERROR_DEPROCESSNOTREGISTERED	(0x8001041f)	

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0420) */
#define CELL_LV2DBG_ERROR_DEEVENTDISCARDED		(0x80010420)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0421) */
#define CELL_LV2DBG_ERROR_DENOMORESYNCID		(0x80010421)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0422) */
#define CELL_LV2DBG_ERROR_DESYNCIDALREADYADDED		(0x80010422)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0423) */
#define CELL_LV2DBG_ERROR_DESYNCIDNOTFOUND		(0x80010423)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0424) */
#define CELL_LV2DBG_ERROR_DESYNCIDNOTACQUIRED		(0x80010424)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0425) */
#define CELL_LV2DBG_ERROR_DEPROCESSALREADYREGISTERED	(0x80010425)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0426) */
#define CELL_LV2DBG_ERROR_DEINVALIDLSADDRESS		(0x80010426)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0427) */
#define CELL_LV2DBG_ERROR_DEINVALIDOPERATION		(0x80010427)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0428) */
#define CELL_LV2DBG_ERROR_DEINVALIDMODULEID		(0x80010428)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x0429) */
#define CELL_LV2DBG_ERROR_DEHANDLERALREADYREGISTERED	(0x80010429)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x042a) */
#define CELL_LV2DBG_ERROR_DEINVALIDHANDLER		(0x8001042a)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x042b) */
#define CELL_LV2DBG_ERROR_DEHANDLENOTREGISTERED		(0x8001042b)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x042c) */
#define CELL_LV2DBG_ERROR_DEOPERATIONDENIED		(0x8001042c)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x042d) */
#define CELL_LV2DBG_ERROR_DEHANDLERNOTINITIALIZED	(0x8001042d)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x042e) */
#define CELL_LV2DBG_ERROR_DEHANDLERALREADYINITIALIZED	(0x8001042e)

/* ERR: CELL_LV2DBG_MAKE_ERROR(0x042f) */
#define CELL_LV2DBG_ERROR_DEILLEGALCOREDUMPPARAMETER	(0x8001042f)


/* Control Flags */
#define SYS_DBG_PPU_THREAD_STOP				0x0000000000000001ULL	/* Stop all PPU threads except for the exception handler when an exception occured */
#define SYS_DBG_SPU_THREAD_GROUP_STOP			0x0000000000000002ULL	/* Stop all SPU thread groups when an exception occured */
#define SYS_DBG_SYSTEM_PPU_THREAD_NOT_STOP		0x0000000000000004ULL	/* Don't stop the PPU thread that name begin with '_' when the exception handler stop a PPU thread */
#define SYS_DBG_SYSTEM_SPU_THREAD_GROUP_NOT_STOP	0x0000000000000008ULL	/* Don't stop the the SPU thread group that name begin with '_' when the exception handler stop a SPU thread group */
#define SYS_DBG_NOT_EXE_CTRL_BY_COREDUMP_EVENT		0x0000000000000010ULL	/* Don't do the execution control, when a core dump event was received */


/*
 * Event type
 *
 */ 
/* PPU Excpetion Type */
#define SYS_DBG_PPU_EXCEPTION_TRAP			0x0000000001000000ULL	/* Program Interrupt by trap instruction execution */
#define SYS_DBG_PPU_EXCEPTION_PREV_INST			0x0000000002000000ULL	/* Program Interrupt by privilege instruction execution */
#define SYS_DBG_PPU_EXCEPTION_ILL_INST			0x0000000004000000ULL	/* Program Interrupt by illegal instruction execution */
#define SYS_DBG_PPU_EXCEPTION_TEXT_HTAB_MISS		0x0000000008000000ULL	/* Instruction Storage Interrupt */
#define SYS_DBG_PPU_EXCEPTION_TEXT_SLB_MISS		0x0000000010000000ULL	/* Instruction Segment Interrupt */
#define SYS_DBG_PPU_EXCEPTION_DATA_HTAB_MISS		0x0000000020000000ULL	/* Data Storage Interrupt */
#define SYS_DBG_PPU_EXCEPTION_DATA_SLB_MISS		0x0000000040000000ULL	/* Data Segment Interrupt */
#define SYS_DBG_PPU_EXCEPTION_FLOAT			0x0000000080000000ULL	/* Program Interrupt by floating point enabled exception (Only when target PPU thread is running under precise mode)*/
#define SYS_DBG_PPU_EXCEPTION_DABR_MATCH		0x0000000100000000ULL	/* Access made to an address set as DABR */
#define SYS_DBG_PPU_EXCEPTION_ALIGNMENT			0x0000000200000000ULL	/* Alignment Interrupt */
#define SYS_DBG_PPU_EXCEPTION_DATA_MAT			0x0000002000000000ULL	/* Memory access trap */

/* Others */
#define SYS_DBG_EVENT_CORE_DUMPED			0x0000000000001000ULL	/* Core Dump completion event (Only when core dump is enabled) */
#define SYS_DBG_EVENT_PPU_EXCEPTION_HANDLER_SIGNALED	0x0000000000002000ULL	/* Occurs when sys_dbg_signal_to_ppu_exception_handler() */

/* Type for Vector registers of PPU */
typedef union {
  uint8_t	byte[PPU_VR_WIDTH];
  uint16_t	halfword[PPU_VR_WIDTH/sizeof(uint16_t)];
  uint32_t	word[PPU_VR_WIDTH/sizeof(uint32_t)];
  uint64_t	dw[PPU_VR_WIDTH/sizeof(uint64_t)];
} sys_dbg_vr_t;

/* Data structure for PPU thread context */
typedef struct {
  uint64_t	gpr[PPU_GPR_NUM];	/* General Purpose Register GPR[0-31] */
  uint32_t	cr;			/* Condition Register */
  uint64_t	xer;			/* Fixed Point Exception Register  */
  uint64_t	lr;			/* Link Register */
  uint64_t	ctr;			/* Control Register */
  uint64_t	pc;			/* Program Counter */
  uint64_t	fpr[PPU_FPR_NUM];	/* Floating Point Register FPR[0-31] */
  uint32_t	fpscr;			/* Floating Point Status and Control Register */
  sys_dbg_vr_t	vr[PPU_VR_NUM];		/* Vector Register VR[0-31]*/
  sys_dbg_vr_t	vscr;			/* Vector Status and Control Register */
} sys_dbg_ppu_thread_context_t;


/* Type for SPU GPR registers */
typedef union {
  uint8_t	byte[SPU_GPR_WIDTH];
  uint16_t	halfword[SPU_GPR_WIDTH/sizeof(uint16_t)];
  uint32_t	word[SPU_GPR_WIDTH/sizeof(uint32_t)];
  uint64_t	dw[SPU_GPR_WIDTH/sizeof(uint64_t)];
} sys_dbg_spu_gpr_t;

/* Type for SPU FPSCR register */
typedef union {
  uint8_t	byte[SPU_FPSCR_WIDTH];
  uint16_t	halfword[SPU_FPSCR_WIDTH/sizeof(uint16_t)];
  uint32_t	word[SPU_FPSCR_WIDTH/sizeof(uint32_t)];
  uint64_t	dw[SPU_FPSCR_WIDTH/sizeof(uint64_t)];
} sys_dbg_spu_fpscr_t;

/* Data structure for SPU thread context (obsolete) */
typedef struct {
  sys_dbg_spu_gpr_t	gpr[SPU_GPR_NUM];		/* GPR[0-127]*/
  uint32_t		npc;				/* Next Program Counter */
  uint32_t		fpscr;				/* FPSCR */
  uint32_t		srr0;				/* SRR0 */
  uint32_t		spu_status;			/* SPU STATUS */
  uint64_t		spu_cfg;			/* SPU CFG */
  uint32_t		mb_stat;			/* MB_stat */
  uint32_t		ppu_mb;				/* PPU Mail Box */
  uint32_t		spu_mb[SPU_MB_NUM];		/* SPU Mail Box */
  uint32_t		decrementer;			/* Decrementer */
  uint64_t		mfc_cq_sr[SPU_MFC_CQ_SR_NUM];	/* MFC Command Queue Save/Restore Register */
} sys_dbg_spu_thread_context_t;

/* Data structure for SPU thread context */
typedef struct {
  sys_dbg_spu_gpr_t	gpr[SPU_GPR_NUM];		/* GPR[0-127]*/
  uint32_t		npc;				/* Next Program Counter */
  sys_dbg_spu_fpscr_t	fpscr;				/* FPSCR */
  uint32_t		srr0;				/* SRR0 */
  uint32_t		spu_status;			/* SPU STATUS */
  uint64_t		spu_cfg;			/* SPU CFG */
  uint32_t		mb_stat;			/* MB_stat */
  uint32_t		ppu_mb;				/* PPU Mail Box */
  uint32_t		spu_mb[SPU_MB_NUM];		/* SPU Mail Box */
  uint32_t		decrementer;			/* Decrementer */
  uint64_t		mfc_cq_sr[SPU_MFC_CQ_SR_NUM];	/* MFC Command Queue Save/Restore Register */
} sys_dbg_spu_thread_context2_t;

/* Mutex information */
typedef struct {
  sys_mutex_attribute_t	attr;
  sys_ppu_thread_t	owner;
  int			lock_counter;
  int			cond_ref_counter;
  sys_cond_t		cond_id;		// Always Zero (0) in this field
  sys_ppu_thread_t*	wait_id_list;
  uint32_t		wait_threads_num;
  uint32_t		wait_all_threads_num;
} sys_dbg_mutex_information_t;

/* Condition Variable information */
typedef struct {
  sys_cond_attribute_t	attr;
  sys_mutex_t		mutex_id;
  sys_ppu_thread_t*	wait_id_list;
  uint32_t		wait_threads_num;
  uint32_t		wait_all_threads_num;
} sys_dbg_cond_information_t;

/* RWLock information */
typedef struct {
  sys_rwlock_attribute_t	attr;
  sys_ppu_thread_t		owner;
  sys_ppu_thread_t*		r_wait_id_list;
  uint32_t			r_wait_threads_num;
  uint32_t			r_wait_all_threads_num;
  sys_ppu_thread_t*		w_wait_id_list;
  uint32_t			w_wait_threads_num;
  uint32_t			w_wait_all_threads_num;
} sys_dbg_rwlock_information_t;

/* Event Queue information */
typedef struct {
  sys_event_queue_attribute_t	attr;
  sys_ipc_key_t			event_queue_key;
  int				queue_size;
  sys_ppu_thread_t*		wait_id_list;
  uint32_t			wait_threads_num;
  uint32_t			wait_all_threads_num;
  sys_event_t*			equeue_list;
  uint32_t			readable_equeue_num;
  uint32_t			readable_all_equeue_num;
} sys_dbg_event_queue_information_t;

/* Semaphore information */
typedef struct {
  sys_semaphore_attribute_t	attr;
  sys_semaphore_value_t		max_val;
  sys_semaphore_value_t		cur_val;
  sys_ppu_thread_t*		wait_id_list;
  uint32_t			wait_threads_num;
  uint32_t			wait_all_threads_num;
} sys_dbg_semaphore_information_t;

/* LWMutex information */
typedef struct {
  sys_lwmutex_attribute_t	attr;
  sys_ppu_thread_t		owner;
  int				lock_counter;
  sys_ppu_thread_t*		wait_id_list;
  uint32_t			wait_threads_num;
  uint32_t			wait_all_threads_num;
} sys_dbg_lwmutex_information_t;

/* Lightweight Condition Variable information */
typedef struct {
  sys_lwcond_attribute_t	attr;
  sys_lwmutex_t*		lwmutex;
  sys_ppu_thread_t*		wait_id_list;
  uint32_t			wait_threads_num;
  uint32_t			wait_all_threads_num;
} sys_dbg_lwcond_information_t;

/* Event Flag information */
typedef struct {
  uint64_t	bitptn;
  uint32_t	mode;
} sys_dbg_event_flag_wait_information_t;

typedef struct {
  sys_event_flag_attribute_t			attr;
  uint64_t					cur_bitptn;
  sys_ppu_thread_t*				wait_id_list;
  sys_dbg_event_flag_wait_information_t*	wait_info_list;
  uint32_t					wait_threads_num;
  uint32_t					wait_all_threads_num;
} sys_dbg_event_flag_information_t;


/* Read ppu thread registers from the target PPU thread */
CDECL int sys_dbg_read_ppu_thread_context(sys_ppu_thread_t id,
                                          sys_dbg_ppu_thread_context_t* ppu_context);

/* Read spu thread registers from the target SPU thread (obsolete) */
CDECL int sys_dbg_read_spu_thread_context(sys_spu_thread_t id,
                                          sys_dbg_spu_thread_context_t* spu_context);

/* Read spu thread registers from the target SPU thread */
CDECL int sys_dbg_read_spu_thread_context2(sys_spu_thread_t id,
                                           sys_dbg_spu_thread_context2_t* spu_context);

/* Type of PPU Exception Handler */
typedef void (*dbg_exception_handler_t)(uint64_t, sys_ppu_thread_t, uint64_t);	/* exception type, id, dar */

/* Set stacksize for PPU exception handling thread (NOT thread safe) */
CDECL int sys_dbg_set_stacksize_ppu_exception_handler(size_t stacksize);


/* Initialize PPU exception handler (NOT thread safe) */
CDECL int sys_dbg_initialize_ppu_exception_handler(int prio);
          
          
/* Finalize PPU exception handler (NOT thread safe) */
CDECL int sys_dbg_finalize_ppu_exception_handler(void);
          

/* Register PPU exception handler */
CDECL int sys_dbg_register_ppu_exception_handler(dbg_exception_handler_t callback,
                                                 uint64_t ctrl_flags);

/* Unregister PPU exception handler */
CDECL int sys_dbg_unregister_ppu_exception_handler(void);


/* Wakeup the PPU exception handler */
CDECL int sys_dbg_signal_to_ppu_exception_handler(uint64_t flags);


/*
 * Get information for some synchronization primitives 
 *
 */


typedef _sys_sleep_queue_t sys_lwmutex_pseudo_id_t;
typedef _sys_lwcond_queue_t sys_lwcond_pseudo_id_t;


/* Get mutex information */
CDECL int sys_dbg_get_mutex_information(sys_mutex_t id,
                                        sys_dbg_mutex_information_t* info);



/* Get condition variable information */
CDECL int sys_dbg_get_cond_information(sys_cond_t id,
                                       sys_dbg_cond_information_t* info);



/* Get RWLock information */
CDECL int sys_dbg_get_rwlock_information(sys_rwlock_t id,
                                         sys_dbg_rwlock_information_t* info);



/* Get event queue information */
CDECL int sys_dbg_get_event_queue_information(sys_event_queue_t id,
                                              sys_dbg_event_queue_information_t* info);



/* Get semaphore information */
CDECL int sys_dbg_get_semaphore_information(sys_semaphore_t id,
                                            sys_dbg_semaphore_information_t* info);


/* Get lwmutex information */
CDECL int sys_dbg_get_lwmutex_information(sys_lwmutex_pseudo_id_t id,
                                          sys_dbg_lwmutex_information_t* info);

/* Get lwcond information */
CDECL int sys_dbg_get_lwcond_information(sys_lwcond_pseudo_id_t id,
                                         sys_dbg_lwcond_information_t* info);

/* Get event flag information */
CDECL int sys_dbg_get_event_flag_information(sys_event_flag_t id,
                                             sys_dbg_event_flag_information_t* info);

/* Get PPU thread ID list */
CDECL int sys_dbg_get_ppu_thread_ids(sys_ppu_thread_t* ids,
                                     uint64_t* ids_num,
                                     uint64_t* all_ids_num);


/* Get SPU thread group ID list */
CDECL int sys_dbg_get_spu_thread_group_ids(sys_spu_thread_group_t* ids,
                                           uint64_t* ids_num,
                                           uint64_t* all_ids_num);


/* Get SPU thread ID list */
CDECL int sys_dbg_get_spu_thread_ids(sys_spu_thread_group_t spu_thread_group_id,
                                     sys_spu_thread_t* ids,
                                     uint64_t* ids_num,
                                     uint64_t* all_ids_num);

/* Get PPU thread name */
CDECL int sys_dbg_get_ppu_thread_name(sys_ppu_thread_t id, char* name);

/* Get SPU thread name */
CDECL int sys_dbg_get_spu_thread_name(sys_spu_thread_t id, char* name);

/* Get SPU thread group name */
CDECL int sys_dbg_get_spu_thread_group_name(sys_spu_thread_group_t id, char* name);


/**
 * The status of PPU thread
 */
typedef enum {
  PPU_THREAD_STATUS_IDLE,		/* IDLE */
  PPU_THREAD_STATUS_RUNNABLE,		/* RUNNABLE */
  PPU_THREAD_STATUS_ONPROC,		/* ONPROC*/
  PPU_THREAD_STATUS_SLEEP,		/* SLEEP */
  PPU_THREAD_STATUS_STOP,		/* STOP */
  PPU_THREAD_STATUS_ZOMBIE,		/* ZOMBIE */
  PPU_THREAD_STATUS_DELETED,		/* DELETED */
  PPU_THREAD_STATUS_UNKNOWN		/* unknown */
} sys_dbg_ppu_thread_status_t;


/**
 * The status of SPU thread group
 */
typedef enum {
  SPU_THREAD_GROUP_STATUS_NOT_INITIALIZED,		/* NOT INITIALIZED */
  SPU_THREAD_GROUP_STATUS_INITIALIZED,			/* INITIALIZED */
  SPU_THREAD_GROUP_STATUS_READY,			/* READY */
  SPU_THREAD_GROUP_STATUS_WAITING,			/* WAITING */
  SPU_THREAD_GROUP_STATUS_SUSPENDED,			/* SUSPENDED */
  SPU_THREAD_GROUP_STATUS_WAITING_AND_SUSPENDED,	/* WAINTNG AND SUSPENDED */
  SPU_THREAD_GROUP_STATUS_RUNNING,			/* RUNNING */
  SPU_THREAD_GROUP_STATUS_STOPPED,			/* STOPPED */
  SPU_THREAD_GROUP_STATUS_UNKNOWN			/* unknown */
} sys_dbg_spu_thread_group_status_t;



/* Get a status of the target PPU thread */
CDECL int sys_dbg_get_ppu_thread_status(sys_ppu_thread_t id,
                                        sys_dbg_ppu_thread_status_t* status);

/* Get a status of the target SPU thread group  */
CDECL int sys_dbg_get_spu_thread_group_status(sys_spu_thread_group_t id,
                                              sys_dbg_spu_thread_group_status_t* status);



/* Enable precise mode (MSR[FE0]==1 and MSR[FE1]==1) to the target PPU thread in order to handle floating point enabled exception */
CDECL int sys_dbg_enable_floating_point_enabled_exception(sys_ppu_thread_t id, uint64_t flags, uint64_t opt1, uint64_t opt2);

/* Disable precise mode to the target PPU thread in order to set to default mode: Ignore Exception (MSR[FE0]==0 and MSR[FE1]==0) */
CDECL int sys_dbg_disable_floating_point_enabled_exception(sys_ppu_thread_t id, uint64_t flags, uint64_t opt1, uint64_t opt2);


/* Get information of the pages in a VM */
#define SYS_VM_STATE_LOCKED                     0x0008ULL
#define SYS_VM_STATE_DIRTY                      0x0010ULL
struct sys_vm_page_information {
    uint64_t state;
};
typedef struct sys_vm_page_information sys_vm_page_information_t;
CDECL int sys_dbg_vm_get_page_information(sys_addr_t addr, unsigned int num,
                                          sys_vm_page_information_t *pageinfo);


/* DABR Control Flags */
#define SYS_DBG_DABR_CTRL_READ       0x0000000000000005ULL   /* DABR match will be occured when the specified address was accessed by LOAD instructions */
#define SYS_DBG_DABR_CTRL_WRITE      0x0000000000000006ULL   /* DABR match will be occured when the specified address was accessed by STORE instructions */
#define SYS_DBG_DABR_CTRL_CLEAR      0x0000000000000000ULL   /* Clear DABR match setting */

/* Set DABR */
CDECL int sys_dbg_set_address_to_dabr(uint64_t addr, uint64_t ctrl_flag);

/* Get DABR */
CDECL int sys_dbg_get_address_from_dabr(uint64_t* addr, uint64_t* ctrl_flag);


/* Wakeup the coredump handler  */
CDECL int sys_dbg_signal_to_coredump_handler(uint64_t data1, uint64_t data2, uint64_t data3);

/* MAT condition flags */
#define SYS_DBG_MAT_TRANSPARENT         1 /* Do not trap */
#define SYS_DBG_MAT_WRITE               2 /* Trap write access */
#define SYS_DBG_MAT_READ_WRITE          4 /* Trap read and write */

 /* MAT granularity.  THe address must be aligned on the value */
#define SYS_MAT_GRANULARITY         4096

CDECL int sys_dbg_mat_set_condition(sys_addr_t addr, uint64_t cond);
CDECL int sys_dbg_mat_get_condition(sys_addr_t addr, uint64_t *condp);


/**
 * The status of coredump function
 */
typedef enum {
  SYS_DBG_COREDUMP_OFF,
  SYS_DBG_COREDUMP_ON_SAVE_TO_APP_HOME,
  SYS_DBG_COREDUMP_ON_SAVE_TO_DEV_MS,
  SYS_DBG_COREDUMP_ON_SAVE_TO_DEV_USB,
  SYS_DBG_COREDUMP_ON_SAVE_TO_DEV_HDD0
} sys_dbg_coredump_parameter_t;

/* Get some parameters of coredump handler */
CDECL int sys_dbg_get_coredump_params(sys_dbg_coredump_parameter_t* param);

/* Set PPU exception mask to the exception handler */
CDECL int sys_dbg_set_mask_to_ppu_exception_handler(uint64_t mask, uint64_t flags);

#endif
/*@}*/
