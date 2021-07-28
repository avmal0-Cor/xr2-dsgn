/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* Buffer::Abstract
 * Buffer base class
 */

#ifndef __CELL_DAISY_BUFFER_H__
#define __CELL_DAISY_BUFFER_H__

#include <cell/daisy/daisy_defs.h>
#include <cell/daisy/snprintf.h>

namespace cell {
	namespace Daisy {
		namespace Buffer {
    
			template <typename tType, SizeType tSize>
			class Abstract {
      
			public:
      
				static const BufferType sBufferType;
				static const SizeType sSize = tSize;
#ifdef __SPU__
				static const ConstructorMode sConstructorMode = NO_PARAMETER;
#endif

				/* data type definition */

				typedef tType DataType;

				/* constructor definition */
      
				Abstract();

				/* virtual destructor definition */
				virtual ~Abstract(){}

				virtual const char *getClassName(){ return 0;}

				/* return reference to entry pointer i
				 * @param i buffer entry pointer
				 * @retval reference to buffer entry
				 */
				volatile tType* getEntryReference(PointerType ptr){ (void)ptr; return NULL; }
      
				/* Blocking type data write begin
				 * @param ptr buffer entry pointer 
				 * @param src buffer entry which has write data
				 */
				void beginWrite(PointerType ptr, tType * __restrict__ src){(void)ptr;(void)src;}
      
				/* Non-blocking type data write begin
				 * @param ptr buffer entry pointer 
				 * @param src buffer entry which has write data
				 * @retval CELL_OK or QUEUE_IS_BUSY
				 */
				int tryBeginWrite(PointerType ptr, tType * __restrict__ src){ (void)ptr;(void)src;return 0; }
      
				/* Blocking type data write end
				 * @param ptr buffer entry pointer 
				 */
				void endWrite(PointerType ptr){(void)ptr;}
      
				/* Non-blocking type data write end
				 * @param ptr buffer entry pointer 
				 * @retval CELL_OK or QUEUE_IS_BUSY
				 */
				int tryEndWrite(PointerType ptr){ (void)ptr;return 0; }
      
				/* Blocking type data read begin
				 * @param ptr buffer entry pointer 
				 * @param src buffer entry to hold read data
				 */
				void beginRead(PointerType ptr, tType * __restrict__ dst){(void)ptr;(void)dst;}
      
				/* Non-blocking type data read begin
				 * @param ptr buffer entry pointer 
				 * @param src buffer entry to hold read data
				 * @retval CELL_OK or QUEUE_IS_BUSY
				 */
				int tryBeginRead(PointerType ptr, tType * __restrict__ dst){ (void)ptr;(void)dst;return 0; }
      
				/* Blocking type data read end
				 * @param ptr buffer entry pointer 
				 */
				void endRead(PointerType ptr){(void)ptr;}
      
				/* Non-blocking type data read end
				 * @param ptr buffer entry pointer 
				 * @retval CELL_OK or QUEUE_IS_BUSY
				 */
				int tryEndRead(PointerType ptr){ (void)ptr;return 0; }

				uint32_t getTag(){ return 0; }

			}; /* class Buffer */
    
			/* constructor description */
    
			template <typename tType, SizeType tSize> inline Abstract<tType, tSize>::Abstract(){}

		} /* namespace Buffer */    
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_BUFFER_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
