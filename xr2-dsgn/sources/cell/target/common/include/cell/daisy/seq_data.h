/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

/* TaggedData - Data container with tag
 */

#ifndef __CELL_DAISY_SEQ_DATA_H__
#define __CELL_DAISY_SEQ_DATA_H__

namespace cell {
	namespace Daisy {
        template<typename tDataType>
			class TaggedData {

			public:

                // tag
                uint32_t mTag __attribute__((aligned(128)));
                
                // data content
				__extension__ union {
					tDataType mContent;
					uint8_t   mEnlargeToAlign[((sizeof(tDataType) + 127) / 128) * 128];
				} __attribute__((aligned(128)));
                
			}; /* class TaggedData */
	} /* namespace Daisy */
} /* namespace cell */

#endif /* __CELL_DAISY_SEQ_DATA_H__ */

/*
 * Local Variables:
 * mode:C++
 * tab-width:4
 * End:
 * vim:ts=4:sw=4:
 */
		                
