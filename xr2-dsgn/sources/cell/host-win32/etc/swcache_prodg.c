/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/*
 * ProDG script to watch swcache
 */

#include "swcache_prodg.h"

void PS3PrintCellSwcacheDefaultCache(sn_uint32 uProcessID, sn_uint32 uSpuThreadID)
{
	int i;
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	pPPUThreadID = (sn_uint64 *)malloc(sizeof(sn_uint64) * 256);
	pSPUThreadID = (sn_uint64 *)malloc(sizeof(sn_uint64) * 256);
	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		for(i = 0; i < NumSPUThreads; i++) {
			if (pSPUThreadID[i].word[0] == uSpuThreadID) break;
		}
		if (i < NumSPUThreads)
		{
			dumpDefaultCache(uProcessID, pSPUThreadID[i]);
		} else {
			printf("Invalid SPU thread ID\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
	free(pPPUThreadID);
	free(pSPUThreadID);
}
