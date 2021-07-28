/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __CELL_GCM_IMPLEMENTATION_ASM_H__
#define __CELL_GCM_IMPLEMENTATION_ASM_H__

/********************************************************************/
/*																	*/
/*		inline functions to reduce LHS								*/
/*																	*/
/********************************************************************/

//	Save/Restore r3 - r10
static inline __attribute__((always_inline)) void _cellGcmReserveImm7(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"std	%%r5, 0x68(%%r1);"	// save r5
		"std	%%r6, 0x60(%%r1);"	// save r6
		"std	%%r7, 0x58(%%r1);"	// save r7
		"std	%%r8, 0x50(%%r1);"	// save r8
		"std	%%r9, 0x48(%%r1);"	// save r9
		"std	%%r10,0x40(%%r1);"	// save r10
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r5, 0x68(%%r1);"	// restore r5
		"ld		%%r6, 0x60(%%r1);"	// restore r6
		"ld		%%r7, 0x58(%%r1);"	// restore r7
		"ld		%%r8, 0x50(%%r1);"	// restore r8
		"ld		%%r9, 0x48(%%r1);"	// restore r9
		"ld		%%r10,0x40(%%r1);"	// restore r10
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3 - r9
static inline __attribute__((always_inline)) void _cellGcmReserveImm6(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"std	%%r5, 0x68(%%r1);"	// save r5
		"std	%%r6, 0x60(%%r1);"	// save r6
		"std	%%r7, 0x58(%%r1);"	// save r7
		"std	%%r8, 0x50(%%r1);"	// save r8
		"std	%%r9, 0x48(%%r1);"	// save r9
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r5, 0x68(%%r1);"	// restore r5
		"ld		%%r6, 0x60(%%r1);"	// restore r6
		"ld		%%r7, 0x58(%%r1);"	// restore r7
		"ld		%%r8, 0x50(%%r1);"	// restore r8
		"ld		%%r9, 0x48(%%r1);"	// restore r9
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3 - r8
static inline __attribute__((always_inline)) void _cellGcmReserveImm5(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"std	%%r5, 0x68(%%r1);"	// save r5
		"std	%%r6, 0x60(%%r1);"	// save r6
		"std	%%r7, 0x58(%%r1);"	// save r7
		"std	%%r8, 0x50(%%r1);"	// save r8
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r5, 0x68(%%r1);"	// restore r5
		"ld		%%r6, 0x60(%%r1);"	// restore r6
		"ld		%%r7, 0x58(%%r1);"	// restore r7
		"ld		%%r8, 0x50(%%r1);"	// restore r8
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3 - r7
static inline __attribute__((always_inline)) void _cellGcmReserveImm4(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"std	%%r5, 0x68(%%r1);"	// save r5
		"std	%%r6, 0x60(%%r1);"	// save r6
		"std	%%r7, 0x58(%%r1);"	// save r7
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r5, 0x68(%%r1);"	// restore r5
		"ld		%%r6, 0x60(%%r1);"	// restore r6
		"ld		%%r7, 0x58(%%r1);"	// restore r7
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3 - r6
static inline __attribute__((always_inline)) void _cellGcmReserveImm3(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"std	%%r5, 0x68(%%r1);"	// save r5
		"std	%%r6, 0x60(%%r1);"	// save r6
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r5, 0x68(%%r1);"	// restore r5
		"ld		%%r6, 0x60(%%r1);"	// restore r6
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3 - r5
static inline __attribute__((always_inline)) void _cellGcmReserveImm2(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"std	%%r5, 0x68(%%r1);"	// save r5
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r5, 0x68(%%r1);"	// restore r5
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3 - r4
static inline __attribute__((always_inline)) void _cellGcmReserveImm1(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3
static inline __attribute__((always_inline)) void _cellGcmReserveImm0(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ volatile (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"addi	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"li		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "i" (count) , [size] "i" (size)
		: "r11","r12","memory");
}


//	Save/Restore r3 - r6
static inline __attribute__((always_inline)) void _cellGcmReserveReg3(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"add	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"std	%%r5, 0x68(%%r1);"	// save r5
		"std	%%r6, 0x60(%%r1);"	// save r6
		"mr		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r5, 0x68(%%r1);"	// restore r5
		"ld		%%r6, 0x60(%%r1);"	// restore r6
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "r" (count) , [size] "r" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3 - r4
static inline __attribute__((always_inline)) void _cellGcmReserveReg1(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"add	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"std	%%r4, 0x70(%%r1);"	// save r4
		"mr		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r4, 0x70(%%r1);"	// restore r4
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "r" (count) , [size] "r" (size)
		: "r11","r12","memory");
}

//	Save/Restore r3
static inline __attribute__((always_inline)) void _cellGcmReserveReg0(uint32_t count)
{
	uint32_t size = count*sizeof(uint32_t);	// byte size

	__asm__ (
		"lwz	%%r12, 4(%%r3);"	// end
		"lwz	%%r11, 8(%%r3);"	// current
		"add	%%r11, %%r11, %[size];"	// current + size
		"cmplw	%%r11, %%r12;"		// compare current vs end
		"ble+	_cellGcmDone%=;"	// if(current <= end) goto done
		"lwz	%%r12, 12(%%r3);"	// callback
		"mflr	%%r0;"				// move from lr
		"std	%%r0, 0x90(%%r1);"	// save lr
		"std	%%r3, 0x78(%%r1);"	// save r3
		"mr		%%r4, %[count];"	// r4 = count
		"std	%%r2, 40(%%r1);"	// save toc
		"lwz	%%r0, 0(%%r12);"	// load callback ptr
		"lwz	%%r2, 4(%%r12);"	// load callback toc
		"mtctr	%%r0;"				// move callback to ctr
		"bctrl;"					// call *callback
		"cmpwi	%%r3,0;"			// compare r3 vs zero
		"ld		%%r3, 0x78(%%r1);"	// restore r3
		"ld		%%r2, 40(%%r1);"	// restore toc
		"ld		%%r0, 0x90(%%r1);"	// restore lr
		"mtlr	%%r0;"				// move to lr
		"beq	_cellGcmDone%=;"	// if(ret == CELL_OK) goto done
		"addi	%%r1, %%r1, 128;"	// stack frame
		"blr;"						// if(ret != CELL_OK) return
		"_cellGcmDone%=:"
		: // no output
		: [count] "r" (count) , [size] "r" (size)
		: "r11","r12","memory");
}

//
//	Stack Frame In
//
static inline __attribute__((always_inline)) void _cellGcmStackIn(void)
{
	__asm__ volatile("stdu    %r1,-128(%r1);");	// size = 128B (0x80)
}

//
//	Stack Frame Out
//
static inline __attribute__((always_inline)) void _cellGcmStackOut(void)
{
	__asm__ volatile("addi    %r1,%r1,128;");	// size = 128B (0x80)
}


/********************************************************************/
/*																	*/
/*		CELL_GCM_METHOD replace inline functions					*/
/*																	*/
/********************************************************************/

//
//	_cellGcmMethodSetDepthBoundsMinMax()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetDepthBoundsMinMax(uint32_t *current, float zmin, float zmax)
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_DEPTH_BOUNDS_MIN, 2);
	__asm__ volatile(
		"stw	%1, 0(%0);"
		"stfs	%2, 4(%0);"
		"stfs	%3, 8(%0);"
		"addi	%0, %0, 12;"
		"stw	%0, 8(%%r3);"
		: : "b" (current), "r" (method), "f" (zmin), "f" (zmax) : "memory");
}

//
//	_cellGcmMethodSetPointSize()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetPointSize(uint32_t *current, float size)
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POINT_SIZE, 1);
	__asm__ volatile (
		"stw	%1, 0(%0);"
		"stfs	%2, 4(%0);"
		"addi	%0, %0, 8;"
		"stw	%0, 8(%%r3);"
		: : "b" (current), "r" (method), "f" (size) : "memory");
}

//
//	_cellGcmMethodSetPolygonOffsetScaleFactorBias()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetPolygonOffsetScaleFactorBias(uint32_t *current, float factor, float units)
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_POLYGON_OFFSET_SCALE_FACTOR, 2);
	__asm__ volatile (
		"stw	%1, 0(%0);"
		"stfs	%2, 4(%0);"
		"stfs	%3, 8(%0);"
		"addi	%0, %0, 12;"
		"stw	%0, 8(%%r3);"
		: : "b" (current), "r" (method), "f" (factor), "f" (units) : "memory");
}

//
//	_cellGcmMethodSetFogParams()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetFogParams(uint32_t *current, float p0, float p1)
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_FOG_PARAMS, 2);
	__asm__ volatile (
		"stw	%1, 0(%0);"
		"stfs	%2, 4(%0);"
		"stfs	%3, 8(%0);"
		"addi	%0, %0, 12;"
		"stw	%0, 8(%%r3);"
		: : "b" (current), "r" (method), "f" (p0), "f" (p1) : "memory");
}

//
//	_cellGcmMethodSetClipMinMax()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetClipMinMax(uint32_t *current, float min, float max)
{
	__asm__ volatile (
		"lis	%%r0, 2<<2;"
		"ori	%%r0, %%r0, %[method0];"
		"stw	%%r0,	0(%[current]);"		// clip method
		"stfs	%[min], 4(%[current]);"		// clip min
		"stfs	%[max], 8(%[current]);"		// clip max
		"addi	%[current], %[current], 12;"// current += 3 words
		"stw	%[current], 8(%%r3);"		// update current
		:
		:[current] "b" (current),
		 [min] "f" (min), [max] "f" (max),
		 [method0] "i" (CELL_GCM_NV4097_SET_CLIP_MIN)
		:"r0","memory");
}

//
//	_cellGcmMethodSetViewport()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetViewport(uint32_t *current, uint16_t x, uint16_t y, uint16_t w, uint16_t h, float min, float max, const float scale[4], const float offset[4])
{
	__asm__ volatile (
		"lfs	%%f3, 0(%[offset]);"		// offset[0]
		"lfs	%%f4, 4(%[offset]);"		// offset[1]
		"lfs	%%f5, 8(%[offset]);"		// offset[2]
		"lfs	%%f6, 12(%[offset]);"		// offset[3]
		"lfs	%%f7, 0(%[scale]);"			// scale[0]
		"lfs	%%f8, 4(%[scale]);"			// scale[1]
		"lfs	%%f9, 8(%[scale]);"			// scale[2]
		"lfs	%%f10, 12(%[scale]);"		// scale[3]
		"rlwinm %[w], %[w], 16,0,15;" 		// ((w)<<16)&0xffff
		"rlwinm %[h], %[h], 16,0,15;" 		// ((h)<<16)&0xffff
		"or		%[x], %[x], %[w];"			// (x)|(w)
		"or		%[y], %[y], %[h];"			// (y)|(h)
		"lis	%%r0, 2<<2;"
		"ori	%%r0, %%r0, %[method0];"
		"stw	%%r0, 0(%[current]);"		// viewport method
		"stw	%[x], 4(%[current]);"		// viewport x,w
		"stw	%[y], 8(%[current]);"		// viewport y,h
		"lis	%%r0, 2<<2;"
		"ori	%%r0, %%r0, %[method1];"
		"stw	%%r0,	12(%[current]);"	// clip method
		"stfs	%[min], 16(%[current]);"	// clip min
		"stfs	%[max], 20(%[current]);"	// clip max
		"lis	%%r0, 8<<2;"
		"ori	%%r0, %%r0, %[method2];"
		"stw	%%r0, 24(%[current]);"		// offset method (1st)
		"stfs	%%f3, 28(%[current]);"		// offset[0]
		"stfs	%%f4, 32(%[current]);"		// offset[1]
		"stfs	%%f5, 36(%[current]);"		// offset[2]
		"stfs	%%f6, 40(%[current]);"		// offset[3]
		"stfs	%%f7, 44(%[current]);"		// scale[0]
		"stfs	%%f8, 48(%[current]);"		// scale[1]
		"stfs	%%f9, 52(%[current]);"		// scale[2]
		"stfs	%%f10, 56(%[current]);"		// scale[3]
		"stw	%%r0, 60(%[current]);"		// offset method (2st)
		"stfs	%%f3, 64(%[current]);"		// offset[0]
		"stfs	%%f4, 68(%[current]);"		// offset[1]
		"stfs	%%f5, 72(%[current]);"		// offset[2]
		"stfs	%%f6, 76(%[current]);"		// offset[3]
		"stfs	%%f7, 80(%[current]);"		// scale[0]
		"stfs	%%f8, 84(%[current]);"		// scale[1]
		"stfs	%%f9, 88(%[current]);"		// scale[2]
		"stfs	%%f10, 92(%[current]);"		// scale[3]
		"addi	%[current], %[current], 96;"// current += 24 words
		"stw	%[current], 8(%%r3);"		// update current
		:
		: [current] "b" (current), [scale] "b" (scale), [offset] "b" (offset),
		  [x] "r" (x), [y] "r" (y), [w] "r" (w), [h] "r" (h),
		  [min] "f" (min), [max] "f" (max),
		  [method0] "i" (CELL_GCM_NV4097_SET_VIEWPORT_HORIZONTAL),
		  [method1] "i" (CELL_GCM_NV4097_SET_CLIP_MIN),
		  [method2] "i" (CELL_GCM_NV4097_SET_VIEWPORT_OFFSET)
		: "r0","memory");
}

//
//	_cellGcmMethodSetTransformConstantLoadCountValue()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetTransformConstantLoadCountValue(uint32_t *current, uint32_t load, uint32_t count, const float * __restrict value)
{
	__asm__ volatile (
		"lwz	%%r8, 8(%%r3);"		// current
		"srwi	%%r7, %[count], 5;"	// loop = count >> 5
		"cmpwi	%%r7, 0;"
		"beq	_cellGcmRest%=;"	// if(loop==0) goto rest
		"mtctr	%%r7;"				// ctr = loop
		"lis	%%r7, 33<<2;"
		"ori	%%r7, %%r7, %[method];"		// method header
		"_cellGcmLoop%=:"
		"stw	%%r7, 0(%%r8);"		// constant method
		"stwu	%[load], 4(%%r8);"	// constant load
		"lfs	%%f0, 0(%[value]);"
		"lfsu	%%f1, 4(%[value]);"
		"lfsu	%%f2, 4(%[value]);"
		"lfsu	%%f3, 4(%[value]);"
		"lfsu	%%f4, 4(%[value]);"
		"lfsu	%%f5, 4(%[value]);"
		"lfsu	%%f6, 4(%[value]);"
		"lfsu	%%f7, 4(%[value]);"
		"stfsu	%%f0, 4(%%r8);"
		"stfsu	%%f1, 4(%%r8);"
		"stfsu	%%f2, 4(%%r8);"
		"stfsu	%%f3, 4(%%r8);"
		"stfsu	%%f4, 4(%%r8);"
		"stfsu	%%f5, 4(%%r8);"
		"stfsu	%%f6, 4(%%r8);"
		"stfsu	%%f7, 4(%%r8);"
		"lfsu	%%f0, 4(%[value]);"
		"lfsu	%%f1, 4(%[value]);"
		"lfsu	%%f2, 4(%[value]);"
		"lfsu	%%f3, 4(%[value]);"
		"lfsu	%%f4, 4(%[value]);"
		"lfsu	%%f5, 4(%[value]);"
		"lfsu	%%f6, 4(%[value]);"
		"lfsu	%%f7, 4(%[value]);"
		"stfsu	%%f0, 4(%%r8);"
		"stfsu	%%f1, 4(%%r8);"
		"stfsu	%%f2, 4(%%r8);"
		"stfsu	%%f3, 4(%%r8);"
		"stfsu	%%f4, 4(%%r8);"
		"stfsu	%%f5, 4(%%r8);"
		"stfsu	%%f6, 4(%%r8);"
		"stfsu	%%f7, 4(%%r8);"
		"lfsu	%%f0, 4(%[value]);"
		"lfsu	%%f1, 4(%[value]);"
		"lfsu	%%f2, 4(%[value]);"
		"lfsu	%%f3, 4(%[value]);"
		"lfsu	%%f4, 4(%[value]);"
		"lfsu	%%f5, 4(%[value]);"
		"lfsu	%%f6, 4(%[value]);"
		"lfsu	%%f7, 4(%[value]);"
		"stfsu	%%f0, 4(%%r8);"
		"stfsu	%%f1, 4(%%r8);"
		"stfsu	%%f2, 4(%%r8);"
		"stfsu	%%f3, 4(%%r8);"
		"stfsu	%%f4, 4(%%r8);"
		"stfsu	%%f5, 4(%%r8);"
		"stfsu	%%f6, 4(%%r8);"
		"stfsu	%%f7, 4(%%r8);"
		"lfsu	%%f0, 4(%[value]);"
		"lfsu	%%f1, 4(%[value]);"
		"lfsu	%%f2, 4(%[value]);"
		"lfsu	%%f3, 4(%[value]);"
		"lfsu	%%f4, 4(%[value]);"
		"lfsu	%%f5, 4(%[value]);"
		"lfsu	%%f6, 4(%[value]);"
		"lfsu	%%f7, 4(%[value]);"
		"stfsu	%%f0, 4(%%r8);"
		"stfsu	%%f1, 4(%%r8);"
		"stfsu	%%f2, 4(%%r8);"
		"stfsu	%%f3, 4(%%r8);"
		"stfsu	%%f4, 4(%%r8);"
		"stfsu	%%f5, 4(%%r8);"
		"stfsu	%%f6, 4(%%r8);"
		"stfsu	%%f7, 4(%%r8);"
		"addi	%[load], %[load], 8;"
		"addi	%%r8, %%r8, 4;"
		"addi	%[value], %[value], 4;"
		"bdnz _cellGcmLoop%=;"			// if(ctr!=0) goto loop
		"_cellGcmRest%=:"
		"clrlwi	%%r7, %[count], 27;"	// rest = count & 0x1f
		"cmpwi	%%r7, 0;"
		"beq	_cellGcmDone%=;"		// if(rest==0) goto done
		"mtctr	%%r7;"					// ctr = rest
		"addi	%%r7, %%r7, 1;"			// rest = rest + 1
		"slwi	%%r7, %%r7, 18;"		// rest << 18
		"ori	%%r7, %%r7, %[method];"	// (rest)|(method)
		"stw	%%r7, 0(%%r8);"		// constant method
		"stw	%[load], 4(%%r8);"	// constant load
		"addi	%%r8, %%r8, 8;"
		"_cellGcmRestLoop%=:"
		"lfs	%%f0, 0(%[value]);"
		"stfs	%%f0, 0(%%r8);"
		"addi	%[value], %[value], 4;"
		"addi	%%r8, %%r8, 4;"
		"bdnz _cellGcmRestLoop%=;"		// if(ctr!=0) goto rest loop
		"_cellGcmDone%=:"
		"stw	%%r8, 8(%%r3);"			// update current
		:
		: [method] "i" (CELL_GCM_NV4097_SET_TRANSFORM_CONSTANT_LOAD),
		  [load] "r" (load), [count] "r" (count), [value] "b" (value)
		: "r7", "r8", "memory");
}

//
//	_cellGcmMethodSetSkipNop()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetSkipNop(uint32_t *current, uint32_t count)
{
	__asm__ volatile (
		"lwz	%%r8, 8(%%r3);"				// load current
		"ori	%%r7, %[count], 0x1000;"	// (ni)|(count)
		"slwi	%%r7, %%r7, 18;"			// <<18
		"ori	%%r7, %%r7, %[method];"		// |(method)
		"stw	%%r7, 0(%%r8);"				// 3dnop method
		"addi	%[count], %[count], 1;"		// count+1
		"slwi	%[count], %[count], 2;"		// <<2
		"add	%%r8, %%r8, %[count];"		// current += count+1
		"stw	%%r8, 8(%%r3);"				// save current
		:
		: [method] "i" (CELL_GCM_NV4097_NO_OPERATION),
		  [count] "r" (count)
		: "r7", "r8", "memory");
}

//
//	_cellGcmMethodSetVertexData4f()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetVertexData4f(uint32_t *current, uint8_t index, const float value[4])
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA4F_M + (index * 4*4), 4);
	__asm__ volatile (
		"lfs	%%f0, 0(%[value]);"			// ld value[0]
		"lfs	%%f1, 4(%[value]);"			// ld value[1]
		"lfs	%%f2, 8(%[value]);"			// ld value[2]
		"lfs	%%f3, 12(%[value]);"		// ld value[3]
		"stw	%[method], 0(%[current]);"	// st method
		"stfs	%%f0, 4(%[current]);"		// st value[0]
		"stfs	%%f1, 8(%[current]);"		// st value[1]
		"stfs	%%f2, 12(%[current]);"		// st value[2]
		"stfs	%%f3, 16(%[current]);"		// st value[3]
		"addi	%[current], %[current], 20;"// current += 5 words
		"stw	%[current], 8(%%r3);"		// update current
		:
		: [method] "r" (method),
		  [current] "b" (current), [index] "r" (index), [value] "b" (value)
		: "memory");
}

//
//	_cellGcmMethodSetVertexData3f()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetVertexData3f(uint32_t *current, uint8_t index, const float value[3])
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA3F_M + (index * 4*4), 3);
	__asm__ volatile (
		"lfs	%%f0, 0(%[value]);"			// ld value[0]
		"lfs	%%f1, 4(%[value]);"			// ld value[1]
		"lfs	%%f2, 8(%[value]);"			// ld value[2]
		"stw	%[method], 0(%[current]);"	// st method
		"stfs	%%f0, 4(%[current]);"		// st value[0]
		"stfs	%%f1, 8(%[current]);"		// st value[1]
		"stfs	%%f2, 12(%[current]);"		// st value[2]
		"addi	%[current], %[current], 16;"// current += 4 words
		"stw	%[current], 8(%%r3);"		// update current
		:
		: [method] "r" (method),
		  [current] "b" (current), [index] "r" (index), [value] "b" (value)
		: "memory");
}

//
//	_cellGcmMethodSetVertexData2f()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetVertexData2f(uint32_t *current, uint8_t index, const float value[2])
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA2F_M + (index * 4*2), 2);
	__asm__ volatile (
		"lfs	%%f0, 0(%[value]);"			// ld value[0]
		"lfs	%%f1, 4(%[value]);"			// ld value[1]
		"stw	%[method], 0(%[current]);"	// st method
		"stfs	%%f0, 4(%[current]);"		// st value[0]
		"stfs	%%f1, 8(%[current]);"		// st value[1]
		"addi	%[current], %[current], 12;"// current += 3 words
		"stw	%[current], 8(%%r3);"		// update current
		:
		: [method] "r" (method),
		  [current] "b" (current), [index] "r" (index), [value] "b" (value)
		: "memory");
}

//
//	_cellGcmMethodSetVertexData1f()
//
static inline __attribute__((always_inline)) void _cellGcmMethodSetVertexData1f(uint32_t *current, uint8_t index, const float value)
{
	register uint32_t method = CELL_GCM_METHOD(CELL_GCM_NV4097_SET_VERTEX_DATA1F_M + (index * 4), 1);
	__asm__ volatile (
		"stw	%[method], 0(%[current]);"	// st method
		"stfs	%[value],  4(%[current]);"	// st value
		"addi	%[current], %[current], 8;"// current += 2 words
		"stw	%[current], 8(%%r3);"		// update current
		:
		: [method] "r" (method),
		  [current] "b" (current), [index] "r" (index), [value] "f" (value)
		: "memory");
}


/********************************************************************/
/*																	*/
/*		redefine macros in gcm_function_macros.h					*/
/*																	*/
/********************************************************************/
#undef	CELL_GCM_ASM_RESERVE_IMM
#undef	CELL_GCM_ASM_RESERVE_REG
#undef	CELL_GCM_ASM_IN
#undef	CELL_GCM_ASM_OUT

#if	CELL_GCM_UNSAFE
#define	CELL_GCM_ASM_RESERVE_IMM(count, args)
#define	CELL_GCM_ASM_RESERVE_REG(count, args)
#define	CELL_GCM_ASM_IN()
#define	CELL_GCM_ASM_OUT()
#else
#define	CELL_GCM_ASM_RESERVE_IMM(count, args)		_cellGcmReserveImm## args(count)
#define	CELL_GCM_ASM_RESERVE_REG(count, args)		_cellGcmReserveReg## args(count)
#define	CELL_GCM_ASM_IN()	_cellGcmStackIn()
#define	CELL_GCM_ASM_OUT()	_cellGcmStackOut()
#endif

/********************************************************************/
/*																	*/
/*		redefine macros in gcm_methos.h								*/
/*																	*/
/********************************************************************/
#undef	CELL_GCM_METHOD_SET_DEPTH_BOUNDS_MIN_MAX
#undef	CELL_GCM_METHOD_SET_POINT_SIZE
#undef	CELL_GCM_METHOD_SET_POLYGON_OFFSET_SCALE_FACTOR_BIAS
#undef	CELL_GCM_METHOD_SET_FOG_PARAMS
#undef	CELL_GCM_METHOD_SET_CLIP_MIN_MAX
#undef	CELL_GCM_METHOD_SET_VIEWPORT
#undef	CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_COUNT_VALUE
#undef	CELL_GCM_METHOD_SET_SKIP_NOP
#undef	CELL_GCM_METHOD_SET_VERTEX_DATA4F
#undef	CELL_GCM_METHOD_SET_VERTEX_DATA3F
#undef	CELL_GCM_METHOD_SET_VERTEX_DATA2F
#undef	CELL_GCM_METHOD_SET_VERTEX_DATA1F

#define CELL_GCM_METHOD_SET_DEPTH_BOUNDS_MIN_MAX(current, zmin, zmax)\
	_cellGcmMethodSetDepthBoundsMinMax(current, zmin, zmax)

#define	CELL_GCM_METHOD_SET_POINT_SIZE(current, size)\
	_cellGcmMethodSetPointSize(current, size)

#define	CELL_GCM_METHOD_SET_POLYGON_OFFSET_SCALE_FACTOR_BIAS(current, factor, units)\
	_cellGcmMethodSetPolygonOffsetScaleFactorBias(current, factor, units)

#define	CELL_GCM_METHOD_SET_FOG_PARAMS(current, p0, p1)\
	_cellGcmMethodSetFogParams(current, p0, p1)

#define CELL_GCM_METHOD_SET_CLIP_MIN_MAX(current, min, max)\
	_cellGcmMethodSetClipMinMax(current, min, max)

#define	CELL_GCM_METHOD_SET_VIEWPORT(current, x, y, w, h, min, max, scale, offset)\
	_cellGcmMethodSetViewport(current, x, y, w, h, min, max, scale, offset)

#define CELL_GCM_METHOD_SET_TRANSFORM_CONSTANT_LOAD_COUNT_VALUE(current, load, count, value) \
	_cellGcmMethodSetTransformConstantLoadCountValue(current, load, count, value)

#define CELL_GCM_METHOD_SET_SKIP_NOP(current, count) \
	_cellGcmMethodSetSkipNop(current, count)

#define CELL_GCM_METHOD_SET_VERTEX_DATA4F(current, index, value) \
	_cellGcmMethodSetVertexData4f(current, index, value)

#define CELL_GCM_METHOD_SET_VERTEX_DATA3F(current, index, value) \
	_cellGcmMethodSetVertexData3f(current, index, value)

#define CELL_GCM_METHOD_SET_VERTEX_DATA2F(current, index, value) \
	_cellGcmMethodSetVertexData2f(current, index, value)

#define CELL_GCM_METHOD_SET_VERTEX_DATA1F(current, index, value) \
	_cellGcmMethodSetVertexData1f(current, index, value)

#endif /* __CELL_GCM_IMPLEMENTATION_ASM_H__ */

