/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/* File: que.h
 * Description:
 *     resc fixed size command que template.
 */

#ifndef __RESC_QUE_H__
#define __RESC_QUE_H__

#include <stdint.h>

#include <sys/synchronization.h>

#include <cell/error.h>

namespace cell {
namespace Resc {

#define IN
#define OUT

#define CELL_RESC_QUEUE_FULL           -1
#define CELL_RESC_QUEUE_EMPTY          -2

// ============================================================================
// CQueue template class definition
// ============================================================================

template<typename T, int QUE_SIZE>
class CFixQueue
{
private:
	static int32_t s_pRead, s_pWrite;
	static T s_que[QUE_SIZE];

private:
	static inline int32_t _Next(int32_t n)
	{
		return (n + 1 + QUE_SIZE) % QUE_SIZE;
	}

	static inline int32_t _Prev(int32_t n)
	{
		return (n + - QUE_SIZE) % QUE_SIZE;
	}

public:
	typedef T value_type;
	//typedef A allocator_type; // no allocator

	typedef T* pointer;
	typedef T& reference;
	typedef const T* const_pointer;
	typedef const T& const_reference;

	class iterator
	{
	private:
		int32_t _Itr;

	public:
		iterator() : _Itr(0) {}
		iterator(int32_t pos) : _Itr(pos) {}
		iterator(const iterator &i) : _Itr(i._Itr) {}
		//~iterator() { }

		// pre increment
		inline iterator& operator++(void){
			_Itr = CFixQueue::_Next(_Itr);
			return (*this);
		}

		// post increment
		iterator operator++(int){
			iterator tmp = *this;
			_Itr = CFixQueue::_Next(_Itr);
			return tmp;
		}

		// pre decrement
		inline iterator& operator--(void){
			_Itr = CFixQueue::_Prev(_Itr);
			return (*this);
		}

		// post decrement
		iterator operator--(int){
			iterator tmp = *this;
			_Itr = CFixQueue::_Prev(_Itr);
			return tmp;
		}

		// dereference
		inline reference operator*(void) const {
			return s_que[_Itr];
		}

		inline pointer operator->(void) const {
			return &s_que[_Itr];
		}

		inline bool operator==(iterator& rhs) const {
			return (rhs._Itr == this->_Itr);
		}

		inline bool operator!=(iterator rhs) const {
			return !(*this == rhs);
		}

		};

public:
	CFixQueue() {};
	//~CFixQueue() {};

	static inline int32_t GetNumElements(void)
	{
		return (QUE_SIZE + s_pWrite - s_pRead) % QUE_SIZE;
	}

	static inline int32_t GetNumSpace(void)
	{
		return (QUE_SIZE - GetNumElements() - 1);
	}

	static inline int32_t GetCapacity(void)
	{
		return (QUE_SIZE - 1);
	}

	static inline void Clear(void)
	{
		s_pRead = s_pWrite = 0;
	}

	static inline iterator Begin(void)
	{
		return iterator(s_pRead);
	}
	
	static inline iterator End(void)
	{
		return iterator(s_pWrite);
	}

	/*static inline T& Front(void)
	{
		return s_que[s_pRead];
	}

	static inline T& Back(void)
	{
		return s_que[_Prev(s_pWrite)];
	}*/

	static inline bool IsEmpty(void)
	{
		return (s_pWrite == s_pRead);
	}

	// 'Enq' returns space after processing.
	static inline int32_t Enq(T& IN item)
	{
		int32_t pWriteNext = _Next(s_pWrite);

		if(pWriteNext == s_pRead){
			return CELL_RESC_QUEUE_FULL;
		} else {
			s_que[s_pWrite] = item;
			s_pWrite = pWriteNext;
			return GetNumSpace();
		}
	}

	// 'Deq' and 'Peek' returns rest after processing.
	static inline int32_t Deq(T& OUT item)
	{
		if(s_pWrite == s_pRead){
			return CELL_RESC_QUEUE_EMPTY;
		} else {
			item = s_que[s_pRead];
			s_pRead = _Next(s_pRead);
			return GetNumElements();
		}
	}

	// 'Deq' and 'Peek' returns rest after processing.
	static inline int32_t Peek(T& OUT item)
	{
		if(s_pWrite == s_pRead){
			return CELL_RESC_QUEUE_EMPTY;
		} else {
			item = s_que[s_pRead];
			//s_pRead = _Next(s_pRead);
			return GetNumElements();
		}
	}

	// access with semaphoe locking.
	static inline int32_t EnqLock(T& IN item, sys_lwmutex_t *lwmutex, int32_t time_out = SYS_NO_TIMEOUT)
	{
		int32_t ret = sys_lwmutex_lock(lwmutex, time_out);
		if(ret!=CELL_OK) return ret;
		ret = Enq(item);
		sys_lwmutex_unlock(lwmutex);
		return ret;
	}

	static inline int32_t DeqLock(T& OUT item, sys_lwmutex_t *lwmutex, int32_t time_out = SYS_NO_TIMEOUT)
	{
		int32_t ret = sys_lwmutex_lock(lwmutex, time_out);
		if(ret!=CELL_OK) return ret;
		ret = Deq(item);
		sys_lwmutex_unlock(lwmutex);
		return ret;
	}

	static inline int32_t PeekLock(T& OUT item, sys_lwmutex_t *lwmutex, int32_t time_out = SYS_NO_TIMEOUT)
	{
		int32_t ret = sys_lwmutex_lock(lwmutex, time_out);
		if(ret!=CELL_OK) return ret;
		ret = Peek(item);
		sys_lwmutex_unlock(lwmutex);
		return ret;
	}

};

template<typename T, int QUE_SIZE> int32_t CFixQueue<T, QUE_SIZE>::s_pRead = 0;
template<typename T, int QUE_SIZE> int32_t CFixQueue<T, QUE_SIZE>::s_pWrite = 0;
template<typename T, int QUE_SIZE> T CFixQueue<T, QUE_SIZE>::s_que[QUE_SIZE];


}; /* namespace Resc */
}; /* namespace cell */

#endif /* __RESC_QUE_H__ */

