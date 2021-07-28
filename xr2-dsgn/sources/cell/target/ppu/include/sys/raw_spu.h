/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

/** \ingroup all_syscall
 * \defgroup raw_spu Raw SPUs
 * \brief system calls defined in sys/raw_spu.h
 * 
 * Interfaces defined in sys/raw_spu.h
 *
 * Refer to <i>Cell OS Lv-2 User's Manual</i> for the detailed description. 
 */

/* A Raw SPU is an executable entity that, from its creation to termination, 
 * keeps the allocated SPUs while the Lv2 OS LPAR is scheduled by the Cell OS.
 * By creating a Raw SPU, the user can directly access SPU resrouces from each 
 * device that can physically access SPU programs, PU threads, or SPU 
 * resources.
 * 
 * <b><Accessibility></b> \n
 * <b>MMIO</b>\n
 * Creating a Raw SPU, the user has wider accessibility to the resources on a 
 * physical SPU compared to an SPU Thread. 
 * From the PU and other SPUs, the user can access to the local storage and 
 * all Problem State Memory on a Raw SPU, using MMIO(Memory-Mapped I/O) via 
 * load/store instructions.  Once a Raw SPU is created, its resources are 
 * mapped to the process address space.  The location of mapped addresses are
 * statically defined as follows.
 *     
 * \code
 * Local Storage = RAW_SPU_OFFSET * Raw SPU ID + RAW_SPU_BASE_ADDR + RAW_SPU_LS_OFFSET
 * Problem       = RAW_SPU_OFFSET * Raw SPU ID + RAW_SPU_BASE_ADDR + RAW_SPU_PROB_OFFSET
 * \endcode
 *
 * where,
 *
 * \code
 * RAW_SPU_OFFSET      = 0x0000000000100000
 * RAW_SPU_BASE_ADDR   = 0x00000000E0000000
 * RAW_SPU_LS_OFFSET   = 0x0000000000000000
 * RAW_SPU_PROB_OFFSET = 0x0000000000040000
 * \endcode
 *
 * All effective addresses to which the Problem State Memory is mapped to can
 * be determined by the above formula with an offset which is statically 
 * assigned to each resource.  (See Broadband Engine Functional Registers for DD 1.0 Table 3-3.)
 * For example, SPU_RunCntl has an offset value 0x401C.  Therefore, the 
 * effective address to which SPU_RunCntl is mapped on the Raw SPU of ID 3 is 
 * calculated as follows.
 * \code
 * RAW_SPU_OFFSET * 3 + RAW_SPU_BASE_ADDR + RAW_SPU_PROB_OFFSET + 0x401C
 * \endcode
 *
 * For the details of each Problem State Memory register, refer to BE Books. 
 * 
 * <b>System Calls</b>\n
 * Some of the resrouces on SPUs are accessible via the provided system calls
 * as listed below.
 *  - All channel mapped registers 
 *  - SPU Configuration Registers
 *  - PU Interrupt Mailbox
 *  - Interrupt-Status Registers 
 *  - Interrupt-Mask Registers 
 * 
 * <b><Interrupt></b> \n
 * The interrupts occured in MFC are classified into three categories (error,
 * translation, application). Cell Lv2 OS provides system calls to create a
 * tag for the interrupts of each class, and to attach to an interrupt thread.
 * When an interrupt is thrown by the MFC, the interrupt PU thread is waken up
 * to execute the code loaded to the interrupt thread in advance.  
 * 
 * By establishing such interrupt thread, the PU can do its own tasks without 
 * waiting by blocking for asychronous signals from the SPUs.  
 * Only when such asychronous signal from an SPU is detected, the interrupt 
 * thread is waken up to handle the signal.  
 *
 * <b><Usage></b> \n
 * <b>Accessing Resources on SPUs</b>
 * The most of communication between the PU and SPU and the control on the SPU
 * are available by controlling the Problem State Memory registers via MMIO
 * accesses.  The following lists the specific tasks
 *    - DMA read/write
 *    - Wait for the completion of specific DMA tag groups
 *    - PU Mailbox (PPC Core Mailbox)
 *    - SPU Mailbox
 *    - SPU run control (start/stop etc.)
 *    - Checking the current SPU status. 
 *    - Set the program counter
 *    - Accessing the local storage
 * 
 * The channel count value associate with each registers are transitioned on
 * every MMIO access as defined in the specification.  For example, writing 
 * to the SPU Mailbox automatically increment its channel count value.  
 * 
 * Some of the tasks requires to access multiple registers.  For instance,
 * complete a DMA, the local storage address, the effective address, the DMA
 * size, the DMA tag need be set to each individual registers.  Finally, 
 * setting a DMA command to DMA Command Opcode enqueues the DMA request. 
 *
 * The chances to use raw_spu_channel_write() and raw_spu_channel_read() are 
 * more limited because most of the channel-mapped registers are accessible via
 * MMIO and the system calls. 
 *  These system calls are expected to be used when the user want to save
 * the context of the channels, instead of communication with SPUs.  
 *
 * PU Interrupt Mailbox and SPU Configuration Registers are accessible only
 * via the provided system calls.
 *
 * <b>Create and Establish an SPU Interrupt Tag</b>
 * An interrupt thread is a PU thread that is waken up only when the interrupt 
 * is caught in order to handle the interrupt.  
 * 
 * A typical sequence to create an interrupt thread:
 *  -# Create an interrupt tag associate with an interrupt class
 *        - sys_raw_spu_create_interrupt_tag()  
 *  -# Create a PU thread with SYS_PPU_THREAD_CREATE_INTERRUPT flag.  
 *        - pu_thread_create()
 *  -# Establish the interrupt thread with the tag.
 *        - intr_thread_establish()
 *
 * And, the interrupt thread should handle the interrupts in the following 
 * manner.
 *  -# If more than one kind of interrupt is expected, read the SPU Interrupt
 *     Status Regsiter, and determine which interrupt is caught.
 *         - sys_raw_spu_get_int_stat()
 *  -# Handle the interrupt.
 *  -# Reset the SPU Interrupt Status Register.
 *         - sys_raw_spu_set_int_stat()
 *  -# Ends the interrupt thread.
 *         - sys_interrupt_thread_eoi()
 *
 * See samp_raw_spu02.cc for the detailed implementation of an interrupt 
 * thread.
 */
/*@{*/

#ifndef __SYS_SYS_RAW_SPU_H__
#define __SYS_SYS_RAW_SPU_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>

#include <sys/integertypes.h>
#include <sys/return_code.h>

#include <sys/interrupt.h>
#include <sys/fixed_addr.h>


CDECL_BEGIN

typedef __CSTD uint32_t sys_class_id_t;

#define RAW_SPU_OFFSET        0x00100000UL
#define RAW_SPU_LS_OFFSET     0x00000000UL
#define RAW_SPU_PROB_OFFSET   0x00040000UL

typedef int sys_raw_spu_t;
typedef void sys_raw_spu_attribute_t;

extern inline int sys_raw_spu_create(sys_raw_spu_t * id,
									 sys_raw_spu_attribute_t * attr);

extern inline int sys_raw_spu_create(sys_raw_spu_t * id,
									 sys_raw_spu_attribute_t * attr)
{
	system_call_2(SYS_RAW_SPU_CREATE, (uint32_t) id, (uint32_t) attr);

#ifdef REG_PASS_SYS_RAW_SPU_CREATE
	REG_PASS_SYS_RAW_SPU_CREATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_destroy(sys_raw_spu_t id);

extern inline int sys_raw_spu_destroy(sys_raw_spu_t id)
{
	system_call_1(SYS_RAW_SPU_DESTROY, id);

#ifdef REG_PASS_SYS_RAW_SPU_DESTROY
	REG_PASS_SYS_RAW_SPU_DESTROY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_create_interrupt_tag(sys_raw_spu_t id,
												   sys_class_id_t class_id,
												   sys_hw_thread_t hwthread,
												   sys_interrupt_tag_t *
												   intrtag);

extern inline int sys_raw_spu_create_interrupt_tag(sys_raw_spu_t id,
												   sys_class_id_t class_id,
												   sys_hw_thread_t hwthread,
												   sys_interrupt_tag_t *
												   intrtag)
{
	system_call_4(SYS_RAW_SPU_CREATE_INTERRUPT_TAG, id, class_id, hwthread,
				  (uint32_t) intrtag);

#ifdef REG_PASS_SYS_RAW_SPU_CREATE_INTERRUPT_TAG
	REG_PASS_SYS_RAW_SPU_CREATE_INTERRUPT_TAG;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_set_int_mask(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t mask);

extern inline int sys_raw_spu_set_int_mask(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t mask)
{
	system_call_3(SYS_RAW_SPU_SET_INT_MASK, id, class_id, mask);

#ifdef REG_PASS_SYS_RAW_SPU_SET_INT_MASK
	REG_PASS_SYS_RAW_SPU_SET_INT_MASK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_get_int_mask(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t * mask);

extern inline int sys_raw_spu_get_int_mask(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t * mask)
{
	system_call_3(SYS_RAW_SPU_GET_INT_MASK, id, class_id, (uint32_t) mask);

#ifdef REG_PASS_SYS_RAW_SPU_GET_INT_MASK
	REG_PASS_SYS_RAW_SPU_GET_INT_MASK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_set_int_stat(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t stat);

extern inline int sys_raw_spu_set_int_stat(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t stat)
{
	system_call_3(SYS_RAW_SPU_SET_INT_STAT, id, class_id, stat);

#ifdef REG_PASS_SYS_RAW_SPU_SET_INT_STAT
	REG_PASS_SYS_RAW_SPU_SET_INT_STAT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_get_int_stat(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t * stat);

extern inline int sys_raw_spu_get_int_stat(sys_raw_spu_t id,
										   sys_class_id_t class_id,
										   __CSTD uint64_t * stat)
{
	system_call_3(SYS_RAW_SPU_GET_INT_STAT, id, class_id, (uint32_t) stat);

#ifdef REG_PASS_SYS_RAW_SPU_GET_INT_STAT
	REG_PASS_SYS_RAW_SPU_GET_INT_STAT;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_read_puint_mb(sys_raw_spu_t id,
											__CSTD uint32_t * value);

extern inline int sys_raw_spu_read_puint_mb(sys_raw_spu_t id,
											__CSTD uint32_t * value)
{
	system_call_2(SYS_RAW_SPU_READ_PUINT_MB, id, (uint32_t) value);

#ifdef REG_PASS_SYS_RAW_SPU_READ_PUINT_MB
	REG_PASS_SYS_RAW_SPU_READ_PUINT_MB;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_set_spu_cfg(sys_raw_spu_t id,
										  __CSTD uint32_t value);

extern inline int sys_raw_spu_set_spu_cfg(sys_raw_spu_t id,
										  __CSTD uint32_t value)
{
	system_call_2(SYS_RAW_SPU_SET_SPU_CFG, id, value);

#ifdef REG_PASS_SYS_RAW_SPU_SET_SPU_CFG
	REG_PASS_SYS_RAW_SPU_SET_SPU_CFG;
#endif
	return_to_user_prog(int);
}

extern inline int sys_raw_spu_get_spu_cfg(sys_raw_spu_t id,
										  __CSTD uint32_t * value);

extern inline int sys_raw_spu_get_spu_cfg(sys_raw_spu_t id,
										  __CSTD uint32_t * value)
{
	system_call_2(SYS_RAW_SPU_GET_SPU_CFG, id, (uint32_t) value);

#ifdef REG_PASS_SYS_RAW_SPU_GET_SPU_CFG
	REG_PASS_SYS_RAW_SPU_GET_SPU_CFG;
#endif
	return_to_user_prog(int);
}


extern inline int sys_raw_spu_recover_page_fault(sys_raw_spu_t id);

extern inline int sys_raw_spu_recover_page_fault(sys_raw_spu_t id)
{
	system_call_1(SYS_RAW_SPU_RECOVER_PAGE_FAULT, id);

#ifdef REG_PASS_SYS_RAW_SPU_RECOVER_PAGE_FAULT
	REG_PASS_SYS_RAW_SPU_RECOVER_PAGE_FAULT;
#endif
	return_to_user_prog(int);
}




/* Macros to calculate base addresses with the given Raw SPU ID */
#define LS_BASE_ADDR(id) \
(RAW_SPU_OFFSET * id + RAW_SPU_BASE_ADDR + RAW_SPU_LS_OFFSET)
#define PROB_BASE_ADDR(id) \
(RAW_SPU_OFFSET * id + RAW_SPU_BASE_ADDR + RAW_SPU_PROB_OFFSET)


/* Problem State Memory Map Register Offset */
#define MFC_LSA           0x3004U
#define MFC_EAH           0x3008U
#define MFC_EAL           0x300CU
#define MFC_Size_Tag      0x3010U
#define MFC_Class_CMD     0x3014U
#define MFC_CMDStatus     0x3014U
#define MFC_QStatus       0x3104U
#define Prxy_QueryType    0x3204U
#define Prxy_QueryMask    0x321CU
#define Prxy_TagStatus    0x322CU
#define SPU_Out_MBox      0x4004U
#define SPU_In_MBox       0x400CU
#define SPU_MBox_Status   0x4014U
#define SPU_RunCntl       0x401CU
#define SPU_Status        0x4024U
#define SPU_NPC           0x4034U
#define SPU_Sig_Notify_1  0x1400CU
#define SPU_Sig_Notify_2  0x1C00CU


/* A macro to obtain the effective address of a problem state memory map 
 * register with the given Raw SPU ID and register offset. 
 *   Usage: sys_addr_t addr = get_reg_addr(1, PU_MB);
 *
 *   \param   id       Raw SPU ID
 *   \param   offset   Register offset
 *   \return  Effective address of the register
 */
#define get_reg_addr(id, offset) \
(volatile uintptr_t)(PROB_BASE_ADDR(id) + offset)


/* A function to write to a problem state memory map register specified by
 * the given Raw SPU ID and register offset.
 *   Usage: mmio_write_prob_reg(1, SPU_MB, 0x1);
 *
 *   \param   id       Raw SPU ID
 *   \param   offset   Register Offset
 *   \param   value    Value to write 
 */
extern inline void sys_raw_spu_mmio_write(int id, int offset, uint32_t value);

extern inline void sys_raw_spu_mmio_write(int id, int offset, uint32_t value)
{
	*(volatile uint32_t *)get_reg_addr(id, offset) = value;
}

/* A function to read from a problem state memory map register specifid by 
 * the given Raw SPU ID and register offset.  
 *   Usage: uint32_t val = read_reg(1, PU_MB);
 *
 *   \param   id       Raw SPU ID
 *   \param   offset   Register Offset
 *   \return  The register value
 */
extern inline uint32_t sys_raw_spu_mmio_read(int id, int offset);

extern inline uint32_t sys_raw_spu_mmio_read(int id, int offset)
{
	return *(volatile uint32_t *)get_reg_addr(id, offset);
}


/* A macro to obtain the effective address of a local storage address
 * with the given Raw SPU ID and local storage address. 
 *   Usage: sys_addr_t addr = get_ls_addr(1, 0x80U);
 *
 *   \param   id       Raw SPU ID
 *   \param   offset   local storage address
 *   \return  Effective address of the local storage
 */
#define get_ls_addr(id, offset) \
(volatile uintptr_t)(LS_BASE_ADDR(id) + offset)


/* A function to write to a local storage specified by the given Raw SPU ID
 * and local storage address.
 *   Usage: mmio_write_ls(1, 0x80U, 0x12345678U);
 *
 *   \param   id       Raw SPU ID
 *   \param   offset   Register Offset
 *   \param   value    Value to write 
 */
extern inline void
sys_raw_spu_mmio_write_ls(int id, int offset, uint32_t value);

extern inline void sys_raw_spu_mmio_write_ls(int id, int offset, uint32_t value)
{
	*(volatile uint32_t *)get_ls_addr(id, offset) = value;
}

/* A function to read from a local storage specifid by the given Raw SPU ID
 * and local storage address
 *   Usage: uint32_t val = mmio_read_ls(1, 0x80U);
 *
 *   \param   id       Raw SPU ID
 *   \param   offset   Register Offset
 *   \return  The loacal storage value
 */
extern inline uint32_t sys_raw_spu_mmio_read_ls(int id, int offset);

extern inline uint32_t sys_raw_spu_mmio_read_ls(int id, int offset)
{
	return *(volatile uint32_t *)get_ls_addr(id, offset);
}


/*@}*/

CDECL_END

#endif /* __SYS_SYS_RAW_SPU_H__ */
