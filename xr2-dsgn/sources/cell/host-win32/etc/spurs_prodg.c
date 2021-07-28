/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/*
 * ProDG script to watch SPURS status
 */

#include "spurs_prodg.h"

void PS3PrintCellSpursJobQueue(sn_uint32 uProcessID, sn_uint32 jobQueueEa) {
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = jobQueueEa;
				addr.word[1] = 0;
				dumpJobQueue(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursJobQueuePort(sn_uint32 uProcessID, sn_uint32 eaPort)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaPort;
				addr.word[1] = 0;
				dumpPort(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursJobQueueSemaphore(sn_uint32 uProcessID, sn_uint32 eaSem)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaSem;
				addr.word[1] = 0;
				dumpJobQueueSemaphore(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}


void PS3PrintCellSpursJobChain(sn_uint32 uProcessID, sn_uint32 jobChainEa) {
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = jobChainEa;
				addr.word[1] = 0;
				dumpJobChain(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursTaskset(sn_uint32 uProcessID, sn_uint32 eaTaskset)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaTaskset;
				addr.word[1] = 0;
				dumpTaskSet(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursTask(sn_uint32 uProcessID, sn_uint32 eaTaskset, unsigned int idTask)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaTaskset;
				addr.word[1] = 0;
				dumpTask(uProcessID, pPPUThreadID[0], addr, idTask);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpurs(sn_uint32 uProcessID, sn_uint32 eaSpurs)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaSpurs;
				addr.word[1] = 0;
				dumpSpursInstance(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursQueue(sn_uint32 uProcessID, sn_uint32 eaSpursQueue)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaSpursQueue;
				addr.word[1] = 0;
				dumpSpursQueue(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursBarrier(sn_uint32 uProcessID, sn_uint32 eaSpursBarrier)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaSpursBarrier;
				addr.word[1] = 0;
				dumpSpursBarrier(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursSemaphore(sn_uint32 uProcessID, sn_uint32 eaSpursSemaphore)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaSpursSemaphore;
				addr.word[1] = 0;
				dumpSpursSemaphore(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursEventFlag(sn_uint32 uProcessID, sn_uint32 eaSpursEventFlag)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaSpursEventFlag;
				addr.word[1] = 0;
				dumpSpursEventFlag(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursTaskExitCode(sn_uint32 uProcessID, sn_uint32 eaSpursTaskExitCode)
{
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		if (NumPPUThreads > 0) {
			pPPUThreadID = (sn_uint64 *)malloc(NumPPUThreads * 8);
			if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
			{
				sn_address addr;
				addr.word[0] = eaSpursTaskExitCode;
				addr.word[1] = 0;
				dumpSpursTaskExitCode(uProcessID, pPPUThreadID[0], addr);
			} else {
				printf("Failed to get thread list\n");
			}
			free(pPPUThreadID);
		} else {
			printf("No PPU thread\n");
		}
	} else {
		printf("Failed to get thread list\n");
	}
}

void PS3PrintCellSpursJobQueueModule(sn_uint32 uProcessID, sn_uint32 uSpuThreadID)
{
	sn_uint64 _uSpuThreadID;

	_uSpuThreadID.word[0] = uSpuThreadID;
	_uSpuThreadID.word[1] = 0;

	dumpJQM( uProcessID, _uSpuThreadID );
}

void PS3PrintCellSpursJobChainModule(sn_uint32 uProcessID, sn_uint32 uSpuThreadID)
{
	sn_uint64 _uSpuThreadID;

	_uSpuThreadID.word[0] = uSpuThreadID;
	_uSpuThreadID.word[1] = 0;

	dumpJM( uProcessID, _uSpuThreadID );
}

int main()
{
	printf("\nSPURS ProDG script is ready.\n\n");
	return 0;
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
