/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_DAISY_RETURN_CODE_H__
#define __CELL_DAISY_RETURN_CODE_H__

#include <sys/return_code.h>

namespace cell {
	namespace Daisy {

		enum ReturnCode {

			/* The queue operation cannot be executed immediately. */
			QUEUE_IS_BUSY	     = 2,

			/* producer is terminated */
			TERMINATED           = 3,

			/* Glue is active */
			GLUE_ACTIVE          = 4
		};

	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_RETURN_CODE_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
