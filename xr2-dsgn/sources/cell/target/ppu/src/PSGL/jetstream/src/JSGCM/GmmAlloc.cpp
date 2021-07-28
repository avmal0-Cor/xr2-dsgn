/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <cell/gcm.h>
#include <JSGCM/GmmAlloc.h>
#include <JSGCM/JSGcmUtils.h>

static GmmAllocator         *pGmmLocalAllocator = NULL;
static GmmAllocator         *pGmmMainAllocator = NULL;
static volatile uint32_t    *pLock = NULL;
static uint32_t             cachedLockValue = 0;
static uint8_t              pinAllocations = 0;
static GmmFixedAllocData    *pGmmFixedAllocData = NULL;
static uint32_t             RSXWaitLabelValue = 1;
#ifdef GMM_DEBUG
static uint32_t             uniqueAllocationId = 0;
#endif

static inline uint32_t pad(
    uint32_t x, 
    uint32_t pad
)
{
    return ( x + pad - 1 ) / pad * pad;
}

static inline uint32_t gmmAddressToOffset(
    uint32_t address,
    uint8_t isMain
)
{
    return isMain ? address-pGmmMainAllocator->memoryBase : 
                    address-pGmmLocalAllocator->memoryBase;
}

static uint32_t gmmInitFixedAllocator()
{
#ifndef GMM_NO_ERROR_CHECK
    if (pGmmFixedAllocData)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }
#endif 

	// allocate fixed allocator
    pGmmFixedAllocData = (GmmFixedAllocData *)malloc(sizeof(GmmFixedAllocData));

    if (pGmmFixedAllocData == NULL)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }

    memset(pGmmFixedAllocData, 0, sizeof(GmmFixedAllocData));

    for (int i=0; i<2; i++)
    {
        int blockCount = (i==0) ? GMM_BLOCK_COUNT : GMM_TILE_BLOCK_COUNT;
        int blockSize  = (i==0) ? sizeof(GmmBlock): sizeof(GmmTileBlock);

        pGmmFixedAllocData->ppBlockList[i] = (char **)malloc(sizeof(char *));
        if (pGmmFixedAllocData->ppBlockList[i] == NULL)
        {
            GMM_ASSERT(0);
            return GMM_ERROR;
        }

		// allocate a list of block descriptors
        pGmmFixedAllocData->ppBlockList[i][0] = (char *)malloc(blockSize * blockCount);
        if (pGmmFixedAllocData->ppBlockList[i][0] == NULL)
        {
            GMM_ASSERT(0);
            return GMM_ERROR;
        }

        pGmmFixedAllocData->ppFreeBlockList[i] = (uint16_t **)malloc(sizeof(uint16_t *));
        if (pGmmFixedAllocData->ppFreeBlockList[i] == NULL)
        {
            GMM_ASSERT(0);
            return GMM_ERROR;
        }

        pGmmFixedAllocData->ppFreeBlockList[i][0] = (uint16_t *)malloc(sizeof(uint16_t) * blockCount);
        if (pGmmFixedAllocData->ppFreeBlockList[i][0] == NULL)
        {
            GMM_ASSERT(0);
            return GMM_ERROR;
        }

        pGmmFixedAllocData->pBlocksUsed[i] = (uint16_t *)malloc(sizeof(uint16_t));
        if (pGmmFixedAllocData->pBlocksUsed[i] == NULL)
        {
            GMM_ASSERT(0);
            return GMM_ERROR;
        }

        for (int j=0; j<blockCount; j++)
        {
            pGmmFixedAllocData->ppFreeBlockList[i][0][j] = j; 
        }

        pGmmFixedAllocData->pBlocksUsed[i][0] = 0;
        pGmmFixedAllocData->BlockListCount[i] = 1;
    }

    return CELL_OK;
}

static void *gmmAllocFixed(uint8_t isTile)
{
    int blockCount = isTile ? GMM_TILE_BLOCK_COUNT : GMM_BLOCK_COUNT;
    int blockSize  = isTile ? sizeof(GmmTileBlock) : sizeof(GmmBlock);
    int listCount  = pGmmFixedAllocData->BlockListCount[isTile];

	// return if there is a free block in the fixed allocator
    for (int i=0; i<listCount; i++)
    {
        GMM_ASSERT(pGmmFixedAllocData->pBlocksUsed[isTile][i] <= blockCount);

        if (pGmmFixedAllocData->pBlocksUsed[isTile][i] < blockCount)
        {
            return pGmmFixedAllocData->ppBlockList[isTile][i] + 
                   (pGmmFixedAllocData->ppFreeBlockList[isTile][i][pGmmFixedAllocData->pBlocksUsed[isTile][i]++] * 
                    blockSize);
        }
    }

	// add a new list of block descriptor
    char **ppBlockList = 
        (char **)realloc(pGmmFixedAllocData->ppBlockList[isTile],
                         (listCount + 1) * sizeof(char *));
    if (ppBlockList == NULL)
    {
        GMM_ASSERT(0);
        return NULL;
    }
    pGmmFixedAllocData->ppBlockList[isTile] = ppBlockList;

	// allocate anothor set of block descriptors
    pGmmFixedAllocData->ppBlockList[isTile][listCount] = 
        (char *)malloc(blockSize * blockCount);
    if (pGmmFixedAllocData->ppBlockList[isTile][listCount] == NULL)
    {
        GMM_ASSERT(0);
        return NULL;
    }

    uint16_t **ppFreeBlockList = 
        (uint16_t **)realloc(pGmmFixedAllocData->ppFreeBlockList[isTile],
                            (listCount + 1) * sizeof(uint16_t *));
    if (ppFreeBlockList == NULL)
    {
        GMM_ASSERT(0);
        return NULL;
    }
    pGmmFixedAllocData->ppFreeBlockList[isTile] = ppFreeBlockList;

    pGmmFixedAllocData->ppFreeBlockList[isTile][listCount] = 
        (uint16_t *)malloc(sizeof(uint16_t) * blockCount);
    if (pGmmFixedAllocData->ppFreeBlockList[isTile][listCount] == NULL)
    {
        GMM_ASSERT(0);
        return NULL;
    }

    uint16_t *pBlocksUsed = 
        (uint16_t *)realloc(pGmmFixedAllocData->pBlocksUsed[isTile],
                            (listCount + 1) * sizeof(uint16_t));
    if (pBlocksUsed == NULL)
    {
        GMM_ASSERT(0);
        return NULL;
    }
    pGmmFixedAllocData->pBlocksUsed[isTile] = pBlocksUsed;
                                
    for (int i=0; i<blockCount; i++)
    {
        pGmmFixedAllocData->ppFreeBlockList[isTile][listCount][i] = i; 
    }

    pGmmFixedAllocData->pBlocksUsed[isTile][listCount] = 0;
    pGmmFixedAllocData->BlockListCount[isTile]++;

    return pGmmFixedAllocData->ppBlockList[isTile][listCount] + 
           (pGmmFixedAllocData->ppFreeBlockList[isTile][listCount][pGmmFixedAllocData->pBlocksUsed[isTile][listCount]++] * 
            blockSize);
}

static void gmmFreeFixed(uint8_t isTile, void *pBlock)
{
    int blockCount = isTile ? GMM_TILE_BLOCK_COUNT : GMM_BLOCK_COUNT;
    int blockSize  = isTile ? sizeof(GmmTileBlock) : sizeof(GmmBlock);
    uint8_t found = 0;

#ifdef GMM_DEBUG
    // write all 0xFFFFFFFF to trap anybody who has stale reference to freed block!
    memset(pBlock, 0xFFFFFFFF, blockSize);
#endif

    for (int i=0; i<pGmmFixedAllocData->BlockListCount[isTile]; i++)
    {
        if (pBlock >= pGmmFixedAllocData->ppBlockList[isTile][i] &&
            pBlock < (pGmmFixedAllocData->ppBlockList[isTile][i] + blockSize * blockCount))
        {
            int index = ((char *)pBlock - pGmmFixedAllocData->ppBlockList[isTile][i]) / blockSize;
            pGmmFixedAllocData->ppFreeBlockList[isTile][i][--pGmmFixedAllocData->pBlocksUsed[isTile][i]] = index;
            found = 1;
        }
    }

    GMM_ASSERT(found);
}

static void gmmDestroyFixedAllocator()
{
    if (pGmmFixedAllocData)
    {
        for (int i=0; i<2; i++) 
        {
            for(int j=0; j<pGmmFixedAllocData->BlockListCount[i]; j++)
            {
                free(pGmmFixedAllocData->ppBlockList[i][j]);
                free(pGmmFixedAllocData->ppFreeBlockList[i][j]);
            }
            free(pGmmFixedAllocData->ppBlockList[i]);
            free(pGmmFixedAllocData->ppFreeBlockList[i]);
            free(pGmmFixedAllocData->pBlocksUsed[i]);
        }

        free(pGmmFixedAllocData);
        pGmmFixedAllocData = NULL;
    }
}

static GmmBlock *gmmAllocFixedBlock()
{
    return (GmmBlock *)gmmAllocFixed(0);
}

static void gmmFreeFixedBlock(GmmBlock *pBlock)
{
    gmmFreeFixed(0, pBlock);
}

static GmmTileBlock *gmmAllocFixedTileBlock()
{
    return (GmmTileBlock *)gmmAllocFixed(1);
}

static void gmmFreeFixedTileBlock(GmmTileBlock *pTileBlock)
{
    gmmFreeFixed(1, pTileBlock);
}

void gmmPinAllocations()
{
    pinAllocations = 1;
}

void gmmUnpinAllocations()
{
    pinAllocations = 0;
}

uint32_t gmmInit(
    const void *localMemoryBase,
    const void *localStartAddress,
    const uint32_t localSize,
    const void *mainMemoryBase,
    const void *mainStartAddress,
    const uint32_t mainSize
)
{
    GmmAllocator *pAllocator[2]; // index 0 is local, 1 is main
    uint32_t alignedLocalSize, alignedMainSize;
    uint32_t localEndAddress = (uint32_t)localStartAddress + localSize;
    uint32_t mainEndAddress = (uint32_t)mainStartAddress + mainSize;

    localEndAddress = (localEndAddress / GMM_TILE_ALIGNMENT) * GMM_TILE_ALIGNMENT;
    mainEndAddress = (mainEndAddress / GMM_TILE_ALIGNMENT) * GMM_TILE_ALIGNMENT;

    GMM_ASSERT(localEndAddress > (uint32_t)localStartAddress);
    GMM_ASSERT(mainEndAddress > (uint32_t)mainStartAddress);

    alignedLocalSize = localEndAddress - (uint32_t)localStartAddress;
    alignedMainSize = mainEndAddress - (uint32_t)mainStartAddress;

#ifndef GMM_NO_ERROR_CHECK
    if ((uint32_t)localStartAddress % GMM_ALIGNMENT != 0 ||
        (uint32_t)mainStartAddress % GMM_ALIGNMENT != 0)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }

    if (pGmmLocalAllocator || pGmmMainAllocator)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }
#endif

    pAllocator[0] = (GmmAllocator *)malloc(2*sizeof(GmmAllocator));
    pAllocator[1] = pAllocator[0] + 1;

    if (pAllocator[0] == NULL)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }

    memset(pAllocator[0], 0, 2*sizeof(GmmAllocator));

    if (pAllocator[0])
    {
        pAllocator[0]->memoryBase = (uint32_t)localMemoryBase;
        pAllocator[1]->memoryBase = (uint32_t)mainMemoryBase;
        pAllocator[0]->startAddress = (uint32_t)localStartAddress;
        pAllocator[1]->startAddress = (uint32_t)mainStartAddress;
        pAllocator[0]->size = alignedLocalSize;
        pAllocator[1]->size = alignedMainSize;
        pAllocator[0]->freeAddress = pAllocator[0]->startAddress;
        pAllocator[1]->freeAddress = pAllocator[1]->startAddress;
        pAllocator[0]->tileStartAddress = ((uint32_t)localStartAddress) + alignedLocalSize;
        pAllocator[1]->tileStartAddress = ((uint32_t)mainStartAddress) + alignedMainSize;
        pAllocator[0]->totalSize = alignedLocalSize;
        pAllocator[1]->totalSize = alignedMainSize;

        pGmmLocalAllocator = pAllocator[0];
        pGmmMainAllocator = pAllocator[1];
    }
    else
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }

    pLock = cellGcmGetLabelAddress(GMM_PPU_WAIT_INDEX);
    *pLock = 0;
    cachedLockValue = 0;

    return gmmInitFixedAllocator();
}

static inline void gmmWaitForSweep()
{
    while (1)
    {
        if (cachedLockValue == 0)
        {
            break;
        }

        cachedLockValue = *pLock;

        if (cachedLockValue == 0)
        {
            break;
        }

        sys_timer_usleep(30);
    }
}

uint32_t gmmDestroy()
{
    GmmBlock *pBlock, *pTmpBlock;
    GmmTileBlock *pTileBlock, *pTmpTileBlock;
    GmmAllocator *pAllocator[2];

    pAllocator[0] = pGmmLocalAllocator;
    pAllocator[1] = pGmmMainAllocator;

    for (int i=0; i<2; i++)
    {
        if (pAllocator[i])
        {
            pBlock = pAllocator[i]->pHead;

            while (pBlock)
            {
                pTmpBlock = pBlock;
                pBlock = pBlock->pNext;
                gmmFreeFixedBlock(pTmpBlock);
            }

            pTileBlock = pAllocator[i]->pTileHead;

            while (pTileBlock)
            {
                pTmpTileBlock = pTileBlock;
                pTileBlock = pTileBlock->pNext;
                gmmFreeFixedTileBlock(pTmpTileBlock);
            }

            free(pAllocator[i]);
        }
    }

    pGmmLocalAllocator = NULL;
    pGmmMainAllocator = NULL;

    gmmDestroyFixedAllocator();

    return CELL_OK;
}

uint32_t gmmGetBlockSize(
    const uint32_t id
)
{
    GmmBaseBlock    *pBaseBlock = (GmmBaseBlock *)id;

    GMM_ASSERT(id != GMM_ERROR);
    GMM_ASSERT(pBaseBlock->debugSignature == GMM_DEBUG_SIG);

    return pBaseBlock->size;
}

void *gmmGetTileData(
    const uint32_t id
)
{
    GmmTileBlock    *pTileBlock = (GmmTileBlock *)id;

    GMM_ASSERT(id != GMM_ERROR);
    GMM_ASSERT(pTileBlock->base.isTile &&
               pTileBlock->base.debugSignature == GMM_DEBUG_SIG);

    return pTileBlock->pData;
}

void gmmSetTileAttrib(
    const uint32_t id,
    const uint32_t tag,
    void *pData
)
{
    GmmTileBlock    *pTileBlock = (GmmTileBlock *)id;

    GMM_ASSERT(id != GMM_ERROR);
    GMM_ASSERT(pTileBlock->base.isTile &&
               pTileBlock->base.debugSignature == GMM_DEBUG_SIG);

    pTileBlock->tileTag = tag;
    pTileBlock->pData = pData;
}

uint32_t gmmIdToOffset(
    const uint32_t id
)
{
    GmmBaseBlock    *pBaseBlock = (GmmBaseBlock *)id;
    uint32_t        offset;

    GMM_ASSERT(id != GMM_ERROR);
    GMM_ASSERT(pBaseBlock->debugSignature == GMM_DEBUG_SIG);

    if (!pBaseBlock->isTile &&
        pinAllocations)
    {
        GmmBlock *pBlock = (GmmBlock *)id;
        pBlock->isPinned = pinAllocations;
    }   

    offset = gmmAddressToOffset(pBaseBlock->address, pBaseBlock->isMain);

#ifdef GMM_MEMORY_GUARD
    if (!pBaseBlock->isTile)
    {
        return offset+GMM_ALIGNMENT;
    }
#endif

    return offset;
}

char *gmmIdToAddress(
    const uint32_t id
)
{
    GmmBaseBlock    *pBaseBlock = (GmmBaseBlock *)id;

    GMM_ASSERT(id != GMM_ERROR);
    GMM_ASSERT(pBaseBlock->debugSignature == GMM_DEBUG_SIG);
    
    gmmWaitForSweep();

#ifdef GMM_MEMORY_GUARD
    if (!pBaseBlock->isTile)
    {
        for (uint32_t *pMem = (uint32_t *)pBaseBlock->address; pMem < (uint32_t *)(pBaseBlock->address+GMM_ALIGNMENT); pMem++)
        {
            GMM_ASSERT(*pMem == GMM_DEBUG_SIG);
        }
        for (uint32_t *pMem = (uint32_t *)(pBaseBlock->address+pBaseBlock->size-GMM_ALIGNMENT); pMem < (uint32_t *)(pBaseBlock->address+pBaseBlock->size); pMem++)
        {
            GMM_ASSERT(*pMem == GMM_DEBUG_SIG);
        }

        return (char *)(pBaseBlock->address + GMM_ALIGNMENT);
    }
#endif 

    return (char *)pBaseBlock->address;
}

uint32_t gmmFPOffsetToId(
    const uint32_t offset,
    uint32_t *pOutOffset,
	bool bLocalMemory
)
{
	// This function does not support offset located in main
    // memory. psglSetFragmentProgramConfiguration lack ability
    // for user to specify memory placement, it always assumes
    // FP offset in local memory. 
    GmmBlock    *pBlock = pGmmLocalAllocator->pHead;
    
	// psglSetFragmentProgramConfiguration now has the ability 
	// to specify local or main for the program location.
	// Therefore if the offset is not a local memory address need 
	// to look in the main memory heap vs the local.  
	if ( !bLocalMemory )
	{
		pBlock = pGmmMainAllocator->pHead; 
	}

    uint32_t    id = GMM_ERROR;
    uint32_t    blockOffset;

    while (pBlock)
    {
		blockOffset = gmmAddressToOffset(pBlock->base.address, !bLocalMemory ); 

        if (offset >= blockOffset &&
            offset <  blockOffset + pBlock->base.size)
        {
            id = (uint32_t)pBlock;
            *pOutOffset = offset - blockOffset;
            break;
        }

        pBlock = pBlock->pNext;
    }

    GMM_ASSERT(id != GMM_ERROR);

    return id;
}

void gmmPinId(
    const uint32_t id
)
{
    GmmBaseBlock    *pBaseBlock = (GmmBaseBlock *)id;

    GMM_ASSERT(id != GMM_ERROR);
    GMM_ASSERT(pBaseBlock->debugSignature == GMM_DEBUG_SIG);

    if (!pBaseBlock->isTile)
    {
        GmmBlock *pBlock = (GmmBlock *)id;

        GMM_ASSERT(pBlock->isPinned == 0);

        pBlock->isPinned = 1;
    }
}

void gmmUnpinId(
    const uint32_t id
)
{
    GmmBaseBlock    *pBaseBlock = (GmmBaseBlock *)id;

    GMM_ASSERT(id != GMM_ERROR);
    GMM_ASSERT(pBaseBlock->debugSignature == GMM_DEBUG_SIG);

    if (!pBaseBlock->isTile)
    {
        GmmBlock *pBlock = (GmmBlock *)id;

        GMM_ASSERT(pBlock->isPinned == 1);

        pBlock->isPinned = 0;
    }
}

static GmmBlock *gmmAllocBlock(
    GmmAllocator *pAllocator,
    uint32_t size
)
{
    uint32_t    address;
    GmmBlock    *pNewBlock = NULL;
    GmmBlock    *pBlock = pAllocator->pTail;

    GMM_ASSERT(size % GMM_ALIGNMENT == 0);
    GMM_ASSERT((pBlock != NULL && pBlock->pNext == NULL) ||
               (pBlock == NULL && pAllocator->pHead == NULL));

    address = pAllocator->freeAddress;

    if (UINT_MAX - address >= size && 
        address + size <= pAllocator->startAddress + pAllocator->size)
    {
        pNewBlock = gmmAllocFixedBlock();
        if (pNewBlock == NULL)
        {
            return NULL;
        }

        memset(pNewBlock, 0, sizeof(GmmBlock));
#ifdef GMM_DEBUG
        pNewBlock->base.debugSignature = GMM_DEBUG_SIG;
#endif

        pNewBlock->base.address = address;
        pNewBlock->base.isMain = (pAllocator == pGmmMainAllocator);
        pNewBlock->base.size = size;
        pAllocator->freeAddress = address + size;

        if (pBlock)
        {
            pNewBlock->pPrev = pBlock;
            pBlock->pNext = pNewBlock;
            pAllocator->pTail = pNewBlock;
        }
        else
        {
            pAllocator->pHead = pNewBlock;
            pAllocator->pTail = pNewBlock;
        }
    }

    return pNewBlock;
}

static GmmTileBlock *gmmFindFreeTileBlock(
    GmmAllocator *pAllocator,
    const uint32_t size
)
{
    GmmTileBlock    *pBlock = pAllocator->pTileHead;
    GmmTileBlock    *pBestAfterBlock = NULL;
    GmmTileBlock    *pNewBlock = NULL;
    uint32_t        bestSize = 0;
    uint32_t        freeSize = 0;

    while (pBlock && pBlock->pNext)
    {
        GMM_ASSERT(pBlock->base.isTile && 
                   pBlock->base.debugSignature == GMM_DEBUG_SIG);

        GMM_ASSERT(pBlock->pNext->base.isTile && 
                   pBlock->pNext->base.debugSignature == GMM_DEBUG_SIG);

        freeSize = pBlock->pNext->base.address - pBlock->base.address - pBlock->base.size;

        // we set pBestAfterBlock if pBlock meets the following criteria:
        //  - free space is more than space requested
        //  - free space is closer match (better fit) than existing pBestAfterBlock
        //  - it's not sandwich between existing region 
        if (freeSize >= size &&
            (pBestAfterBlock == NULL || freeSize < bestSize) &&
            (pBlock->pNext == NULL ||
             pBlock->pData != pBlock->pNext->pData))
        {
            pBestAfterBlock = pBlock;
            bestSize = freeSize;
        }

        pBlock = pBlock->pNext;
    }

    if (pBestAfterBlock)
    {
        // Make a block and insert after pBestAfterBlock and before pBestAfterBlock->pNext
        pNewBlock = gmmAllocFixedTileBlock();
        if (pNewBlock == NULL)
        {
            return NULL;
        }

        memset(pNewBlock, 0, sizeof(GmmTileBlock));
#ifdef GMM_DEBUG
        pNewBlock->base.debugSignature = GMM_DEBUG_SIG;
#endif

        pNewBlock->base.address = pBestAfterBlock->base.address + pBestAfterBlock->base.size;
        pNewBlock->base.isMain = (pAllocator == pGmmMainAllocator);
        pNewBlock->base.isTile = 1;
        pNewBlock->base.size = size;

        GMM_ASSERT(pNewBlock->base.address % GMM_TILE_ALIGNMENT == 0);

        pNewBlock->pNext = pBestAfterBlock->pNext;
        pNewBlock->pPrev = pBestAfterBlock;
        pNewBlock->pPrev->pNext = pNewBlock;
        pNewBlock->pNext->pPrev = pNewBlock;

        return pNewBlock;
    }
    else
    {
        // No free space, return NULL
        return NULL;
    }
}

static GmmTileBlock *gmmCreateTileBlock(
    GmmAllocator *pAllocator,
    const uint32_t size
)
{
    GmmTileBlock    *pNewBlock;
    uint32_t        address;

    address = pAllocator->tileStartAddress - size;

    GMM_ASSERT(address % GMM_TILE_ALIGNMENT == 0);

    if (address > pAllocator->startAddress + pAllocator->size)
    {
        return NULL;
    }

    if (pAllocator->pTail &&
        pAllocator->pTail->base.address + pAllocator->pTail->base.size > address)
    {
        return NULL;
    }

    pAllocator->size = address - pAllocator->startAddress;
    pAllocator->tileSize = pAllocator->tileStartAddress + pAllocator->tileSize - address;
    pAllocator->tileStartAddress = address;
    GMM_ASSERT(pAllocator->totalSize == pAllocator->size + pAllocator->tileSize);

    pNewBlock = gmmAllocFixedTileBlock();
    if (pNewBlock == NULL)
    {
        return NULL;
    }

    memset(pNewBlock, 0, sizeof(GmmTileBlock));
#ifdef GMM_DEBUG
    pNewBlock->base.debugSignature = GMM_DEBUG_SIG;
#endif

    pNewBlock->base.address = address;
    pNewBlock->base.isMain = (pAllocator == pGmmMainAllocator);
    pNewBlock->base.isTile = 1;
    pNewBlock->base.size = size;
    pNewBlock->pNext = pAllocator->pTileHead;

    if (pAllocator->pTileHead)
    {
        GMM_ASSERT(pAllocator->pTileHead->pPrev == NULL);
        pAllocator->pTileHead->pPrev = pNewBlock;
        pAllocator->pTileHead = pNewBlock;
    }
    else
    {
        GMM_ASSERT(pAllocator->pTileTail == NULL);
        pAllocator->pTileHead = pNewBlock;
        pAllocator->pTileTail = pNewBlock;
    }

    return pNewBlock;
}

static void gmmFreeTileBlock(
    GmmTileBlock    *pTileBlock
)
{
    GmmAllocator    *pAllocator;

    GMM_ASSERT(pTileBlock);

    if (pTileBlock->pPrev)
    {
        pTileBlock->pPrev->pNext = pTileBlock->pNext;
    }

    if (pTileBlock->pNext)
    {
        pTileBlock->pNext->pPrev = pTileBlock->pPrev;
    }

    if (pTileBlock->base.isMain)
    {
        pAllocator = pGmmMainAllocator;
    }
    else
    {
        pAllocator = pGmmLocalAllocator;
    }

    if (pAllocator->pTileHead == pTileBlock)
    {
        pAllocator->pTileHead = pTileBlock->pNext;

        if (pAllocator->pTileHead)
        {
            pAllocator->pTileHead->pPrev = NULL;
        }

        uint32_t prevSize;
        prevSize = pAllocator->size;
        pAllocator->size = pAllocator->pTileHead ? 
                           pAllocator->pTileHead->base.address - pAllocator->startAddress :
                           pAllocator->totalSize;
        GMM_ASSERT(pAllocator->size > prevSize);
        pAllocator->tileSize = pAllocator->totalSize - pAllocator->size;
        pAllocator->tileStartAddress = pAllocator->pTileHead ?
                                       pAllocator->pTileHead->base.address :
                                       pAllocator->startAddress + pAllocator->size;
        GMM_ASSERT(pAllocator->totalSize >= pAllocator->size &&
                   pAllocator->totalSize >= pAllocator->tileSize);
    }

    if (pAllocator->pTileTail == pTileBlock)
    {
        pAllocator->pTileTail = pTileBlock->pPrev;
        
        if (pAllocator->pTileTail)
        {
            pAllocator->pTileTail->pNext = NULL;
        }
    }

    gmmFreeFixedTileBlock(pTileBlock);
}

uint32_t gmmAllocExtendedTileBlock(
    const uint8_t location, 
    const uint32_t size,
    const uint32_t tag
)
{
    GmmAllocator    *pAllocator;
    uint32_t        retId = 0;
    uint32_t        newSize;
    uint8_t         resizeSucceed = 1;

#ifndef GMM_NO_ERROR_CHECK
    if (location != CELL_GCM_LOCATION_MAIN &&
        location != CELL_GCM_LOCATION_LOCAL)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }

    if (size == 0)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }
#endif

    pAllocator = (location == CELL_GCM_LOCATION_LOCAL) ? 
                 pGmmLocalAllocator : 
                 pGmmMainAllocator;
    
#ifndef GMM_NO_ERROR_CHECK
    if (pAllocator == NULL)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }
#endif

    newSize = pad(size, GMM_TILE_ALIGNMENT);

    GmmTileBlock    *pBlock = pAllocator->pTileTail;

    while (pBlock)
    {
        if (pBlock->tileTag == tag)
        {
            GLuint address, tileSize;
            _jsGcmGetTileRegionInfo(pBlock->pData, &address, &tileSize);

            if ((pBlock->pNext && pBlock->pNext->base.address-pBlock->base.address-pBlock->base.size >= newSize) ||
                (pBlock->pPrev && pBlock->base.address-pBlock->pPrev->base.address-pBlock->pPrev->base.size >= newSize))
            {
                GmmTileBlock *pNewBlock = gmmAllocFixedTileBlock();
                if (pNewBlock == NULL)
                {
                    GMM_ASSERT(0);
                    break;
                }

                retId = (uint32_t)pNewBlock;

                memset(pNewBlock, 0, sizeof(GmmTileBlock));
#ifdef GMM_DEBUG
                pNewBlock->base.debugSignature = GMM_DEBUG_SIG;
#endif

                pNewBlock->base.isMain = (pAllocator == pGmmMainAllocator);
                pNewBlock->base.isTile = 1;
                pNewBlock->base.size = newSize;

                if (pBlock->pNext && pBlock->pNext->base.address-pBlock->base.address-pBlock->base.size >= newSize)
                {
                    pNewBlock->base.address = pBlock->base.address+pBlock->base.size;
                    pNewBlock->pNext = pBlock->pNext;
                    pNewBlock->pPrev = pBlock;
                    pBlock->pNext->pPrev = pNewBlock;
                    pBlock->pNext = pNewBlock;

                    // no need to resize tile if we are already sandwich in between one
                    if (pNewBlock->pPrev->pData != pNewBlock->pNext->pData)
                    {
                        resizeSucceed = _jsGcmTryResizeTileRegion( address, tileSize+newSize, pBlock->pData );
                    }
                }
                else
                {
                    pNewBlock->base.address = pBlock->base.address-newSize;
                    pNewBlock->pNext = pBlock;
                    pNewBlock->pPrev = pBlock->pPrev;
                    pBlock->pPrev->pNext = pNewBlock;
                    pBlock->pPrev = pNewBlock;

                    // no need to resize tile if we are already sandwich in between one
                    if (pNewBlock->pPrev->pData != pNewBlock->pNext->pData)
                    {
                        resizeSucceed = _jsGcmTryResizeTileRegion( (GLuint)gmmIdToOffset((uint32_t)pNewBlock), tileSize+newSize, pBlock->pData );
                    }
                }
                gmmSetTileAttrib( retId, tag, pBlock->pData );
                break;
            }

            if (pBlock == pAllocator->pTileHead)
            {
                retId = (uint32_t)gmmCreateTileBlock(pAllocator, newSize); 
                if (retId == 0)
                {
                    GMM_ASSERT(0);
                    break;
                }

                resizeSucceed = _jsGcmTryResizeTileRegion( (GLuint)gmmIdToOffset(retId), tileSize+newSize, pBlock->pData );
                gmmSetTileAttrib( retId, tag, pBlock->pData );
                break;
            }
        }

        pBlock = pBlock->pPrev;
    }

    if (retId == 0)
    {
        return GMM_ERROR;
    }

    // free the block if somehow resize region failed...
    if (!resizeSucceed)
    {
        gmmFreeTileBlock((GmmTileBlock *)retId);
        return GMM_ERROR;
    }

#ifdef GMM_DEBUG
    GmmBaseBlock *pBaseBlock = (GmmBaseBlock *)retId;

    pBaseBlock->allocationId = uniqueAllocationId;
    uniqueAllocationId++;
#endif

    return retId;
}

static GmmTileBlock *gmmAllocTileBlock(
    GmmAllocator *pAllocator,
    const uint32_t size
)
{
    GmmTileBlock    *pBlock = gmmFindFreeTileBlock(pAllocator, size); 

    if (pBlock == NULL)
    {
        pBlock = gmmCreateTileBlock(pAllocator, size);
    }

    return pBlock;
}

static void gmmFreeBlock(
    GmmBlock *pBlock
)
{
    GmmAllocator    *pAllocator;

    if (pBlock->pPrev)
    {
        pBlock->pPrev->pNext = pBlock->pNext;
    }

    if (pBlock->pNext)
    {
        pBlock->pNext->pPrev = pBlock->pPrev;
    }
   
    if (pBlock->base.isMain)
    {
        pAllocator = pGmmMainAllocator;
    }
    else
    {
        pAllocator = pGmmLocalAllocator;
    }

    if (pAllocator->pHead == pBlock)
    {
        pAllocator->pHead = pBlock->pNext;

        if (pAllocator->pHead)
        {
            pAllocator->pHead->pPrev = NULL;
        }
    }

    if (pAllocator->pTail == pBlock)
    {
        pAllocator->pTail = pBlock->pPrev;
        
        if (pAllocator->pTail)
        {
            pAllocator->pTail->pNext = NULL;
        }
    }

    if (pBlock->pPrev == NULL)
    {
        pAllocator->pSweepHead = pAllocator->pHead;
    }
    else if (pBlock->pPrev &&
             (pAllocator->pSweepHead == NULL || 
              (pAllocator->pSweepHead &&
               pAllocator->pSweepHead->base.address > pBlock->pPrev->base.address)))
    {
        pAllocator->pSweepHead = pBlock->pPrev;
    }

    pAllocator->freedSinceSweep += pBlock->base.size;
    
    gmmFreeFixedBlock(pBlock);
}

static void gmmAddPendingFree(
    GmmBlock *pBlock
)
{
    GmmAllocator    *pAllocator;

    if (pBlock->base.isMain)
    {
        pAllocator = pGmmMainAllocator;
    }
    else
    {
        pAllocator = pGmmLocalAllocator;
    }

    if (pAllocator->pPendingFreeTail)
    {
        pBlock->pNextFree = NULL;
        pBlock->pPrevFree = pAllocator->pPendingFreeTail;
        pAllocator->pPendingFreeTail->pNextFree = pBlock;
        pAllocator->pPendingFreeTail = pBlock;
    }
    else
    {
        GMM_ASSERT(pAllocator->pPendingFreeHead == NULL);

        pBlock->pNextFree = NULL;
        pBlock->pPrevFree = NULL;
        pAllocator->pPendingFreeHead = pBlock;
        pAllocator->pPendingFreeTail = pBlock;
    }

    pBlock->isPinned = 0;

    // set fence
    _jsGcmIncFenceRef( &pBlock->fence );
}

static uint8_t gmmSizeToFreeIndex(
    uint32_t size
)
{
    GMM_ASSERT(size >= GMM_FREE_BIN_0);

    if (size >= GMM_FREE_BIN_0 && size < GMM_FREE_BIN_1)
    {
        return 0;
    }
    else if (size >= GMM_FREE_BIN_1 && size < GMM_FREE_BIN_2)
    {
        return 1;
    }
    else if (size >= GMM_FREE_BIN_2 && size < GMM_FREE_BIN_3)
    {
        return 2;
    }
    else if (size >= GMM_FREE_BIN_3 && size < GMM_FREE_BIN_4)
    {
        return 3;
    }
    else if (size >= GMM_FREE_BIN_4 && size < GMM_FREE_BIN_5)
    {
        return 4;
    }
    else if (size >= GMM_FREE_BIN_5 && size < GMM_FREE_BIN_6)
    {
        return 5;
    }
    else if (size >= GMM_FREE_BIN_6 && size < GMM_FREE_BIN_7)
    {
        return 6;
    }
    else if (size >= GMM_FREE_BIN_7 && size < GMM_FREE_BIN_8)
    {
        return 7;
    }
    else if (size >= GMM_FREE_BIN_8 && size < GMM_FREE_BIN_9)
    {
        return 8;
    }
    else if (size >= GMM_FREE_BIN_9 && size < GMM_FREE_BIN_10)
    {
        return 9;
    }
    else if (size >= GMM_FREE_BIN_10 && size < GMM_FREE_BIN_11)
    {
        return 10;
    }
    else if (size >= GMM_FREE_BIN_11 && size < GMM_FREE_BIN_12)
    {
        return 11;
    }
    else if (size >= GMM_FREE_BIN_12 && size < GMM_FREE_BIN_13)
    {
        return 12;
    }
    else if (size >= GMM_FREE_BIN_13 && size < GMM_FREE_BIN_14)
    {
        return 13;
    }
    else if (size >= GMM_FREE_BIN_14 && size < GMM_FREE_BIN_15)
    {
        return 14;
    }
    else if (size >= GMM_FREE_BIN_15 && size < GMM_FREE_BIN_16)
    {
        return 15;
    }
    else if (size >= GMM_FREE_BIN_16 && size < GMM_FREE_BIN_17)
    {
        return 16;
    }
    else if (size >= GMM_FREE_BIN_17 && size < GMM_FREE_BIN_18)
    {
        return 17;
    }
    else if (size >= GMM_FREE_BIN_18 && size < GMM_FREE_BIN_19)
    {
        return 18;
    }
    else if (size >= GMM_FREE_BIN_19 && size < GMM_FREE_BIN_20)
    {
        return 19;
    }
    else if (size >= GMM_FREE_BIN_20 && size < GMM_FREE_BIN_21)
    {
        return 20;
    }
    else
    {
        GMM_ASSERT(size >= GMM_FREE_BIN_21);
        return 21;
    }

    // make sure code above is inline with GMM_NUM_FREE_BINS
    GMM_ASSERT(GMM_NUM_FREE_BINS == 22);
}

static void gmmAddFree(
    GmmAllocator *pAllocator,
    GmmBlock *pBlock
)
{
    uint8_t freeIndex = gmmSizeToFreeIndex(pBlock->base.size);

#ifdef GMM_DEBUG
    pAllocator->blockCount[freeIndex]++;
#endif

    if (pAllocator->pFreeHead[freeIndex])
    {
        GmmBlock *pInsertBefore = pAllocator->pFreeHead[freeIndex];

        while (pInsertBefore && pInsertBefore->base.size < pBlock->base.size)
        {
            pInsertBefore = pInsertBefore->pNextFree;
        }

        if (pInsertBefore == NULL)
        {
            GMM_ASSERT(pAllocator->pFreeTail[freeIndex]);

            pBlock->pNextFree = NULL;
            pBlock->pPrevFree = pAllocator->pFreeTail[freeIndex];
            pAllocator->pFreeTail[freeIndex]->pNextFree = pBlock;
            pAllocator->pFreeTail[freeIndex] = pBlock;
        }
        else if (pInsertBefore == pAllocator->pFreeHead[freeIndex])
        {
            GMM_ASSERT(pInsertBefore->pPrevFree == NULL);

            pBlock->pNextFree = pInsertBefore;
            pBlock->pPrevFree = pInsertBefore->pPrevFree;
            pInsertBefore->pPrevFree = pBlock;
            pAllocator->pFreeHead[freeIndex] = pBlock;
        }
        else
        {
            pBlock->pNextFree = pInsertBefore;
            pBlock->pPrevFree = pInsertBefore->pPrevFree;
            pInsertBefore->pPrevFree->pNextFree = pBlock;
            pInsertBefore->pPrevFree = pBlock;
        }
    }
    else
    {
        GMM_ASSERT(pAllocator->pFreeTail[freeIndex] == NULL);

        pBlock->pNextFree = NULL;
        pBlock->pPrevFree = NULL;
        pAllocator->pFreeHead[freeIndex] = pBlock;
        pAllocator->pFreeTail[freeIndex] = pBlock;
    }
}

uint32_t gmmFree(
    const uint32_t freeId
)
{
    GmmBaseBlock    *pBaseBlock = (GmmBaseBlock *)freeId;
    
#ifndef GMM_NO_ERROR_CHECK
    if (pBaseBlock == NULL)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }
#endif

    GMM_ASSERT(pBaseBlock->debugSignature == GMM_DEBUG_SIG);

    if (pBaseBlock->isTile)
    {
        GmmTileBlock *pTileBlock = (GmmTileBlock *)pBaseBlock;

        if (pTileBlock->pPrev &&
            pTileBlock->pNext &&
            pTileBlock->pPrev->pData == pTileBlock->pNext->pData)
        {
            // don't need to resize region since block free is in the middle of a region
            GMM_ASSERT(pTileBlock->pData == pTileBlock->pPrev->pData);
        }
        else if (pTileBlock->pPrev && pTileBlock->pPrev->pData == pTileBlock->pData)
        {
            // subtract pTileBlock's size off the end of the region
            GLuint address, size;

            _jsGcmGetTileRegionInfo(pTileBlock->pData, &address, &size);
            if ( !_jsGcmTryResizeTileRegion(address, (size-pTileBlock->base.size), pTileBlock->pData) )
            {
                // If we can't resize, we will delete the original tile and recreate
                _jsGcmTryResizeTileRegion(address, 0, pTileBlock->pData);
                if ( !_jsGcmTryResizeTileRegion(address, (size-pTileBlock->base.size), pTileBlock->pData) )
                {
                    // should not fail
                    GMM_ASSERT(0);
                }
            }
        }
        else if (pTileBlock->pNext && pTileBlock->pNext->pData == pTileBlock->pData)
        {
            // subtract pTileBlock's size off the start of the region
            GLuint address, size;

            _jsGcmGetTileRegionInfo(pTileBlock->pData, &address, &size);
            if ( !_jsGcmTryResizeTileRegion((address+pTileBlock->base.size), (size-pTileBlock->base.size), pTileBlock->pData) )
            {
                // If we can't resize, we will delete the original tile and recreate
                _jsGcmTryResizeTileRegion(address, 0, pTileBlock->pData);
                if ( !_jsGcmTryResizeTileRegion((address+pTileBlock->base.size), (size-pTileBlock->base.size), pTileBlock->pData) )
                {
                    // should not fail
                    GMM_ASSERT(0);
                }
            }
        }
        else
        {
            // this means the block is in its own unique region
            // delete the region
            GMM_ASSERT((pTileBlock->pPrev == NULL || pTileBlock->pData != pTileBlock->pPrev->pData) &&
                       (pTileBlock->pNext == NULL || pTileBlock->pData != pTileBlock->pNext->pData));

            if ( !_jsGcmTryResizeTileRegion( (GLuint)gmmIdToOffset(freeId), 0, gmmGetTileData(freeId) ) )
            {
                // should not fail
                GMM_ASSERT(0);
            }
        }

        gmmFreeTileBlock(pTileBlock);
    }
    else
    {
        GmmBlock *pBlock = (GmmBlock *)pBaseBlock;

        gmmAddPendingFree(pBlock);
    }

    return CELL_OK;
}

static inline void gmmLocalMemcpy(
    CellGcmContextData *thisContext,
    const uint32_t dstOffset,
    const uint32_t srcOffset,
    const uint32_t moveSize
)
{
    // this function is not overlap safe!!!
    // caller must make sure src and dst doesn't overlap
    int32_t offset = 0;
    int32_t sizeLeft = moveSize;
    int32_t dimension = 4096;

    while (sizeLeft > 0)
    {
        while(sizeLeft >= dimension*dimension*4)
        {
            GMM_ASSERT((dstOffset+offset) % 64 == 0);
            GMM_ASSERT((srcOffset+offset) % 64 == 0);

            cellGcmSetTransferImage(thisContext,
                                    CELL_GCM_TRANSFER_LOCAL_TO_LOCAL,
                                    dstOffset+offset,
                                    dimension*4,
                                    0,
                                    0,
                                    srcOffset+offset,
                                    dimension*4,
                                    0,
                                    0,
                                    dimension,
                                    dimension,
                                    4);

            offset = offset + dimension*dimension*4;
            sizeLeft = sizeLeft - (dimension*dimension*4);
        }

        dimension = dimension >> 1;

        // through lots of trial and error, found that
        // for remain portions smaller than 32x32x4, we
        // need break and handle rest with 1 line transfer
        if (dimension == 32)
        {
            break;
        }
    }

    if (sizeLeft > 0)
    {
        GMM_ASSERT((dstOffset+offset) % 64 == 0);
        GMM_ASSERT((srcOffset+offset) % 64 == 0);
        GMM_ASSERT(sizeLeft % 4 == 0);

        cellGcmSetTransferImage(thisContext, 
                                CELL_GCM_TRANSFER_LOCAL_TO_LOCAL,
                                dstOffset+offset,
                                sizeLeft,
                                0,
                                0,
                                srcOffset+offset,
                                sizeLeft,
                                0,
                                0,
                                sizeLeft/4,
                                1,
                                4);
    }
}

static inline void gmmMemcpy(
    CellGcmContextData *thisContext,
    const uint8_t mode,
    const uint32_t dstOffset,
    const uint32_t srcOffset,
    const uint32_t moveSize
)
{
    GMM_ASSERT(mode == CELL_GCM_TRANSFER_LOCAL_TO_LOCAL ||
               mode == CELL_GCM_TRANSFER_MAIN_TO_MAIN);

    if (mode == CELL_GCM_TRANSFER_LOCAL_TO_LOCAL)
    {
        if (dstOffset + moveSize <= srcOffset)
        {
            gmmLocalMemcpy(thisContext,
                           dstOffset,
                           srcOffset,
                           moveSize);
        }
        // else case takes care of dividing overlap into smaller chunks.
        else
        {
            uint32_t moveBlockSize = srcOffset-dstOffset;
            uint32_t iterations = (moveSize+moveBlockSize-1)/moveBlockSize;

            GMM_ASSERT(srcOffset > dstOffset);
            GMM_ASSERT(moveBlockSize < moveSize);

            for (uint32_t i=0; i<iterations; i++)
            {
                gmmLocalMemcpy(thisContext,
                               dstOffset+(i*moveBlockSize),
                               srcOffset+(i*moveBlockSize),
                               moveBlockSize);
            }
        }
    }
    else
    {
        // cellGcmSetTransferData is overlap safe if rowCount == 1
        cellGcmSetTransferData(thisContext,
                               mode,
                               dstOffset,
                               moveSize,
                               srcOffset,
                               moveSize,
                               moveSize,
                               1);

        // this to to make sure main to main transfers are in correct order
        cellGcmSetWriteBackEndLabel(thisContext, GMM_RSX_WAIT_INDEX, RSXWaitLabelValue);
        cellGcmSetWaitLabel(thisContext, GMM_RSX_WAIT_INDEX, RSXWaitLabelValue);
        RSXWaitLabelValue++;
    }
}

static uint8_t gmmInternalSweep(
    CellGcmContextData *thisContext,
    const uint8_t location
)
{
    GmmAllocator    *pAllocator;
    GmmBlock        *pBlock;       
    GmmBlock        *pSrcBlock;
    GmmBlock        *pTempBlock;
    GmmBlock        *pTempBlockNext;
    uint32_t        dstAddress, srcAddress;
    uint32_t        srcOffset, dstOffset;
    uint32_t        prevEndAddress;
    uint32_t        moveSize, moveDistance;
    uint8_t         mode;
    uint8_t         isMain;
    uint8_t         ret = 0;
    uint32_t        totalMoveSize = 0;

#ifndef GMM_NO_ERROR_CHECK
    if (location != CELL_GCM_LOCATION_MAIN &&
        location != CELL_GCM_LOCATION_LOCAL)
    {
        GMM_ASSERT(0);
        return ret;
    }
#endif

    pAllocator = (location == CELL_GCM_LOCATION_LOCAL) ? 
                 pGmmLocalAllocator : 
                 pGmmMainAllocator;

#ifndef GMM_NO_ERROR_CHECK
    if (pAllocator == NULL)
    {
        GMM_ASSERT(0);
        return ret;
    }
#endif

    GMM_ASSERT(pAllocator->startAddress + pAllocator->size >= pAllocator->freeAddress);

    if (location == CELL_GCM_LOCATION_LOCAL)
    {
        mode = CELL_GCM_TRANSFER_LOCAL_TO_LOCAL;
        isMain = 0;
    }
    else
    {
        mode = CELL_GCM_TRANSFER_MAIN_TO_MAIN;
        isMain = 1;

        // this to to make sure main to main transfers are in correct order
        if (pAllocator->pSweepHead)
        {
            cellGcmSetWriteBackEndLabel(thisContext, GMM_RSX_WAIT_INDEX, RSXWaitLabelValue);
            cellGcmSetWaitLabel(thisContext, GMM_RSX_WAIT_INDEX, RSXWaitLabelValue);
            RSXWaitLabelValue++;
        }
    }

    pBlock = pAllocator->pSweepHead;
    srcAddress = 0;
    dstAddress = 0;
    prevEndAddress = 0;
    pSrcBlock = pBlock;

    while (pBlock != NULL)
    {
        GMM_ASSERT(pBlock->base.debugSignature == GMM_DEBUG_SIG &&
                   pBlock->base.isTile == 0);

        if (pBlock->isPinned == 0)
        {
            if (pBlock->pPrev)
            {
                prevEndAddress = pBlock->pPrev->base.address + pBlock->pPrev->base.size;
            }
            else
            {
                GMM_ASSERT(pBlock == pAllocator->pHead);

                prevEndAddress = pAllocator->startAddress;
            }

            GMM_ASSERT(pBlock->base.address >= prevEndAddress);

            if (pBlock->base.address > prevEndAddress)
            {
                dstAddress = prevEndAddress;
                srcAddress = pBlock->base.address;
                pSrcBlock = pBlock;
            }

            moveSize = pBlock->base.address + pBlock->base.size - srcAddress;

            if (srcAddress > dstAddress &&
                (pBlock->pNext == NULL ||
                 pBlock->pNext->base.address > pBlock->base.address + pBlock->base.size ||
                 pBlock->pNext->isPinned))
            {
                dstOffset = gmmAddressToOffset(dstAddress, isMain);
                srcOffset = gmmAddressToOffset(srcAddress, isMain);

                totalMoveSize += moveSize;

                gmmMemcpy(thisContext,
                          mode,
                          dstOffset,
                          srcOffset,
                          moveSize);

                pTempBlock = pSrcBlock;

                GMM_ASSERT(srcOffset > dstOffset);
                GMM_ASSERT((srcOffset - dstOffset) == (srcAddress - dstAddress));

                moveDistance = srcOffset - dstOffset;

                while (pTempBlock != pBlock->pNext)
                {
                    GMM_ASSERT(pTempBlock->base.address >= srcAddress &&
                               pTempBlock->base.address <= srcAddress + moveSize &&
                               pTempBlock->base.address + pTempBlock->base.size >= srcAddress &&
                               pTempBlock->base.address + pTempBlock->base.size <= srcAddress + moveSize);

                    pTempBlock->base.address -= moveDistance;

                    GMM_ASSERT(pTempBlock->base.address >= dstAddress &&
                               pTempBlock->base.address < dstAddress + moveSize);

                    pTempBlock = pTempBlock->pNext;
                }
            }
        }
        else
        {
            uint32_t availibleSize;

            // re-initialize srcAddress and dstAddress here this is intentional 
            // because we need to wipe the old values before we enter the         
            // "if (pBlock->isPinned == 0)" case
            srcAddress = 0;
            dstAddress = 0;

            if (pBlock->pPrev == NULL)
            {
                GMM_ASSERT(pBlock == pAllocator->pHead);
                GMM_ASSERT(pBlock->base.address >= pAllocator->startAddress);
                availibleSize = pBlock->base.address - pAllocator->startAddress;
            }
            else
            {
                GMM_ASSERT(pBlock->base.address >= pBlock->pPrev->base.address + pBlock->pPrev->base.size);
                availibleSize = pBlock->base.address - (pBlock->pPrev->base.address + pBlock->pPrev->base.size);
            }

            pTempBlock = pBlock->pNext;

            while (availibleSize >= GMM_ALIGNMENT &&
                   pTempBlock)
            {
                pTempBlockNext = pTempBlock->pNext;

                if (pTempBlock->isPinned == 0 &&
                    pTempBlock->base.size <= availibleSize)
                {
                    uint32_t pinDstAddress = (pBlock->pPrev == NULL) ?
                                             pAllocator->startAddress :
                                             pBlock->pPrev->base.address + pBlock->pPrev->base.size;
                    uint32_t pinSrcAddress = pTempBlock->base.address;

                    dstOffset = gmmAddressToOffset(pinDstAddress, isMain);
                    srcOffset = gmmAddressToOffset(pinSrcAddress, isMain);

                    totalMoveSize += pTempBlock->base.size;

                    gmmMemcpy(thisContext,
                              mode,
                              dstOffset,
                              srcOffset,
                              pTempBlock->base.size);

                    pTempBlock->base.address = pinDstAddress;

                    if (pTempBlock == pAllocator->pTail)
                    {
                        if (pTempBlock->pNext)
                        {
                            pAllocator->pTail = pTempBlock->pNext;
                        }
                        else
                        {
                            GMM_ASSERT(pTempBlock->pPrev);

                            pAllocator->pTail = pTempBlock->pPrev;
                        }
                    }

                    // disconnect pTempBlock from existing neighbors
                    if (pTempBlock->pNext)
                    {
                        pTempBlock->pNext->pPrev = pTempBlock->pPrev;
                    }
                    if (pTempBlock->pPrev)
                    {
                        pTempBlock->pPrev->pNext = pTempBlock->pNext;
                    }

                    // connect pTempBlock to front of pBlock
                    if (pBlock->pPrev)
                    {
                        pBlock->pPrev->pNext = pTempBlock;
                    }
                    else
                    {
                        GMM_ASSERT(pBlock == pAllocator->pHead);
                        pAllocator->pHead = pTempBlock;
                    }
                    pTempBlock->pPrev = pBlock->pPrev;
                    pTempBlock->pNext = pBlock;
                    pBlock->pPrev = pTempBlock;
                }

                if (pBlock->pPrev)
                {
                    GMM_ASSERT(pBlock->base.address >= pBlock->pPrev->base.address + pBlock->pPrev->base.size);
                    availibleSize = pBlock->base.address - (pBlock->pPrev->base.address + pBlock->pPrev->base.size);
                }

                pTempBlock = pTempBlockNext;
            }

            if (availibleSize > 0)
            {
                // create a block here that's at pBlock->base.address - availibleSize
                // and mark it dirty... and double check if dirty head is correct.
                GMM_ASSERT((pBlock->base.address - availibleSize) % GMM_ALIGNMENT == 0); 

                GmmBlock *pNewBlock = gmmAllocFixedBlock();

                if (pNewBlock)
                {
                    memset(pNewBlock, 0, sizeof(GmmBlock));
#ifdef GMM_DEBUG
                    pNewBlock->base.debugSignature = GMM_DEBUG_SIG;

                    pNewBlock->base.allocationId = uniqueAllocationId;
                    uniqueAllocationId++;
#endif
                    pNewBlock->base.address = pBlock->base.address - availibleSize;
                    pNewBlock->base.isMain = pBlock->base.isMain;
                    pNewBlock->base.size = availibleSize;
                    pNewBlock->pNext = pBlock;
                    pNewBlock->pPrev = pBlock->pPrev;
                    if (pBlock->pPrev)
                    {
                        pBlock->pPrev->pNext = pNewBlock;
                    }
                    pBlock->pPrev = pNewBlock;

                    if (pBlock == pAllocator->pHead)
                    {
                        pAllocator->pHead = pNewBlock;
                    }

                    gmmAddFree(pAllocator, pNewBlock);

                    // need to return 1 in order to signal gmmAllocSweep to properly 
                    // do write back end label for synchronization
                    ret = 1;
                }
            }
        }

        pBlock = pBlock->pNext;
    }

    uint32_t newFreeAddress = pAllocator->pTail ? 
                              pAllocator->pTail->base.address + pAllocator->pTail->base.size :
                              pAllocator->startAddress;

    if (pAllocator->freeAddress != newFreeAddress)
    {
        GMM_ASSERT(pAllocator->freeAddress > newFreeAddress);
        pAllocator->freeAddress = newFreeAddress;
        ret = 1;
    }

    pAllocator->freedSinceSweep = 0;
    pAllocator->pSweepHead = NULL;

#ifdef GMM_DEBUG
    pBlock = pAllocator->pHead;

    while (pBlock != NULL)
    {
        if (pBlock->pNext)
        {
            GMM_ASSERT(pBlock == pBlock->pNext->pPrev);
            GMM_ASSERT(pBlock->base.address < pBlock->pNext->base.address);
        }
        else
        {
            GMM_ASSERT(pBlock == pAllocator->pTail);
        }
        pBlock = pBlock->pNext;
    }
#endif

    return ret;
}

static void gmmRemovePendingFree(
    GmmAllocator *pAllocator,
    GmmBlock *pBlock
)
{
    if (pBlock == pAllocator->pPendingFreeHead)
    {
        pAllocator->pPendingFreeHead = pBlock->pNextFree;
    }

    if (pBlock == pAllocator->pPendingFreeTail)
    {
        pAllocator->pPendingFreeTail = pBlock->pPrevFree;
    }

    if (pBlock->pNextFree)
    {
        pBlock->pNextFree->pPrevFree = pBlock->pPrevFree;
    }

    if (pBlock->pPrevFree)
    {
        pBlock->pPrevFree->pNextFree = pBlock->pNextFree;
    }
}

void gmmUpdateFreeList(
    const uint8_t location
)
{
    GmmAllocator    *pAllocator;
    const uint32_t  fence = _jsGcmGetFenceRef();
    GmmBlock        *pBlock = NULL;
    GmmBlock        *pTemp = NULL;

#ifndef GMM_NO_ERROR_CHECK
    if (location != CELL_GCM_LOCATION_MAIN &&
        location != CELL_GCM_LOCATION_LOCAL)
    {
        GMM_ASSERT(0);
    }
#endif

    pAllocator = (location == CELL_GCM_LOCATION_LOCAL) ? 
                 pGmmLocalAllocator : 
                 pGmmMainAllocator;
    
#ifndef GMM_NO_ERROR_CHECK
    if (pAllocator == NULL)
    {
        GMM_ASSERT(0);
    }
#endif

    pBlock = pAllocator->pPendingFreeHead;

    while (pBlock)
    {
        // this is tricky because gmmRemovePendingFree & gmmAddFree will 
        // change pNextFree, save pNextFree to pTemp so we don't lose it
        pTemp = pBlock->pNextFree;

        if ( !(( fence - pBlock->fence ) & 0x80000000 ) )
        {
            gmmRemovePendingFree(pAllocator, pBlock);
            gmmAddFree(pAllocator, pBlock);
        }

        pBlock = pTemp;
    }

#ifdef GMM_DEBUG
#ifdef GMM_VERBOSE
    static int count = 0;
    //if (count%1200 == 0)  // uncomment this, if it is too often
    {
        printf("local\n");
        for (int i=0; i<GMM_NUM_FREE_BINS; i++)
        {
            printf("bin %d: %d\n", i, pGmmLocalAllocator->blockCount[i]);
        }
        printf("main\n");
        for (int i=0; i<GMM_NUM_FREE_BINS; i++)
        {
            printf("bin %d: %d\n", i, pGmmMainAllocator->blockCount[i]);
        }
    }
    count++;
#endif
#endif
}

static void gmmFreeAll(
    const uint8_t location
)
{
    GmmAllocator    *pAllocator;
    GmmBlock        *pBlock;
    GmmBlock        *pTemp;

#ifndef GMM_NO_ERROR_CHECK
    if (location != CELL_GCM_LOCATION_MAIN &&
        location != CELL_GCM_LOCATION_LOCAL)
    {
        GMM_ASSERT(0);
    }
#endif

    pAllocator = (location == CELL_GCM_LOCATION_LOCAL) ? 
                 pGmmLocalAllocator : 
                 pGmmMainAllocator;
    
#ifndef GMM_NO_ERROR_CHECK
    if (pAllocator == NULL)
    {
        GMM_ASSERT(0);
    }
#endif

    pBlock = pAllocator->pPendingFreeHead;
    while (pBlock)
    {
        pTemp = pBlock->pNextFree;
        gmmFreeBlock(pBlock);
        pBlock = pTemp;
    }
    pAllocator->pPendingFreeHead = NULL;
    pAllocator->pPendingFreeTail = NULL;

    for (int i=0; i<GMM_NUM_FREE_BINS; i++)
    {
        pBlock = pAllocator->pFreeHead[i];
        while (pBlock)
        {
            pTemp = pBlock->pNextFree;
            gmmFreeBlock(pBlock);
            pBlock = pTemp;
        }
        pAllocator->pFreeHead[i] = NULL;
        pAllocator->pFreeTail[i] = NULL;
    }
}

static void gmmAllocSweep(
    CellGcmContextData *thisContext,
    const uint8_t location
)
{
    gmmFreeAll(location);

    if (gmmInternalSweep(thisContext, location))
    {
        *pLock = 1;
        cachedLockValue = 1;
        cellGcmSetWriteBackEndLabel(thisContext, GMM_PPU_WAIT_INDEX, 0);

        cellGcmFlush(thisContext);
    }
}

static uint32_t gmmInternalAlloc(
    GmmAllocator *pAllocator,
    const uint8_t isTile,
    const uint32_t size
)
{
    uint32_t        retId;

    if (isTile)
    {
        retId = (uint32_t)gmmAllocTileBlock(pAllocator, size);
    }
    else
    {
        retId = (uint32_t)gmmAllocBlock(pAllocator, size);
    }

    if (retId == 0)
    {
        return GMM_ERROR;
    }

#ifdef GMM_DEBUG
    GmmBaseBlock *pBaseBlock = (GmmBaseBlock *)retId;

    pBaseBlock->allocationId = uniqueAllocationId;
    uniqueAllocationId++;
#endif

    return retId;
}

static void gmmRemoveFree(
    GmmAllocator *pAllocator,
    GmmBlock *pBlock,
    uint8_t freeIndex
)
{
#ifdef GMM_DEBUG
    pAllocator->blockCount[freeIndex]--;
#endif

    if (pBlock == pAllocator->pFreeHead[freeIndex])
    {
        pAllocator->pFreeHead[freeIndex] = pBlock->pNextFree;
    }

    if (pBlock == pAllocator->pFreeTail[freeIndex])
    {
        pAllocator->pFreeTail[freeIndex] = pBlock->pPrevFree;
    }

    if (pBlock->pNextFree)
    {
        pBlock->pNextFree->pPrevFree = pBlock->pPrevFree;
    }

    if (pBlock->pPrevFree)
    {
        pBlock->pPrevFree->pNextFree = pBlock->pNextFree;
    }
}

static uint32_t gmmFindFreeBlock(
    GmmAllocator    *pAllocator,
    uint32_t        size
)
{
    uint32_t        retId = GMM_ERROR;
    GmmBlock        *pBlock;
    uint8_t         found = 0;
    uint8_t         freeIndex = gmmSizeToFreeIndex(size);

    pBlock = pAllocator->pFreeHead[freeIndex];

    while (freeIndex < GMM_NUM_FREE_BINS)
    {
        if (pBlock)
        {
            if (pBlock->base.size >= size)
            {
                found = 1;
                break;
            }
            pBlock = pBlock->pNextFree;
        }
        else if (++freeIndex < GMM_NUM_FREE_BINS)
        {
            pBlock = pAllocator->pFreeHead[freeIndex];
        }
    }

    if (found)
    {
        if (pBlock->base.size != size)
        {
            GMM_ASSERT(pBlock->base.size > size);

            // create a new block here
            GmmBlock *pNewBlock = gmmAllocFixedBlock();
            if (pNewBlock == NULL)
            {
                return GMM_ERROR;
            }

            memset(pNewBlock, 0, sizeof(GmmBlock));
#ifdef GMM_DEBUG
            pNewBlock->base.debugSignature = GMM_DEBUG_SIG;

            pNewBlock->base.allocationId = uniqueAllocationId;
            uniqueAllocationId++;
#endif
            pNewBlock->base.address = pBlock->base.address + size;
            pNewBlock->base.isMain = pBlock->base.isMain;
            pNewBlock->base.size = pBlock->base.size - size;
            pNewBlock->pNext = pBlock->pNext;
            pNewBlock->pPrev = pBlock;
            if (pBlock->pNext)
            {
                pBlock->pNext->pPrev = pNewBlock;
            }
            pBlock->pNext = pNewBlock;

            if (pBlock == pAllocator->pTail)
            {
                pAllocator->pTail = pNewBlock;
            }

            gmmAddFree(pAllocator, pNewBlock);
        }
        pBlock->base.size = size;
        gmmRemoveFree(pAllocator, pBlock, freeIndex);
        retId = (uint32_t)pBlock;
    }

    return retId;
}

static uint32_t gmmAllocFromFreeList(
    GmmAllocator    *pAllocator,
    const uint32_t size
)
{
    uint32_t        retId = GMM_ERROR;

    // find free block from free list
    retId = gmmFindFreeBlock(pAllocator, size);

    // perhaps we can coalesce here...
    // but not sure how much gain if we do so
    // cuz we are effectively coalescing when we sweep
    // and there's benefits to keeping blocks at the same 
    // size since usage patterns are likely to repeat

#ifdef GMM_DEBUG
    if (retId != GMM_ERROR)
    {
        GmmBaseBlock *pBaseBlock = (GmmBaseBlock *)retId;

        pBaseBlock->allocationId = uniqueAllocationId;
        uniqueAllocationId++;
    }
#endif

    return retId;
}

uint32_t gmmAlloc(
    CellGcmContextData *thisContext,
    const uint8_t location, 
    const uint8_t isTile,
    const uint32_t size
)
{
    GmmAllocator    *pAllocator;
    uint32_t        retId;
    uint32_t        newSize;

    // we check this even when GMM_NO_ERROR_CHECK is defined
    // this is because various GL APIs makes it possible for 
    // user to trigger zero sized allocation from above
    // provide compiler hint by setting the expectation of 
    // (size == 0) to be zero
    if (__builtin_expect((size == 0),0))
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }

#ifndef GMM_NO_ERROR_CHECK
    if (location != CELL_GCM_LOCATION_MAIN &&
        location != CELL_GCM_LOCATION_LOCAL)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }
#endif

    pAllocator = (location == CELL_GCM_LOCATION_LOCAL) ? 
                 pGmmLocalAllocator : 
                 pGmmMainAllocator;
    
#ifndef GMM_NO_ERROR_CHECK
    if (pAllocator == NULL)
    {
        GMM_ASSERT(0);
        return GMM_ERROR;
    }
#endif

    if (!isTile)
    {
#ifdef GMM_MEMORY_GUARD
        newSize = pad(size + 2*GMM_ALIGNMENT, GMM_ALIGNMENT);
#else
        newSize = pad(size, GMM_ALIGNMENT);
#endif

        retId = gmmAllocFromFreeList(pAllocator, newSize);
    }
    else
    {
        newSize = pad(size, GMM_TILE_ALIGNMENT);
        retId = GMM_ERROR;
    }

    if (retId == GMM_ERROR)
    {
        retId = gmmInternalAlloc(pAllocator,
                                 isTile,
                                 newSize);

        if (retId == GMM_ERROR)
        {
            gmmAllocSweep(thisContext, location);

            retId = gmmInternalAlloc(pAllocator,
                                     isTile,
                                     newSize);

            if (!isTile &&
                retId == GMM_ERROR)
            {
                retId = gmmAllocFromFreeList(pAllocator, newSize);
            }
        }
    }

    // assert that we are not out of memory
    GMM_ASSERT(retId != GMM_ERROR);

#ifdef GMM_MEMORY_GUARD
    if (!isTile && retId != GMM_ERROR)
    {
        gmmWaitForSweep();

        GmmBlock *pNewBlock = (GmmBlock *)retId;

        if (pNewBlock)
        {
            for (uint32_t *pMem = (uint32_t *)pNewBlock->base.address; pMem < (uint32_t *)(pNewBlock->base.address+GMM_ALIGNMENT); pMem++)
            {
                *pMem = GMM_DEBUG_SIG;
            }
            for (uint32_t *pMem = (uint32_t *)(pNewBlock->base.address+pNewBlock->base.size-GMM_ALIGNMENT); pMem < (uint32_t *)(pNewBlock->base.address+pNewBlock->base.size); pMem++)
            {
                *pMem = GMM_DEBUG_SIG;
            }
        }
    }
#endif

    return retId;
}

#ifdef GMM_DEBUG
void gmmPrintState()
{
    GmmAllocator    *pAllocator;
    GmmBlock        *pBlock;   
    GmmTileBlock    *pTileBlock;

    for (int i=0; i<2; i++)
    {
        if (i == 0)
        {
            pAllocator = pGmmLocalAllocator;
            fprintf(stderr, "Local ");
        }
        else
        {
            pAllocator = pGmmMainAllocator;
            fprintf(stderr, "Main ");
        }
        fprintf(stderr, "Blocks (address, size, unique ID, pinned):\n");

        pBlock = pAllocator->pHead;

        while (pBlock != NULL)
        {
            fprintf(stderr, "0x%08x, %d, %d, %d\n", pBlock->base.address, pBlock->base.size, pBlock->base.allocationId, pBlock->isPinned);
            if (pBlock->pNext)
            {
                GMM_ASSERT(pBlock == pBlock->pNext->pPrev);
            }
            pBlock = pBlock->pNext;
        }

        if (i == 0)
        {
            fprintf(stderr, "Local ");
        }
        else
        {
            fprintf(stderr, "Main ");
        }
        fprintf(stderr, "Tile Blocks (address, size, unique ID):\n");

        pTileBlock = pAllocator->pTileHead;

        while (pTileBlock != NULL)
        {
            fprintf(stderr, "0x%08x, %d, %d\n", pTileBlock->base.address, pTileBlock->base.size, pTileBlock->base.allocationId);
            if (pTileBlock->pNext)
            {
                GMM_ASSERT(pTileBlock == pTileBlock->pNext->pPrev);
            }
            pTileBlock = pTileBlock->pNext;
        }
    }
}
#endif
