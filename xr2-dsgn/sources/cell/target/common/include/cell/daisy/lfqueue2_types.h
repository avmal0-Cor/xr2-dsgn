/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#ifndef __DAISY_LFQUEUE2_TYPES_H__
#define __DAISY_LFQUEUE2_TYPES_H__ 1

namespace cell {
	namespace Daisy {
		typedef struct  LFQueue2 {
			unsigned char skip[256];
		} __attribute__((aligned(128))) LFQueue2;
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __DAISY_LFQUEUE2_TYPES_H__ */

/*
 * Local Variables:
 * mode: C++
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
