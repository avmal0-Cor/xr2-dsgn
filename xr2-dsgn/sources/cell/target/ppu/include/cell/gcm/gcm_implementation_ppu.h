/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2006 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#if (CELL_GCM_INLINE == 0)
CELL_GCM_DECL void CELL_GCM_FUNC(Flush)(CELL_GCM_NO_ARGS())
{
	CellGcmControl volatile *control = cellGcmGetControlRegister();

	// Out-of-order write protection.
	// this needs to be sync, not eieio as command buffer is on main memory(which is cached)
	// but control registers are mapped as cache inhibited, eieio doesn't gurantee order 
	// between cached and cache inhibited region
	#ifdef __SNC__
	__builtin_sync();
	#else
	__asm__ volatile("sync");
	#endif // __SNC__

    // Make sure that the current location is mapped
    uint32_t offsetInBytes;
    if(cellGcmAddressToOffset(CELL_GCM_CURRENT, &offsetInBytes) != CELL_OK)
	{
        CELL_GCM_ASSERT(0);
	}

    control->put = offsetInBytes;
}

CELL_GCM_DECL void CELL_GCM_FUNC(Finish)(CELL_GCM_ARGS(uint32_t referenceValue))
{
    CELL_GCM_FUNC(SetReferenceCommand)(CELL_GCM_ARGS_FUNC(referenceValue));
    CELL_GCM_FUNC(Flush)(CELL_GCM_NO_ARGS_FUNC());

	CellGcmControl volatile *control = cellGcmGetControlRegister();
    while( control->ref != referenceValue )
    {
		// Don't be a ppu hog ;)
	    sys_timer_usleep(30);
    }
}
#else	// (CELL_GCM_INLINE == 1)

// call library function even if inline version
CELL_GCM_DECL void CELL_GCM_FUNC(Flush)(CELL_GCM_NO_ARGS())
{
#if (CELL_GCM_UNSAFE == 0)
	cellGcmFlush(CELL_GCM_NO_ARGS_FUNC());
#else
	cellGcmFlushUnsafe(CELL_GCM_NO_ARGS_FUNC());
#endif
}

// call library function even if inline version
CELL_GCM_DECL void CELL_GCM_FUNC(Finish)(CELL_GCM_ARGS(uint32_t referenceValue))
{
#if (CELL_GCM_UNSAFE == 0)
	cellGcmFinish(CELL_GCM_ARGS_FUNC(referenceValue));
#else
	cellGcmFinishUnsafe(CELL_GCM_ARGS_FUNC(referenceValue));
#endif
}

#endif


CELL_GCM_DECL void CELL_GCM_FUNC(ResetDefaultCommandBuffer)(CELL_GCM_NO_ARGS())
{
	uint32_t offset = CELL_GCM_INIT_STATE_OFFSET;
	CELL_GCM_FUNC(SetJumpCommand)(CELL_GCM_ARGS_FUNC(offset));

#ifdef __SNC__
	__builtin_sync();
#else
	__asm__ volatile("sync");
#endif // __SNC__

	// wait until jump to offset
	CellGcmControl volatile *control = cellGcmGetControlRegister();

	control->put = offset;
	while(control->get != offset){
		sys_timer_usleep(30);
	}

	// to be safe for gcm context
	cellGcmSetDefaultFifoSize(cellGcmGetDefaultCommandWordSize(), cellGcmGetDefaultSegmentWordSize());
}
