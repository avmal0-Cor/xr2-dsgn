/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SYS_SYS_VM_H__
#define __SYS_SYS_VM_H__

#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <sys/integertypes.h>
#include <sys/return_code.h>
#include <sys/types.h>

CDECL_BEGIN

#define SYS_VM_STATE_INVALID                    0x0000ULL
#define SYS_VM_STATE_UNUSED                     0x0001ULL
#define SYS_VM_STATE_ON_MEMORY                  0x0002ULL
#define SYS_VM_STATE_STORED                     0x0004ULL



#define SYS_VM_POLICY_AUTO_RECOMMENDED    1UL

extern inline int sys_vm_memory_map(size_t vsize, size_t psize,
									sys_memory_container_t container,
									uint64_t flag, uint64_t policy,
									sys_addr_t * addr);

extern inline int sys_vm_memory_map(size_t vsize, size_t psize,
									sys_memory_container_t container,
									uint64_t flag, uint64_t policy,
									sys_addr_t * addr)
{
	system_call_6(SYS_VM_MEMORY_MAP, vsize, psize, container, flag, policy,
				  (uint32_t) addr);

#ifdef REG_PASS_SYS_VM_MEMORY_MAP
	REG_PASS_SYS_VM_MEMORY_MAP;
#endif
	return_to_user_prog(int);
}



extern inline int sys_vm_unmap(sys_addr_t addr);

extern inline int sys_vm_unmap(sys_addr_t addr)
{
	system_call_1(SYS_VM_UNMAP, addr);

#ifdef REG_PASS_SYS_VM_UNMAP
	REG_PASS_SYS_VM_UNMAP;
#endif
	return_to_user_prog(int);
}


extern inline int sys_vm_append_memory(sys_addr_t addr, size_t size);

extern inline int sys_vm_append_memory(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_APPEND_MEMORY, addr, size);

#ifdef REG_PASS_SYS_VM_APPEND_MEMORY
	REG_PASS_SYS_VM_APPEND_MEMORY;
#endif
	return_to_user_prog(int);
}

extern inline int sys_vm_return_memory(sys_addr_t addr, size_t size);

extern inline int sys_vm_return_memory(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_RETURN_MEMORY, addr, size);

#ifdef REG_PASS_SYS_VM_RETURN_MEMORY
	REG_PASS_SYS_VM_RETURN_MEMORY;
#endif
	return_to_user_prog(int);
}


extern inline int sys_vm_lock(sys_addr_t addr, size_t size);

extern inline int sys_vm_lock(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_LOCK, addr, size);

#ifdef REG_PASS_SYS_VM_LOCK
	REG_PASS_SYS_VM_LOCK;
#endif
	return_to_user_prog(int);
}

extern inline int sys_vm_unlock(sys_addr_t addr, size_t size);

extern inline int sys_vm_unlock(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_UNLOCK, addr, size);

#ifdef REG_PASS_SYS_VM_UNLOCK
	REG_PASS_SYS_VM_UNLOCK;
#endif
	return_to_user_prog(int);
}


extern inline int sys_vm_touch(sys_addr_t addr, size_t size);

extern inline int sys_vm_touch(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_TOUCH, addr, size);

#ifdef REG_PASS_SYS_VM_TOUCH
	REG_PASS_SYS_VM_TOUCH;
#endif
	return_to_user_prog(int);
}

extern inline int sys_vm_flush(sys_addr_t addr, size_t size);

extern inline int sys_vm_flush(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_FLUSH, addr, size);

#ifdef REG_PASS_SYS_VM_FLUSH
	REG_PASS_SYS_VM_FLUSH;
#endif
	return_to_user_prog(int);
}

extern inline int sys_vm_invalidate(sys_addr_t addr, size_t size);

extern inline int sys_vm_invalidate(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_INVALIDATE, addr, size);

#ifdef REG_PASS_SYS_VM_INVALIDATE
	REG_PASS_SYS_VM_INVALIDATE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_vm_store(sys_addr_t addr, size_t size);

extern inline int sys_vm_store(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_STORE, addr, size);

#ifdef REG_PASS_SYS_VM_STORE
	REG_PASS_SYS_VM_STORE;
#endif
	return_to_user_prog(int);
}

extern inline int sys_vm_sync(sys_addr_t addr, size_t size);

extern inline int sys_vm_sync(sys_addr_t addr, size_t size)
{
	system_call_2(SYS_VM_SYNC, addr, size);

#ifdef REG_PASS_SYS_VM_SYNC
	REG_PASS_SYS_VM_SYNC;
#endif
	return_to_user_prog(int);
}

extern inline int sys_vm_test(sys_addr_t addr, size_t size, uint64_t * result);

extern inline int sys_vm_test(sys_addr_t addr, size_t size, uint64_t * result)
{
	system_call_3(SYS_VM_TEST, addr, size, (uint32_t) result);

#ifdef REG_PASS_SYS_VM_TEST
	REG_PASS_SYS_VM_TEST;
#endif
	return_to_user_prog(int);
}


struct sys_vm_statistics {
	__CSTD uint64_t page_fault_ppu;
	__CSTD uint64_t page_fault_spu;
	__CSTD uint64_t page_in;
	__CSTD uint64_t page_out;
	__CSTD size_t pmem_total;
	__CSTD size_t pmem_used;
	__CSTD uint64_t time;
};
typedef struct sys_vm_statistics sys_vm_statistics_t;

extern inline int sys_vm_get_statistics(sys_addr_t addr,
										sys_vm_statistics_t * stat);

extern inline int sys_vm_get_statistics(sys_addr_t addr,
										sys_vm_statistics_t * stat)
{
	system_call_2(SYS_VM_GET_STATISTICS, addr, (uint32_t) stat);

#ifdef REG_PASS_SYS_VM_GET_STATISTICS
	REG_PASS_SYS_VM_GET_STATISTICS;
#endif
	return_to_user_prog(int);
}





CDECL_END

#endif /* __SYS_SYS_VM_H__ */
