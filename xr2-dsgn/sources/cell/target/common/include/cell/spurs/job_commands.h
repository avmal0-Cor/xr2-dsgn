/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SPURS_JOB_COMMANDS_H__
#define __CELL_SPURS_JOB_COMMANDS_H__

// Commands:
#define CELL_SPURS_JOB_OPCODE_NOP   0

// Bit 0,1,2 == 0 => Job
// Bit 0,1,2 != 0 => Commands (>= 8 byte aligned pointers only)
// Bit 0,1,2 all set => Extended commands (>= 128 byte aligned pointers only)
// Extended Command defined by bits 3,4,5,6

// 3-bit commands: (1..6 available; 0,7 reserved)
#define CELL_SPURS_JOB_OPCODE_RESET_PC     1
#define CELL_SPURS_JOB_OPCODE_SYNC         (2|(0 << 3))
#define CELL_SPURS_JOB_OPCODE_LWSYNC       (2|(2 << 3))
#define CELL_SPURS_JOB_OPCODE_SYNC_LABEL   (2|(1 << 3))
#define CELL_SPURS_JOB_OPCODE_LWSYNC_LABEL (2|(3 << 3))
#define CELL_SPURS_JOB_OPCODE_NEXT         3
#define CELL_SPURS_JOB_OPCODE_CALL         4
#define CELL_SPURS_JOB_OPCODE_FLUSH        5
#define CELL_SPURS_JOB_OPCODE_JOBLIST      6

// Extended commands: 0..15 available
#define CELL_SPURS_JOB_OPCODE_ABORT           (7|(0 << 3))
#define CELL_SPURS_JOB_OPCODE_GUARD           (7|(1 << 3))
#define CELL_SPURS_JOB_OPCODE_SET_LABEL       (7|(2 << 3))
#define CELL_SPURS_JOB_OPCODE_RET             (7|(14 << 3))
#define CELL_SPURS_JOB_OPCODE_END             (7|(15 << 3))

// Combined command
#define CELL_SPURS_JOB_OPCODE_JTS         (0x800000000ull|CELL_SPURS_JOB_OPCODE_LWSYNC)

/*
 *  COMMAND list
 */  
#define CELL_SPURS_JOB_COMMAND_NOP       CELL_SPURS_JOB_OPCODE_NOP
#define CELL_SPURS_JOB_COMMAND_JOB(ea)   ((uint64_t)(uintptr_t)(ea)&~7)
#define CELL_SPURS_JOB_COMMAND_JOBLIST(ea) (CELL_SPURS_JOB_OPCODE_JOBLIST|((uint64_t)(uintptr_t)(ea)&~7))
#define CELL_SPURS_JOB_COMMAND_RESET_PC(ea) (CELL_SPURS_JOB_OPCODE_RESET_PC|(((uint64_t)(uintptr_t)(ea))&~7))
#define CELL_SPURS_JOB_COMMAND_GUARD(ea) (CELL_SPURS_JOB_OPCODE_GUARD|(((uint64_t)(uintptr_t)(ea))&~127))
#define CELL_SPURS_JOB_COMMAND_SYNC      CELL_SPURS_JOB_OPCODE_SYNC
#define CELL_SPURS_JOB_COMMAND_LWSYNC    CELL_SPURS_JOB_OPCODE_LWSYNC
#define CELL_SPURS_JOB_COMMAND_SYNC_LABEL(label) (CELL_SPURS_JOB_OPCODE_SYNC_LABEL|((uint64_t)(label)<<7))
#define CELL_SPURS_JOB_COMMAND_LWSYNC_LABEL(label) (CELL_SPURS_JOB_OPCODE_LWSYNC_LABEL|((uint64_t)(label)<<7))
#define CELL_SPURS_JOB_COMMAND_NEXT(ea)  (CELL_SPURS_JOB_OPCODE_NEXT|(((uint64_t)(uintptr_t)(ea))&~7))
#define CELL_SPURS_JOB_COMMAND_CALL(ea)  (CELL_SPURS_JOB_OPCODE_CALL|(((uint64_t)(uintptr_t)(ea))&~7))
#define CELL_SPURS_JOB_COMMAND_FLUSH CELL_SPURS_JOB_OPCODE_FLUSH
#define CELL_SPURS_JOB_COMMAND_RET       CELL_SPURS_JOB_OPCODE_RET
#define CELL_SPURS_JOB_COMMAND_ABORT     CELL_SPURS_JOB_OPCODE_ABORT
#define CELL_SPURS_JOB_COMMAND_END       CELL_SPURS_JOB_OPCODE_END
#define CELL_SPURS_JOB_COMMAND_SET_LABEL(label) (CELL_SPURS_JOB_OPCODE_SET_LABEL|((uint64_t)(label)<<7))
#define CELL_SPURS_JOB_COMMAND_JTS       CELL_SPURS_JOB_OPCODE_JTS


#define cellSpursJobGetOpcode(c) (uint32_t)((c) & 7)
#define cellSpursJobGetOpcodeExt(c) (uint32_t)((c) & 127)
#define cellSpursJobIsJob(c) ((((c) & 7) == 0) && (c) != 0)
#define cellSpursJobIsCommandExt(c) (cellSpursJobGetOpcode(c) == 7)
#define cellSpursJobGetAddress8(c) ((c) & 0xFFFFFFFFfffffff8ull)
#define cellSpursJobGetAddress128(c) ((c) & 0xFFFFFFFFffffff80ull)


#endif /* __CELL_SPURS_JOB_COMMANDS_H__ */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
