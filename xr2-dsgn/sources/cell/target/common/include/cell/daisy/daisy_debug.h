/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* libdaisy debug macro definitions */

#include <cell/daisy/snprintf.h>
#include <spu_printf.h>

/* CELL_DAISY_DEBUG_PRINTF */
#undef CELL_DAISY_DEBUG_PRINTF

#ifdef __SPU__
#define CELL_DAISY_DEBUG_PRINTF(...)\
do{\
	char __buf[256];\
	cell::Daisy::_snprintf(__buf, 256, __VA_ARGS__);\
	spu_printf("[%s]: %s", cell::Daisy::sCellDaisyTaskName, __buf);\
}while(0)
#else /* __SPU__ */
#define CELL_DAISY_DEBUG_PRINTF(...) \
do{ \
	char __buf[256];\
	cell::Daisy::_snprintf(__buf, 256, __VA_ARGS__);\
	printf("[%s]: %s", cell::Daisy::sCellDaisyTaskName, __buf);\
}while(0)
#endif /* __SPU__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
