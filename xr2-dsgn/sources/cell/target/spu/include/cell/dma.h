/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_DMA_H__
#define __CELL_DMA_H__

#include <spu_mfcio.h>
#include <cellstatus.h>
#include <cell/error.h>
#include <stdint.h>

typedef mfc_list_element_t CellDmaListElement;

/***************************************************************
 * DMA Assertions
 ***************************************************************/

#ifdef NO_CELL_DMA_ASSERT
#define cellDmaAssert(cond, ...)
#else /* NO_CELL_DMA_ASSERT */
#ifndef CELL_DMA_ASSERT_VERBOSE
#define cellDmaAssert(cond, ...) spu_hcmpeq((cond), 0)
#else
#include <spu_printf.h>

#define __CELL_DMA_STR(name) #name
#define __CELL_DMA_STR_LINE(line) __CELL_DMA_STR(line)
#define __CELL_DMA_FILELINE __FILE__":"__CELL_DMA_STR_LINE(__LINE__)": "

#define cellDmaAssert(cond, fmt, ...) \
do { \
	if (!(cond)) { \
		spu_printf("%s:%u %s" fmt, __FILE__,__LINE__,"[cellDmaAssert]",##__VA_ARGS__); \
		__asm__ volatile("stopd $0,$0,$0\n"); \
	} \
} while(0)
#endif
#endif /* NO_CELL_DMA_ASSERT */

/***************************************************************
 * DMA which transfer size is a multiple of 16bytes
 ***************************************************************/
#define cellDmaNormalAssert(ls, ea, size, tag) \
	cellDmaAssert((((uintptr_t)(ls) & 0xf) == 0)	& \
				  (((uintptr_t)(ea) & 0xf) == 0)	& \
				  (((size) & 0xf) == 0)				& \
				  ((size) <= (16<<10))				& \
				  ((tag) < 32), "ls=%#x,ea=%#llx,size=%#x,tag=%u\n", ls,ea,size,tag)

#define cellDmaPut(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaNormalAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_put((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaPutf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaNormalAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_putf((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaPutb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaNormalAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_putb((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaGet(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaNormalAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_get((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaGetf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaNormalAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_getf((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaGetb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaNormalAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_getb((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

/***************************************************************
 * DMA which transfer size is within 16bytes
 ***************************************************************/
#define cellDmaSmallAssert(ls, ea, size, tag) \
	cellDmaAssert((((uintptr_t)(ls) & 0xf) == ((uintptr_t)(ea) & 0xf))	& \
				  (((uintptr_t)(ls) & (size - 1)) == 0)					& \
				  (((size)==1)||((size)==2)||((size)==4)||((size)==8))	& \
				  ((tag) < 32), "ls=%#x,ea=%#llx,size=%#x,tag=%u\n", ls,ea,size,tag)

#define cellDmaSmallPut(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaSmallAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_put((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaSmallPutf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaSmallAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_putf((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaSmallPutb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaSmallAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_putb((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaSmallGet(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaSmallAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_get((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaSmallGetf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaSmallAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_getf((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaSmallGetb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls   = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea   = ea; \
	uint32_t  __cellDma_size = size; \
	uint32_t  __cellDma_tag  = tag; \
	cellDmaSmallAssert(__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag); \
	mfc_getb((void*)__cellDma_ls,__cellDma_ea,__cellDma_size,__cellDma_tag,tid,rid); \
} while(0)

/***************************************************************
 * List DMA
 ***************************************************************/
#define cellDmaListAssert(ls, ea, la, lsize, tag) \
	cellDmaAssert((((uintptr_t)(ls) & 0xf) == 0)	& \
				  (((uintptr_t)(ea) & 0xf) == 0)	& \
				  (((uintptr_t)(la) &   7) == 0)	& \
				  (((lsize) & 7) == 0)				& \
				  ((lsize) <= (16<<10))				& \
				  ((tag) < 32), "ls=%#x,ea=%#llx,la=%#x,lsize=%#x,tag=%u\n", ls,ea,la,lsize,tag)

#define cellDmaListPut(ls, ea, list, lsize, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls    = (uintptr_t)(ls); \
	uint64_t __cellDma_ea    = ea; \
	const CellDmaListElement *__cellDma_list = list; \
	uint32_t __cellDma_lsize = lsize; \
	uint32_t __cellDma_tag   = tag; \
	cellDmaListAssert(__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag); \
	mfc_putl((void*)__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaListPutf(ls, ea, list, lsize, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t __cellDma_ea   = ea; \
	const CellDmaListElement *__cellDma_list = list; \
	uint32_t __cellDma_lsize = lsize; \
	uint32_t __cellDma_tag = tag; \
	cellDmaListAssert(__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag); \
	mfc_putlf((void*)__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaListPutb(ls, ea, list, lsize, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t __cellDma_ea   = ea; \
	const CellDmaListElement *__cellDma_list = list; \
	uint32_t __cellDma_lsize = lsize; \
	uint32_t __cellDma_tag = tag; \
	cellDmaListAssert(__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag); \
	mfc_putlb((void*)__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaListGet(ls, ea, list, lsize, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t __cellDma_ea   = ea; \
	const CellDmaListElement *__cellDma_list = list; \
	uint32_t __cellDma_lsize = lsize; \
	uint32_t __cellDma_tag = tag; \
	cellDmaListAssert(__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag); \
	mfc_getl((void*)__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaListGetf(ls, ea, list, lsize, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t __cellDma_ea   = ea; \
	const CellDmaListElement *__cellDma_list = list; \
	uint32_t __cellDma_lsize = lsize; \
	uint32_t __cellDma_tag = tag; \
	cellDmaListAssert(__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag); \
	mfc_getlf((void*)__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag,tid,rid); \
} while(0)

#define cellDmaListGetb(ls, ea, list, lsize, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t __cellDma_ea   = ea; \
	const CellDmaListElement *__cellDma_list = list; \
	uint32_t __cellDma_lsize = lsize; \
	uint32_t __cellDma_tag = tag; \
	cellDmaListAssert(__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag); \
	mfc_getlb((void*)__cellDma_ls,__cellDma_ea,__cellDma_list,__cellDma_lsize,__cellDma_tag,tid,rid); \
} while(0)

/***************************************************************
 * Atomic DMA
 ***************************************************************/
#define cellDmaAtomicAssert(ls, ea) \
	cellDmaAssert((((uintptr_t)(ls) & 0x7f) == 0)	& \
				  (((uintptr_t)(ea) & 0x7f) == 0), "ls=%#x,ea=%#llx\n", ls,ea)

#define cellDmaGetllar(ls, ea, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea = ea; \
	cellDmaAtomicAssert(__cellDma_ls,__cellDma_ea); \
	mfc_getllar((void*)__cellDma_ls,__cellDma_ea,tid,rid); \
} while(0)

#define cellDmaPutllc(ls, ea, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea = ea; \
	cellDmaAtomicAssert(__cellDma_ls,__cellDma_ea); \
	mfc_putllc((void*)__cellDma_ls,__cellDma_ea,tid,rid); \
} while(0)

#define cellDmaPutlluc(ls, ea, tid, rid) \
do { \
	uintptr_t __cellDma_ls = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea = ea; \
	cellDmaAtomicAssert(__cellDma_ls,__cellDma_ea); \
	mfc_putlluc((void *)__cellDma_ls,__cellDma_ea,tid,rid); \
} while(0)

#define cellDmaPutqllucAssert(ls, ea, tag) \
	cellDmaAssert((((uintptr_t)(ls) & 0x7f) == 0)	& \
				  (((uintptr_t)(ea) & 0x7f) == 0)	& \
				  ((tag) < 32), "ls=%#x,ea=%#llx,tag=%u\n", ls,ea,tag)

#define cellDmaPutqlluc(ls, ea, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaPutqllucAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	mfc_putqlluc((void*)__cellDma_ls,__cellDma_ea,__cellDma_tag,tid,rid); \
} while(0)

/***************************************************************
 * DMA Utilities - Data typed DMA
 ***************************************************************/
#ifdef __cplusplus
extern "C"
#endif
void cellDmaAndWait(uintptr_t ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t cmd);

#define cellDmaEa2Ls(ea, ls) ((uintptr_t)(ls)+((uint32_t)(ea)&15))

#define cellDmaDataAssert(ea, size, tag) \
	cellDmaAssert((((uint32_t)(ea) & (size - 1)) == 0)					& \
				  (((size)==1)||((size)==2)||((size)==4)||((size)==8))	& \
				  ((tag) < 32), "ea=%#llx,size=%#x,tag=%u\n", ea,size,tag)

#define cellDmaPutUintTemplate(SIZE, value, ea, tag, tid, rid)																				\
do {																																		\
	uint64_t __cellDma_ea  = ea;																											\
	uint32_t __cellDma_tag = tag;																											\
	qword _buf = (qword)spu_splats(value);																									\
	cellDmaDataAssert(__cellDma_ea,sizeof(uint##SIZE##_t),__cellDma_tag);																	\
	cellDmaAndWait(cellDmaEa2Ls(__cellDma_ea,&_buf),__cellDma_ea,sizeof(uint##SIZE##_t),__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_PUT_CMD));	\
} while(0)

#define cellDmaPutUint8(value, ea, tag, tid, rid) cellDmaPutUintTemplate(8, ((uint8_t)value), ea, tag, tid, rid)
#define cellDmaPutUint16(value, ea, tag, tid, rid) cellDmaPutUintTemplate(16, ((uint16_t)value), ea, tag, tid, rid)
#define cellDmaPutUint32(value, ea, tag, tid, rid) cellDmaPutUintTemplate(32, ((uint32_t)value), ea, tag, tid, rid)
#define cellDmaPutUint64(value, ea, tag, tid, rid) cellDmaPutUintTemplate(64, ((uint64_t)value), ea, tag, tid, rid)

#define cellDmaGetUintTemplate(SIZE, ea, tag, tid, rid) __extension__																		\
({																																			\
	uint64_t __cellDma_ea  = ea;																											\
	uint32_t __cellDma_tag = tag;																											\
	qword _buf;																																\
	cellDmaDataAssert(__cellDma_ea,sizeof(uint##SIZE##_t),__cellDma_tag);																	\
	cellDmaAndWait(cellDmaEa2Ls(__cellDma_ea,&_buf),__cellDma_ea,sizeof(uint##SIZE##_t),__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_GET_CMD));	\
	*(uint##SIZE##_t*)((uintptr_t)&_buf + ((uintptr_t)__cellDma_ea&15));																	\
})

#define cellDmaGetUint8(ea, tag, tid, rid) cellDmaGetUintTemplate(8, ea, tag, tid, rid)
#define cellDmaGetUint16(ea, tag, tid, rid) cellDmaGetUintTemplate(16, ea, tag, tid, rid)
#define cellDmaGetUint32(ea, tag, tid, rid) cellDmaGetUintTemplate(32, ea, tag, tid, rid)
#define cellDmaGetUint64(ea, tag, tid, rid) cellDmaGetUintTemplate(64, ea, tag, tid, rid)

/***************************************************************
 * DMA Utilities - Any size DMA
 ***************************************************************/
#ifdef __cplusplus
extern "C"
#endif
void cellDmaLargeCmd(uintptr_t ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t cmd);

#define cellDmaLargeAssert(ls, ea, tag) \
	cellDmaAssert((((uintptr_t)(ls) & 0xf) == 0)	& \
				  (((uintptr_t)(ea) & 0xf) == 0)	& \
				  ((tag) < 32), "ls=%#x,ea=%#llx,tag=%u\n", ls,ea,tag)

#define cellDmaLargePut(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaLargeAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaLargeCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_PUT_CMD)); \
} while(0)

#define cellDmaLargePutf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaLargeAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaLargeCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_PUTF_CMD)); \
} while(0)

#define cellDmaLargePutb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaLargeAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaLargeCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_PUTB_CMD)); \
} while(0)

#define cellDmaLargeGet(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaLargeAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaLargeCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_GET_CMD)); \
} while(0)

#define cellDmaLargeGetf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaLargeAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaLargeCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_GETF_CMD)); \
} while(0)

#define cellDmaLargeGetb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaLargeAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaLargeCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_GETB_CMD)); \
} while(0)

/***************************************************************
 * DMA Utilities - Any size unaligned DMA
 ***************************************************************/
#ifdef __cplusplus
extern "C"
#endif
void cellDmaUnalignedCmd(uintptr_t ls, uint64_t ea, uint32_t size, uint32_t tag, uint32_t cmd);

#define cellDmaUnalignedAssert(ls, ea, tag) \
	cellDmaAssert((((uintptr_t)(ls) & 0xf) == ((uintptr_t)(ea) & 0xf)) & \
				  ((tag) < 32), "ls=%#x,ea=%#llx,tag=%u\n", ls,ea,tag)

#define cellDmaUnalignedPut(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaUnalignedAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaUnalignedCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_PUT_CMD)); \
} while(0)

#define cellDmaUnalignedPutf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaUnalignedAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaUnalignedCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_PUTF_CMD)); \
} while(0)

#define cellDmaUnalignedPutb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaUnalignedAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaUnalignedCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_PUTB_CMD)); \
} while(0)

#define cellDmaUnalignedGet(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaUnalignedAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaUnalignedCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_GET_CMD)); \
} while(0)

#define cellDmaUnalignedGetf(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaUnalignedAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaUnalignedCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_GETF_CMD)); \
} while(0)

#define cellDmaUnalignedGetb(ls, ea, size, tag, tid, rid) \
do { \
	uintptr_t __cellDma_ls  = (uintptr_t)(ls); \
	uint64_t  __cellDma_ea  = ea; \
	uint32_t  __cellDma_tag = tag; \
	cellDmaUnalignedAssert(__cellDma_ls,__cellDma_ea,__cellDma_tag); \
	cellDmaUnalignedCmd(__cellDma_ls,__cellDma_ea,size,__cellDma_tag,MFC_CMD_WORD(tid,rid,MFC_GETB_CMD)); \
} while(0)

/***************************************************************
 * DMA Utilities - Tag wait
 ***************************************************************/
__attribute__((always_inline))
static inline void
cellDmaCancelTagStatusUpdate(void)
{
	mfc_write_tag_update_immediate();
	do {} while(__builtin_expect(mfc_stat_tag_update() == 0,0));
	mfc_read_tag_status();
}

__attribute__((always_inline))
static inline uint32_t
cellDmaCancelAndWaitTagStatusAny(uint32_t tagmask)
{
	cellDmaCancelTagStatusUpdate();
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_any();
}

__attribute__((always_inline))
static inline uint32_t
cellDmaCancelAndWaitTagStatusAll(uint32_t tagmask)
{
	cellDmaCancelTagStatusUpdate();
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_all();
}

__attribute__((always_inline))
static inline uint32_t
cellDmaWaitTagStatusImmediate(uint32_t tagmask)
{
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_immediate();
}

__attribute__((always_inline))
static inline uint32_t
cellDmaWaitTagStatusAny(uint32_t tagmask)
{
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_any();
}

__attribute__((always_inline))
static inline uint32_t
cellDmaWaitTagStatusAll(uint32_t tagmask)
{
	mfc_write_tag_mask(tagmask);
	return mfc_read_tag_status_all();
}

#define cellDmaWaitAtomicStatus() mfc_read_atomic_status()

/***************************************************************
 * Error codes
 ***************************************************************/
#define CELL_ERROR_MINOR_FACILITY_DMA          0x0
#define CELL_ERROR_MAKE_DMA_ERROR(id) \
	(CELL_ERROR_MAKE_ERROR(CELL_ERROR_FACILITY_SPU, (CELL_ERROR_MINOR_FACILITY_DMA << 8) | (id)))

#define CELL_DMA_ERROR_INVAL CELL_ERROR_CAST(0x80410002)

#endif /* CELL_DMA_H */

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
