/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/*
 * ProDG script utility functions
 */

#include "spurs_prodg.h"

char buf[256];

void dumpDirEnt(sn_uint32 pDirEnt)
{
	sn_val *pVal;
	sn_uint32 dmaTag, refCount, needWriteBack, isLoading, lsa, hasVtable, ea;

	sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::DirEntry*)%#x)->dmaTag", pDirEnt);
	pVal = getSpuVal(buf); dmaTag = pVal->val.u32;
	sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::DirEntry*)%#x)->refCount", pDirEnt);
	pVal = getSpuVal(buf); refCount = pVal->val.u32;
	sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::DirEntry*)%#x)->needWriteBack", pDirEnt);
	pVal = getSpuVal(buf); needWriteBack = pVal->val.u32;
	sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::DirEntry*)%#x)->isLoading", pDirEnt);
	pVal = getSpuVal(buf); isLoading = pVal->val.u32;
	sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::DirEntry*)%#x)->lsa", pDirEnt);
	pVal = getSpuVal(buf); lsa = pVal->val.u32;
	sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::DirEntry*)%#x)->hasVtable", pDirEnt);
	pVal = getSpuVal(buf); hasVtable = pVal->val.u32;
	sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::DirEntry*)%#x)->ea", pDirEnt);
	pVal = getSpuVal(buf); ea = pVal->val.u32;

	printf("DirEnt@%#x(lsa:%#x, ea:%#x, refCount:%u, %s, dmaTag:%#x%s%s\n",
		pDirEnt, (lsa<<4)|(ea&0xf), ea, refCount, needWriteBack?"RW":"RO", dmaTag, isLoading?" LOADING":"", hasVtable?" VTAB":"");
}

void dumpDefaultCache(sn_uint32 uProcessID, sn_uint64 uSpuThreadID)
{
	sn_val *pVal;
	sn_uint32 pHeader;
	sn_uint16 aux, slot;
	sn_uint32 pDirEnt, magicWord, magicWordExp, size, sizeHeap, sizeUsed, sizeFragmented;

	magicWordExp = 0x3e3b;

	guProcessID = uProcessID;
	guSpuThreadID = uSpuThreadID;
	pVal = getSpuVal("_ZZN4cell7swcache11DefaultHeap6getPtrENS1_6OpcodeEmPvE5sSize");
	sizeHeap = pVal->val.u32;
	pVal = getSpuVal("*((unsigned*)_ZZN4cell7swcache11DefaultHeap6getPtrENS1_6OpcodeEmPvE9spHeapTop)");
	if (pVal != NULL)
	{
		pHeader = pVal->val.u32;
		if (pHeader)
		{
			printf("\t===== cell::swcache::DefaultCache dump begin =====\n");
			sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::MemBlockHeader*)%#x)->aux", pHeader);
			pVal = getSpuVal(buf); aux = pVal->val.u16;
			sizeUsed = 0; sizeFragmented = 0;
			while(aux)
			{
				pHeader -= aux*16;
				sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::MemBlockHeader*)%#x)->pDirEnt", pHeader);
				pVal = getSpuVal(buf); pDirEnt = pVal->val.u32;
				sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::MemBlockHeader*)%#x)->size", pHeader);
				pVal = getSpuVal(buf); size = pVal->val.u32;
				sizeUsed += ((size+15+15)&~15) + 16;
				sizeFragmented += aux*16 - (((size+15+15)&~15) + 16);
				sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::MemBlockHeader*)%#x)->magicWord", pHeader);
				pVal = getSpuVal(buf); magicWord = pVal->val.u32;
				sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::MemBlockHeader*)%#x)->aux", pHeader);
				pVal = getSpuVal(buf); aux = pVal->val.u16;
				sprintf(buf, "((cell::swcache::DefaultCache<cell::swcache::DefaultHeap,4u,5u>::MemBlockHeader*)%#x)->slot", pHeader);
				pVal = getSpuVal(buf); slot = pVal->val.u16;
				if (magicWord == magicWordExp)
				{
					printf("\tMemBlock@%#x(size:%#x, DirSlot:%d)\n\t\t", pHeader, size, slot);
					dumpDirEnt(pDirEnt);
				} else {
					printf("!!! MemBlock@%#x is broken !!!\n", pHeader);
				}
			}
			printf("\t------------------------------------------------\n");
			printf("\tSize of heap = %u bytes, %u bytes used(%d.%02d%% used) %d.%02d%% fragmented\n",
				sizeHeap, sizeUsed,
				sizeUsed*100/sizeHeap, sizeUsed*10000/sizeHeap - (sizeUsed*100/sizeHeap)*100,
				sizeFragmented*100/sizeHeap, sizeFragmented*10000/sizeHeap - (sizeFragmented*100/sizeHeap)*100);
			printf("\t===== cell::swcache::DefaultCache dump end =====\n");
		} else {
			printf("cell::swcache::DefaultCache is not initialized\n");
		}
	} else {
		printf("cell::swcache::DefaultCache is not found in this SPU\n");
	}
}
