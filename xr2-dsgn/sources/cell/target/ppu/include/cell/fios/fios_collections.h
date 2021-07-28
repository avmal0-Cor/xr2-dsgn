/*
	SCE CONFIDENTIAL
	PlayStation(R)3 Programmer Tool Runtime Library 310.001
	Copyright (C) 2008 Sony Computer Entertainment Inc.
	All Rights Reserved.
*/
/**
	\internal
	\file fios_collections.h

	Definitions for the collection classes in the fios::collections namespace.
*/
#ifndef __CELL_FIOS_COLLECTIONS_H__
#define __CELL_FIOS_COLLECTIONS_H__

#include <cell/fios/fios_types.h>
#include <cell/fios/fios_base.h>
#include <cell/fios/fios_platform.h>

namespace cell {
namespace fios {

/**
 * \internal
 * Lightweight collection classes used by FIOS.
 */
namespace collections {

/** \internal \brief Simple singly-linked list class. */
template <class T> class FIOS_LINKAGE list
{
public:
	list<T>() : m_pListHead(NULL) {}
	list<T>(T pHead) : m_pListHead(pHead) {}
	void push(T pNewHead) {
		if (FIOS_UNLIKELY(pNewHead == NULL)) return;
		pNewHead->m_pNext = m_pListHead;
		m_pListHead = pNewHead;
	}
	T pop() {
		T pOut = m_pListHead;
		if (pOut != NULL) {
			m_pListHead = pOut->m_pNext;
			pOut->m_pNext = NULL;
		}
		return pOut;
	}
	T steal() {
		T pOut = m_pListHead;
		m_pListHead = NULL;
		return pOut;
	}
	size_t count() {
		size_t c = 0;
		T pWalk = m_pListHead;
		while (pWalk != NULL) {
			c++;
			pWalk = pWalk->m_pNext;
		}
		return c;
	}
protected:
	T & find(T pItem) {
		T *pOut = &m_pListHead;
		while (((*pOut) != pItem) && ((*pOut) != NULL)) {
			pOut = const_cast<T*>(&(*pOut)->m_pNext);
		}
		return (*pOut);
	}
public:
	bool remove(T pItem) {
		if (FIOS_UNLIKELY(pItem == NULL)) return false;
		T &pFound = find(pItem);
		if (pFound) {
			pFound = pItem->m_pNext;
			pItem->m_pNext = NULL;
			return true;
		} else {
			return false;
		}
	}
	bool contains(T pItem) {
		if (FIOS_UNLIKELY(pItem == NULL)) return false;
		return find(pItem) == pItem;
	}
	void pushLast(T pItem) { // expensive, requires walking to end of list
		if (FIOS_UNLIKELY(pItem == NULL)) return;
		(find(NULL)) = pItem;
	}
	void unsteal(T pRestore) {
		pushLast(pRestore);
	}
	bool isEmpty() const {
		T pHead = m_pListHead;
		return (pHead == NULL);
	}
	const T & head() const {
		return m_pListHead;
	}
	T & head() {
		return m_pListHead;
	}
	
protected:
	T m_pListHead;
};

/** \internal \brief Atomic singly-linked list class. */
template <class T> class FIOS_LINKAGE atomicList
{
public:
	inline atomicList<T>() {}
	void push(T pNewHead) {
		if (FIOS_LIKELY(pNewHead != NULL)) do {
			pNewHead->m_pNext = m_pListHead;
		} while (FIOS_UNLIKELY(!m_pListHead.compareAndSwap(pNewHead->m_pNext,pNewHead)));
	}
	T pop() {
		T pOut;
		do {
			pOut = m_pListHead;
		} while ((pOut != NULL) && FIOS_UNLIKELY(!m_pListHead.compareAndSwap(pOut,pOut->m_pNext)));
		if (pOut != NULL) pOut->m_pNext = NULL;
		return pOut;
	}
	T steal() {
		T pOut;
		do {
			pOut = m_pListHead;
		} while (FIOS_UNLIKELY(!m_pListHead.compareAndSwap(pOut,NULL)));
		return pOut;
	}
	void pushLast(T pItem) {
		if (FIOS_UNLIKELY(pItem == NULL)) return;
		// pRestore:     D C B A
		// m_pListHead:  F E
		// end result:   F E D C B A
		while (!m_pListHead.compareAndSwap(NULL,pItem))
		{
			list<T> newList(steal());
			newList.pushLast(pItem);
			pItem = newList.head();
		}
	}
	void unsteal(T pRestore) {
		pushLast(pRestore);
	}

	bool isEmpty() const {
		T pHead = m_pListHead;
		return (pHead == NULL);
	}

protected:
        platform::atomicPtr<T> m_pListHead;
};

/** \internal \brief Dynamic array. */
template <class T> class FIOS_LINKAGE array {
public:
    explicit array(allocator *allocator_, uint32_t memPurpose_)
	: m_allocator(allocator_), m_memPurpose(memPurpose_ | kPoolIDLongTerm | FIOS_MEMFLAGS_FROM_ALIGNMENT(__alignof(T))),
	  m_internalElements(m_internalElementStorage),
	  m_elements(m_internalElements), m_capacity(kInternalCapacityCount), m_used(0) {}
    
	~array() {
		reset();
		if (m_elements != m_internalElements)
			m_allocator->Deallocate(m_elements, m_memPurpose, FIOS_MEMORY_ANNOTATION);
	}
    
    size_t length() const { return m_used; }
    
    bool append(const T &value_) {
        //FIOS_ASSERT(invariants());
        
        if (m_used == m_capacity) {
            //  No space left, attempt to reallocate.
            size_t newCapacity = m_capacity * 2;
			T *newElements = static_cast<T*>(m_allocator->Allocate((MemSize)(newCapacity * sizeof(T)), m_memPurpose, FIOS_MEMORY_ANNOTATION));
            if (newElements) {
                for (size_t i = 0; i < m_used; ++i) {
                    newElements[i] = m_elements[i];
					// TODO: I'm not sure this destructor call is actually useful, since
					// the above line essentially assume the object is POD.
                    m_elements[i].~T();
                }
                if (m_elements != m_internalElements)
					m_allocator->Deallocate(m_elements, m_memPurpose, FIOS_MEMORY_ANNOTATION);
                m_elements = newElements;
                m_capacity = newCapacity;
            } else {
                //FIOS_ASSERT(invariants());
                return false;
            }
        }
        
        m_elements[m_used++] = value_;
        //FIOS_ASSERT(invariants());
        return true;
    }
    
    T& operator[](size_t index_) const {
        //FIOS_ASSERT(invariants());
        //FIOS_ASSERT(index_ < m_used);
		// "Since we can't throw exceptions, it should assert against bad
		// accesses and if one occurs just return the first slot."
		if (index_ >= m_used)
			index_ = 0;
        return m_elements[index_];
    }
    
    void reset() {
        //FIOS_ASSERT(invariants());
        remove(0, m_used);
        //FIOS_ASSERT(invariants());
    }
    
    void remove(size_t offset_, size_t count_) {
        //FIOS_ASSERT(invariants());
        //FIOS_ASSERT(count_ <= m_used);
        //FIOS_ASSERT(offset_ <= m_used);
        //FIOS_ASSERT(count_+offset_ <= m_used);
        
		T *elementsEnd = m_elements + m_used;
		
		// Call destructors.
		// TODO: I'm not sure this destructor loop is actually useful, since other parts of
		// the code essentially assume the object is POD.
        T *removingIt = m_elements + offset_;
        T *removingEnd = removingIt + count_;
        //FIOS_ASSERT((removingEnd - removingIt) == count_);
        for (; removingIt != removingEnd; ++removingIt) {
            removingIt->~T();
        }
		
		//	Cinch up the array (overwrite the old elements with the elements that proceed them).
		removingIt = m_elements + offset_;
		T *replacingIt = removingEnd;
        T *replacingEnd = elementsEnd;
        for (; replacingIt != replacingEnd; ++replacingIt, ++removingIt) {
            *removingIt = *replacingIt;
        }
        m_used -= count_;
        
        //FIOS_ASSERT(invariants());
    }
	void move(size_t from_, size_t to_) {
		//FIOS_ASSERT(invariants());
		//FIOS_ASSERT(from_ <= m_used);
		//FIOS_ASSERT(to_ <= m_used);
		if (from_ != to_) {
			int shiftBy = (from_ < to_) ? +1:-1;
			T *movingIt = m_elements + from_;
			T *movingEnd = m_elements + to_;
			T moved(*movingIt);
			for (; movingIt != movingEnd; movingIt += shiftBy) {
				*movingIt = *(movingIt+shiftBy);
			}
			*movingIt = moved;
		}
	}

private:
	array(const array&){}; // Don't allow copying.
	array& operator=(const array&){ return *this; }; // Don't allow assignment.
	
    enum { kInternalCapacityCount = 4 };
    
    allocator *m_allocator;
    uint32_t  m_memPurpose;
    T         *m_internalElements;
    T         m_internalElementStorage[kInternalCapacityCount];
    T         *m_elements;
    size_t    m_capacity, m_used;
    
    bool invariants() const {
        //FIOS_ASSERT(m_allocator);
        //FIOS_ASSERT(m_internalElements);
        //FIOS_ASSERT(m_elements);
        //FIOS_ASSERT(m_capacity);
        //FIOS_ASSERT(m_capacity % 2 == 0);
        //FIOS_ASSERT(m_used <= m_capacity);
        return true;
    }
};

/** \brief Dynamic array; elements are stored in slices. Scales better to arbitrary sizes. */
template <class T, int kInitialCapacity=4, int kGrowthSize=32> class FIOS_LINKAGE slicearray {
protected:
	struct header {
		size_t   capacity;
		size_t   used;
		T *      pElements;
		header * pPrev;
		header * pNext;
		
		header(size_t c, T* e) : capacity(c), used(0), pElements(e), pPrev(0), pNext(0) {}
	};
	template <int n> struct slice {
		header  h;
		T       elements[n];
		
		void * operator new (size_t, void *pPlacement) { return (void*)pPlacement; }
		void operator delete(void *, void *) {}
		slice() : h(n,elements) {}
	};
	typedef slice<kInitialCapacity> islice;
	typedef slice<kGrowthSize>      gslice;

public:
	slicearray(allocator *allocator_, uint32_t memPurpose_) :
		m_pAllocator(allocator_),
		m_pLastSlice(&m_internalSlice.h),
		m_purpose(memPurpose_ | FIOS_MEMFLAGS_FROM_ALIGNMENT(__alignof(T))),
		m_capacity(kInitialCapacity),
		m_used(0),
		m_internalSlice()
	{
        //FIOS_ASSERT(invariants());
	}

	~slicearray()
	{
        //FIOS_ASSERT(invariants());
		reset();
	}

    size_t length() const
	{
		//FIOS_ASSERT(invariants());
		return m_used;
	}

    bool append(const T &value_)
	{
		//FIOS_ASSERT(invariants());
		if (FIOS_LIKELY(m_used < m_capacity)) {
			// Stash this item at the end of the last slice.
			//FIOS_ASSERT_RELATION(m_used,<,m_capacity);
			//FIOS_ASSERT_RELATION(m_pLastSlice->used,<,m_pLastSlice->capacity);
			m_pLastSlice->pElements[m_pLastSlice->used++] = value_;
			m_used++;
			return true;
		}

		// Nope - we're full.
		// Allocate and initialize a new slice.
		gslice *pNew = static_cast<gslice*>(m_pAllocator->Allocate(sizeof(gslice), m_purpose, FIOS_MEMORY_ANNOTATION));
		if (pNew == NULL)
			return false;
		pNew = new (pNew) gslice; // placement new
		m_capacity += pNew->h.capacity;

		// Put the new slice into our slice list.
		pNew->h.pPrev = m_pLastSlice;
		m_pLastSlice->pNext = &pNew->h;
		m_pLastSlice = &pNew->h;
		
		// Tail-recurse to put it in.
		return append(value_);
	}

	T& operator[](size_t index_) const
	{
        //FIOS_ASSERT(invariants());
		size_t i = index_;
		//FIOS_ASSERT_RELATION(i,<,m_used);
		if (FIOS_LIKELY(i < m_used)) {
			for (const header *pWalk = &m_internalSlice.h; pWalk != NULL; pWalk = pWalk->pNext) {
				if (i < pWalk->capacity) {
					//FIOS_ASSERT_RELATION(i,<,pWalk->used);
					return (T&)pWalk->pElements[i];
				}
				i -= pWalk->capacity;
			}
			//FIOS_ASSERT(false); // unreachable
		}
		return m_internalSlice.h.pElements[0]; // error result
	}

	void reset()
	{
	        //FIOS_ASSERT(invariants());
		remove(0,m_used);
	}
	
	void remove(size_t offset_, size_t count_)
	{
	        //FIOS_ASSERT(invariants());
		
		// Check for parameter errors
		//FIOS_ASSERT_RELATION(offset_,<,m_used);
		//FIOS_ASSERT_RELATION(offset+count,<=,m_used);
		if (FIOS_UNLIKELY(offset_ >= m_used) ||
			FIOS_UNLIKELY(offset_+count_ > m_used) ||
			FIOS_UNLIKELY(count_ < 1))
			return;
		
		// Loop and shift elements.
		for (size_t i=offset_; i<m_used && (i+count_)<m_used; ++i)
			(*this)[i] = (*this)[i+count_];

		// Clear out items from the end of the array, and remove empty slices.
		size_t entriesToRemove = count_;
		for (header *pWalk = m_pLastSlice; pWalk != NULL && entriesToRemove > 0;)
		{
			header *pPrev = pWalk->pPrev;
			if (FIOS_LIKELY(pWalk->used > entriesToRemove))
			{
				m_used -= entriesToRemove;
				pWalk->used -= entriesToRemove;
				entriesToRemove = 0;
			}
			else
			{
				entriesToRemove -= pWalk->used;
				m_used -= pWalk->used;
				pWalk->used = 0;
				if (pWalk != &m_internalSlice.h)
				{
					// This slice is empty; free it.
					//FIOS_ASSERT(pPrev != NULL);
					pPrev->pNext = NULL;
					//FIOS_ASSERT_RELATION(m_capacity,>=,pWalk->capacity);
					m_capacity -= pWalk->capacity;
					m_pAllocator->Deallocate(pWalk, m_purpose, FIOS_MEMORY_ANNOTATION);
					m_pLastSlice = pPrev;
				}
			}
			pWalk = pPrev;
		}
		//FIOS_ASSERT_EQUAL(entriesToRemove,0);
	}

	void move(size_t from_, size_t to_)
	{
        //FIOS_ASSERT(invariants());
        //FIOS_ASSERT(from_ <= m_used);
        //FIOS_ASSERT(to_ <= m_used);
        if (from_ != to_) {
            int shiftBy = (from_ < to_) ? +1:-1;
            T moved((*this)[from_]);
            for (; from_ != to_; from_ += shiftBy)
                (*this)[from_] = (*this)[from_+shiftBy];
			(*this)[to_] = moved;
        }
	}

private:
	slicearray(const slicearray&){}; // Don't allow copying.
	slicearray& operator=(const slicearray&){ return *this; }; // Don't allow assignment.
	
	bool invariants() const {
		if ((m_pAllocator == NULL) ||
			(m_pLastSlice != NULL) ||
			(m_used > m_capacity) ||
			(m_capacity < m_internalSlice.h.capacity) ||
			(m_internalSlice.h.used > m_internalSlice.h.capacity))
			return false;
		return true;
	}

	allocator *  m_pAllocator;      //!< Memory allocator
	header *     m_pLastSlice;      //!< Pointer to the tail of the slice list.
	uint32_t     m_purpose;         //!< Memory purpose for the allocator.
	size_t       m_capacity;        //!< Available slots.
	size_t       m_used;            //!< Used slots.
	islice       m_internalSlice;   //!< Built-in array slice; contains pointer to the next slice in the list.
};

/** \brief Class to hold variable-sized data, stored in chunks */
class FIOS_LINKAGE chunkstorage {
public:
    chunkstorage(allocator *allocator_, uint32_t memPurpose_, uint32_t chunkSize_ = 0);
	~chunkstorage();
	
	// Helper template which automatically selects proper size and alignment
	template<typename T> T * store(T *pData) {
		return (T*)store((void*)pData, sizeof(T), __alignof(T));
	}

	// Helper template which automatically selects proper alignment
	template<typename T> T * store(T *pData, size_t len) {
		return (T*)store((void*)pData, len, __alignof(T));
	}
	
	// Common implementation of store
	void * store(void *pData, size_t len, size_t align);

	// Resets the storage. All memory is released.
	void reset();
	
	// Bookkeeping.
	uint32_t chunkSize() const { return m_chunkSize; }
	uint32_t chunkCount() const { return m_chunkCount; }
	uint32_t storedCount() const { return m_storedCount; }

private:
	enum { kSmallestChunkSize = 4096 };
	chunkstorage(const chunkstorage&){}; // Don't allow copying.
	chunkstorage& operator=(const chunkstorage&){ return *this; }; // Don't allow assignment.
	struct chunk {
		chunk * pNext;
		size_t  bytesUsed;
	};
	
	allocator *m_pAllocator;
	uint32_t   m_purpose;
	uint32_t   m_chunkSize;
	chunk *    m_pChunks;
	uint32_t   m_chunkCount;
	uint32_t   m_storedCount;
};

}; /* namespace collections */
}; /* namespace fios */
}; /* namespace cell */


#endif	//	__CELL_FIOS_COLLECTIONS_H__
