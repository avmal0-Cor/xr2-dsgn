/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef	__CELL_SAIL_DESCRIPTOR_H__
#define	__CELL_SAIL_DESCRIPTOR_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif


typedef struct CellSailDesriptor {
    uint64_t internalData[32];
} CellSailDescriptor;


int cellSailDescriptorGetStreamType(CellSailDescriptor *pSelf);
const char* cellSailDescriptorGetUri(CellSailDescriptor *pSelf);
void* cellSailDescriptorGetMediaInfo(CellSailDescriptor *pSelf);

int cellSailDescriptorSetAutoSelection(CellSailDescriptor *pSelf,
                                       bool                autoSelection);
int cellSailDescriptorIsAutoSelection(CellSailDescriptor *pSelf);


int cellSailDescriptorCreateDatabase(CellSailDescriptor *pSelf,
                                     void               *pDatabase,
                                     size_t              size,
                                     uint64_t            arg);
int cellSailDescriptorDestroyDatabase(CellSailDescriptor *pSelf,
                                      void               *pDatabase);

int cellSailDescriptorOpen(CellSailDescriptor *pSelf);
int cellSailDescriptorClose(CellSailDescriptor *pSelf);


int cellSailDescriptorSetEs(CellSailDescriptor *pSelf,
                            int                 type,
                            unsigned            index,
                            bool                enable,
                            void               *pSpecifier);

int cellSailDescriptorClearEs(CellSailDescriptor *pSelf,
                              int                 type,
                              unsigned            index);


int cellSailDescriptorGetCapabilities(CellSailDescriptor *pSelf,
                                      uint32_t           *pStartCapabilities,
                                      uint32_t           *pSeekCapabilities,
                                      uint32_t           *pTerminusCapabilities,
                                      uint32_t           *pAvailableFlags);
int cellSailDescriptorInquireCapability(CellSailDescriptor   *pSelf,
                                        CellSailStartCommand *pCommand);


enum {
    CELL_SAIL_DESCRIPTOR_PARAMETER_METADATA_PARSING = 1,
};

enum {
    CELL_SAIL_DESCRIPTOR_PARAMETER_OMIT_EP_MAP = 8,
};

int cellSailDescriptorSetParameter(CellSailDescriptor *pSelf,
                                   int                 type,
                                   uint64_t            param0,
                                   uint64_t            param1);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* __CELL_SAIL_DESCRIPTOR_H__ */
/*[eof]*/
