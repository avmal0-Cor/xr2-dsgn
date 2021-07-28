/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/*
 * ProDG script utility functions
 */

sn_uint32 guProcessID;
sn_address gJobChains[16];
sn_uint64 guSpuThreadID;
sn_uint64 guPpuThreadID;
sn_val gVal;
sn_uint8 gpJmGuid[16], gpJqmGuid[16], gpTmGuid[16];

/* #define	READ_AREA_SIZE	256 */
#define	READ_AREA_SIZE	4096

sn_val *getSpuVal(const char *var_name) {
	if (SN_FAILED(PS3EvaluateExpression(guProcessID, guSpuThreadID, &gVal, var_name))) {
		sn_notify Notify;
		RequestNotification(NT_DEBUG);
		GetNotification(NT_DEBUG, &Notify);
		if (SN_FAILED(PS3EvaluateExpression(guProcessID, guSpuThreadID, &gVal, var_name))) {
			printf("\n1 Error: %s -> %s", GetLastErrorString(), var_name);
			return NULL;
		}
	}
	return &gVal;
}

sn_int16 readSpuInt16Array(const char *var_name, unsigned index) {
	sn_int16 pInt16[READ_AREA_SIZE/2];
	sn_address addr;
	sn_val *val = getSpuVal(var_name);
	if (val == NULL) return NULL;
	addr = val->val.Address;
	if (SN_FAILED(PS3GetMemory(guProcessID, guSpuThreadID, (void *)pInt16, addr, READ_AREA_SIZE))) {
		printf("\n3 Error: %s", GetLastErrorString());
		return 0;
	}
	return EndianSwap_2(pInt16[index]);
}

sn_uint8 readSpuUint8Array(const char *var_name, unsigned index) {
	sn_uint8 pUint8[READ_AREA_SIZE];
	sn_address addr;
	sn_val *val = getSpuVal(var_name);
	if (val == NULL) return NULL;
	addr = val->val.Address;
	if (SN_FAILED(PS3GetMemory(guProcessID, guSpuThreadID, (void *)pUint8, addr, READ_AREA_SIZE))) {
		printf("\n4 Error: %s", GetLastErrorString());
		return 0;
	}
	return pUint8[index];
}

sn_uint32 readSpuUint32Array(const char *var_name, unsigned index) {
	sn_uint32 pUint32[READ_AREA_SIZE/4];
	sn_address addr;
	sn_val *val = getSpuVal(var_name);
	if (val == NULL) return NULL;
	addr = val->val.Address;
	if (SN_FAILED(PS3GetMemory(guProcessID, guSpuThreadID, (void *)pUint32, addr, READ_AREA_SIZE))) {
		printf("\n5 Error: %s", GetLastErrorString());
		return 0;
	}
	return EndianSwap_4(pUint32[index]);
}

sn_uint8 readPpuUint8Array(sn_address addr, unsigned index) {
	sn_uint8 pUint8[READ_AREA_SIZE];
	if (SN_FAILED(PS3GetMemory(guProcessID, guPpuThreadID, (void *)pUint8, addr, READ_AREA_SIZE))) {
		printf("\n6 Error: %s", GetLastErrorString());
		return 0;
	}
	return pUint8[index];
}

sn_uint16 readPpuUint16Array(sn_address addr, unsigned index) {
	sn_uint16 pUint16[READ_AREA_SIZE/2];
	if (SN_FAILED(PS3GetMemory(guProcessID, guPpuThreadID, (void *)pUint16, addr, READ_AREA_SIZE))) {
		printf("\n6 Error: %s", GetLastErrorString());
		return 0;
	}
	return EndianSwap_2(pUint16[index]);
}

sn_uint32 readPpuUint32Array(sn_address addr, unsigned index) {
	sn_uint32 pUint32[READ_AREA_SIZE/4];
	if (SN_FAILED(PS3GetMemory(guProcessID, guPpuThreadID, (void *)pUint32, addr, READ_AREA_SIZE))) {
		printf("\n7 Error: %s", GetLastErrorString());
		return 0;
	}
	return EndianSwap_4(pUint32[index]);
}

sn_uint64 readPpuUint64Array(sn_address addr, unsigned index) {
	sn_uint64 ret;
	sn_uint32 pUint32[READ_AREA_SIZE/4];
	if (SN_FAILED(PS3GetMemory(guProcessID, guPpuThreadID, (void *)pUint32, addr, READ_AREA_SIZE))) {
		printf("\n7 Error: %s", GetLastErrorString());
		ret.word[0] = 0;
		ret.word[1] = 0;
		return ret;
	}

	ret.word[0] = EndianSwap_4(pUint32[2*index+1]);
	ret.word[1] = EndianSwap_4(pUint32[2*index]);
	return ret;
}

const char* readPpuString(sn_address addr) {
	static char tmp[READ_AREA_SIZE];
	unsigned int len;
	if (SN_FAILED(PS3GetMemory(guProcessID, guPpuThreadID, (void *)tmp, addr, READ_AREA_SIZE))) {
		printf("\n8 Error: %s", GetLastErrorString());
		return 0;
	}
	tmp[255] = '\0';
	return tmp;
}

void dumpKernel(sn_uint64 threadID) {
	int i;
	sn_val val, *pVal;
	sn_uint8 prio;
	char str_buffer[256];

	guSpuThreadID = threadID;
	pVal = getSpuVal("gKernelContext.idSpu");
	if (pVal == NULL) return;
	printf("\n\n");
	printf("- SPU%d -------------------------------\n", pVal->val.u32);
	printf("* Kernel Info\n");
	printf("\tEa of Spurs        \t%#x\n", getSpuVal("gKernelContext.eaSpurs")->val.u64.word[0]);
	if (getSpuVal("gKernelContext.idWorkload")->val.u32 != 16) {
		printf("\tCurrent Workload ID\t%d\n", getSpuVal("gKernelContext.idWorkload")->val.u32);
	} else {
		printf("System Service\n");
	}
}

void dumpJM(sn_uint32 processID, sn_uint64 threadID) {
	int i;
	char str_buffer[256];
	sn_address addr;
	sn_uint32 u32_var;
	sn_val *pVal;
	guProcessID = processID;
	guSpuThreadID = threadID;

	pVal = getSpuVal("gCellSpursJobStreamer.eaWork");
	if (pVal == NULL) return;
	printf("\n");
	printf("* Jobchain Informataion\n");
	printf("\tEa of JobChain           \t%#x\n", pVal->val.u64.word[0]);
	printf("\tIs preempted?            \t%d\n",  readSpuInt16Array("gCellSpursJobStreamer.context", 2));
	printf("\tIs auto ready count?     \t%d\n",  readSpuUint8Array("gCellSpursJobStreamer.jobHeader.state", 4));
	printf("\tSize of Job              \t%d\n",  getSpuVal("gCellSpursJobStreamer.jobHeader.jobDefSize")->val.u32);
	printf("\tmax grab                 \t%d\n",  getSpuVal("gCellSpursJobStreamer.jobHeader.maxGrab")->val.u32);
	printf("* Jobchain Context\n");
	printf("\tJobchain PC              \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.__pclr_debug__.pc")->val.u64.word[0]);
	printf("\tJobchain LR[0]           \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.__pclr_debug__.lr[0]")->val.u64.word[0]);
	printf("\tJobchain LR[1]           \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.__pclr_debug__.lr[1]")->val.u64.word[0]);
	printf("\tJobchain LR[2]           \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.__pclr_debug__.lr[2]")->val.u64.word[0]);
	printf("\tIs halted?               \t%d\n",  readSpuUint8Array("gCellSpursJobStreamer.jobHeader.state", 3));
	printf("\turgent command slot[0]   \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.urgent_command[0]")->val.u64.word[0]);
	printf("\turgent command slot[1]   \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.urgent_command[1]")->val.u64.word[0]);
	printf("\turgent command slot[2]   \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.urgent_command[2]")->val.u64.word[0]);
	printf("\turgent command slot[3]   \t%#x\n", getSpuVal("gCellSpursJobStreamer.jobHeader.urgent_command[3]")->val.u64.word[0]);
	
	printf("* Pipeline information\n");
	printf("\tFetch            \t");
	if (getSpuVal("sStages.stage1")->val.i16 != 0) {
		if (getSpuVal("sStages.stage1")->val.i16 & 0x80) {
			printf("LABEL here");
		} else {
			printf("          ");
		}
		printf("descriptor=%#x",
			   readSpuUint32Array("sJobDescriptorPtrs", 0));
		printf("(ea=%#x)",
			   getSpuVal("sEaJobDescriptors.s1_ea")->val.u64.word[0]);
	}
	printf("\n");
	printf("\tInput            \t");
	if (getSpuVal("sStages.stage2")->val.i16 != 0) {
		if (getSpuVal("sStages.stage2")->val.i16 & 0x80) {
			printf("LABEL here");
		} else {
			printf("          ");
		}
		printf("descriptor=%#x", readSpuUint32Array("sJobDescriptorPtrs", 1));
		printf("(ea=%#x)", getSpuVal("sEaJobDescriptors.s2_ea")->val.u64.word[0]);
	}
	printf("\n");
	printf("\tExecute          \t");
	if (getSpuVal("sStages.stage3")->val.i16 != 0) {
		if (getSpuVal("sStages.stage3")->val.i16 & 0x80) {
			printf("LABEL here");
		} else {
			printf("          ");
		}
		printf("descriptor=%#x", readSpuUint32Array("sJobDescriptorPtrs", 2));
	}
	printf("\n");
	printf("\tOutput           \t");
	if (getSpuVal("sStages.stage4")->val.i16 != 0) {
		if (getSpuVal("sStages.stage4")->val.i16 & 0x80) {
			printf("LABEL here");
		} else {
			printf("          ");
		}
		printf("descriptor=%#x", readSpuUint32Array("sJobDescriptorPtrs", 3));
	}
	printf("\n");
	
	printf("* Current Job information\n");
	printf("\tEa of Job binary         \t%#x\n", getSpuVal("gCellSpursJobDebug.info.jobEa")->val.u64.word[0]);
	printf("\tEntry address(LS) of Job \t%#x\n", getSpuVal("gCellSpursJobDebug.info.jobEntry")->val.u32);
	
	printf("* Cache information\n");
	for(i = 0; i < 8; i++) {
		sprintf(str_buffer, "gCellSpursCacheDirectory.ea[%d]", i);
		if (getSpuVal(str_buffer)->val.u64.word[0] != 0) {
			printf("\tea=%#08x,", getSpuVal(str_buffer)->val.u64.word[0]);
			sprintf(str_buffer, "gCellSpursCacheDirectory.ls[%d]", i);
			printf("lsa=%#05x,", getSpuVal(str_buffer)->val.u8*1024);
			sprintf(str_buffer, "gCellSpursCacheDirectory.refcount[%d]", i);
			printf("refcount=%d\n",getSpuVal(str_buffer)->val.u8);
		}
	}
	
	printf("* Memory Manager information\n");
	for(i = 0; i < 16; i++) {
		sn_uint8 size, base;
		sprintf(str_buffer, "gCellSpursMallocList.base[%d]", i);
		base = getSpuVal(str_buffer)->val.u8;
		sprintf(str_buffer, "gCellSpursMallocList.size[%d]", i);
		size = getSpuVal(str_buffer)->val.u8;
		if (size && base) {
			printf("\tlsa=%#05x,size=%#x\n", base*1024, size*1024);
		}
	}
}

void dumpStage(sn_uint32 state, sn_uint32 pJob, sn_uint32 isResumed)
{
	char *states[5] = {
		"EMPTY   ",
		"RUNNING ",
		"STALL   ",
		"LOADING ",
		"WAITTING"
	};
	printf(
		"        State\t   :%s\n"
		"        Job descriptor\tLsa:%#05x%s\n",
		states[state], pJob,
		isResumed ? "\tresumed":"");
}

void dumpJQM(sn_uint32 uProcessID, sn_uint64 uSpuThreadID) {
	sn_uint32 EMPTY   = 0u;
	sn_uint32 RUNNING = 1u;
	sn_uint32 STALL   = 2u;
	sn_uint32 LOADING = 3u;
	sn_uint32 WAITING = 4u;
	
	sn_uint32	IS_CACHE_STALE_MASK_IN_COMMAND =	(0x04u);
	sn_uint32 	IS_RESUMED_JOB_MASK_IN_COMMAND =	(0x01u);
	
	sn_uint32	IS_FLUSH_MASK_IN_INFO =			(0x01u);
	sn_uint32	IS_CACHE_STALE_MASK_IN_INFO	=	(0x02u);
	sn_uint32	IS_RESUMED_JOB_MASK_IN_INFO =		(0x04u);
	sn_uint32	IS_SUSPEND_JOB_MASK_IN_INFO =		(0x08u);
	sn_uint32	IS_CONTEXT_LESS_JOB_MASK_IN_INFO =	(0x10u);
	sn_uint32	IS_CACHE_STALE_MASK_IN_INFO	=	(0x02u);

	sn_uint32 tag;

	sn_val *pVal;

	guProcessID = uProcessID;
	guSpuThreadID = uSpuThreadID;
	pVal = getSpuVal("gPm.eaJobQueue");
	if (pVal == NULL){
		printf("Error: There is no SPURS JobQueue module on specified SPU thread\n");
		return;
	}

	printf(
		"\n* SPURS JobQueue module Informataion (CellSpursJobQueue ea:%#x)\n"
		"  Fetch    : "
		, pVal->val.u64.word[0]);
	if (getSpuVal("gPm.mFetch.mState")->val.u32 != EMPTY)
	{
		if (getSpuVal("gPm.mFetchInfo")->val.u64.word[0] & IS_FLUSH_MASK_IN_INFO)
		{
			printf("FLUSH command\n");
		} else {
			printf("\n");
			if (getSpuVal("gPm.mFetchInfo")->val.u64.word[1] & IS_CACHE_STALE_MASK_IN_INFO)
			{
				printf("        Use cache\t: No\n");
			}
			if (getSpuVal("gPm.mFetchInfo")->val.u64.word[0] & ~0x7f)
			{
				printf("        Ea   of semaphore\t:%#08x\n", getSpuVal("gPm.mFetchInfo")->val.u64.word[0] & ~0x7f);
			}
			if (!getSpuVal("gPm.mFetch.mIsResumed")->val.u32) {
				printf(
					"        Ea   of job descriptor\t:%#08x\n"
					"        Size of job descriptor\t:%u bytes\n"
					"        Dma tag ID\t:%u\n"
					, getSpuVal("gPm.mFetchEaJobDesc")->val.u32, getSpuVal("gPm.mFetchJobDescSize")->val.u32
					, getSpuVal("gPm.mFetchDmaTag")->val.u32);
				dumpStage(getSpuVal("gPm.mFetch.mState")->val.u32, getSpuVal("gPm.mFetch.mpJob")->val.u32, getSpuVal("gPm.mFetch.mIsResumed")->val.u32);
			} else {
				printf(
					"        Ea   of resume job context\t:%#08x\n"
					, getSpuVal("gPm.mFetchResumeDesc.eaResumeJobContext")->val.u32);
			}
		}
	} else {
		printf("No job\n");
	}
	printf("  Input    : ");
	if (getSpuVal("gPm.mInput.mState")->val.u32 != EMPTY)
	{
		if (getSpuVal("gPm.mInputInfo")->val.u64.word[0] & IS_FLUSH_MASK_IN_INFO)
		{
			printf("FLUSH command\n");
		} else {
			printf("\n");
			if (getSpuVal("gPm.mInputInfo")->val.u64.word[1] & IS_CACHE_STALE_MASK_IN_INFO)
			{
				printf("        Use cache\t: No\n");
			}
			if (getSpuVal("gPm.mInputInfo")->val.u64.word[0] & ~0x7f)
			{
				printf("        Ea   of semaphore\t:%#08x\n", getSpuVal("gPm.mInputInfo")->val.u64.word[0] & ~0x7f);
			}
			if (!getSpuVal("gPm.mInput.mIsResumed")->val.u32) {
				printf(
					"        Slot\t:%u\n"
					"        Ea   of job descriptor\t:%#08x\n"
					"        Size of job descriptor\t:%u bytes\n"
					"        Dma tag ID\t:%u\n"
					, getSpuVal("gPm.mInFiberIndex")->val.u32
					, getSpuVal("gPm.mInputEaJobDesc")->val.u32, getSpuVal("gPm.mInputJobDescSize")->val.u32
					, getSpuVal("gPm.mInDmaTag")->val.u32);
				dumpStage(getSpuVal("gPm.mInput.mState")->val.u32, getSpuVal("gPm.mInput.mpJob")->val.u32, getSpuVal("gPm.mInput.mIsResumed")->val.u32);
			} else {
				printf(
					"        Ea   of resume job context\t:%#08x\n"
					, getSpuVal("gPm.mInputResumeDesc.eaResumeJobContext")->val.u32);
			}
		}
	} else {
		printf("No job\n");
	}
	printf("  Execute#0: %s\n",
		getSpuVal("gPm.mExecManager.mContexts[0].mStage.mState")->val.u32 != RUNNING ? "No job" : "");

	if (getSpuVal("gPm.mExecManager.mContexts[0].mStage.mState")->val.u32 == RUNNING)
	{
		if (getSpuVal("gPm.mExecManager.mContexts[0].mStage.mState")->val.u32 == 0) return;
		printf("        Size of Job descriptor\t:%u bytes\n", getSpuVal("gPm.mExecManager.mContexts[0].mSizeJobDesc")->val.u32);
		if (getSpuVal("gPm.mExecManager.mContexts[0].mInfo")->val.u64.word[1] & IS_CACHE_STALE_MASK_IN_INFO)
		{
			printf("        Use cache\t: No\n");
		}
		if (getSpuVal("gPm.mExecManager.mContexts[0].mInfo")->val.u64.word[0] & ~0x7f)
		{
			printf("        Ea   of semaphore\t:%#08x\n", getSpuVal("gPm.mExecManager.mContexts[0].mInfo")->val.u64.word[0] & ~0x7f);
		}
		dumpStage(
			getSpuVal("gPm.mExecManager.mContexts[0].mStage.mState")->val.u32,
			getSpuVal("gPm.mExecManager.mContexts[0].mStage.mpJob")->val.u32,
			getSpuVal("gPm.mExecManager.mContexts[0].mStage.mIsResumed")->val.u32);
	}
	printf("  Execute#1: %s\n",
		getSpuVal("gPm.mExecManager.mContexts[1].mStage.mState")->val.u32 != RUNNING ? "No job" : "");
	if (getSpuVal("gPm.mExecManager.mContexts[1].mStage.mState")->val.u32 == RUNNING)
	{
		if (getSpuVal("gPm.mExecManager.mContexts[1].mStage.mState")->val.u32 == 0) return;
		printf("        Size of Job descriptor\t:%u bytes\n", getSpuVal("gPm.mExecManager.mContexts[1].mSizeJobDesc")->val.u32);
		if (getSpuVal("gPm.mExecManager.mContexts[1].mInfo")->val.u64.word[1] & IS_CACHE_STALE_MASK_IN_INFO)
		{
			printf("        Use cache\t: No\n");
		}
		if (getSpuVal("gPm.mExecManager.mContexts[1].mInfo")->val.u64.word[0] & ~0x7f)
		{
			printf("        Ea   of semaphore\t:%#08x\n", getSpuVal("gPm.mExecManager.mContexts[1].mInfo")->val.u64.word[0] & ~0x7f);
		}
		dumpStage(
			getSpuVal("gPm.mExecManager.mContexts[1].mStage.mState")->val.u32,
			getSpuVal("gPm.mExecManager.mContexts[1].mStage.mpJob")->val.u32,
			getSpuVal("gPm.mExecManager.mContexts[1].mStage.mIsResumed")->val.u32);
	}
	printf("  Output   : ");
	if (getSpuVal("gPm.mOutDmaTagMask")->val.u32)
	{
		printf("\n        Output DMA tag ID\t:");
		for(tag = 0; tag < 32; tag++)
		{
			if (getSpuVal("gPm.mOutDmaTagMask")->val.u32 & (1u << tag)) {
				printf("%u ", tag);
			}
		}
		if (getSpuVal("gPm.mOutputInfo")->val.u64.word[0] & ~0x7f)
		{
			printf("\n        Ea   of semaphore\t:%#08x\n", getSpuVal("gPm.mOutputInfo")->val.u64.word[0] & ~0x7f);
		}
		printf("\n");
	} else {
		printf("No job\n");
	}
}

void dumpJobChain(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address jobChain) {
	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	PS3FlushMemoryCache(uProcessID, uThreadID);

	printf("* Jobchain Info\n");
	printf("\tEA of Jobchain   \t%#x\n", jobChain.word[0]);
	printf("\tWorkload ID      \t%d\n", readPpuUint32Array(jobChain, 29));
	printf("\tIs halted?            \t%d\n", readPpuUint8Array(jobChain, 35));
	printf("\tCurrent status code\t%#x\n", readPpuUint32Array(jobChain, 32));
	printf("\t(cause)            \t%#x\n", readPpuUint32Array(jobChain, 35));
	printf("\n\n");
}

void dumpJobQueue(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address jobQueue) {
	unsigned int i;
	sn_uint32	eaSpursAndIdWorkload;
	sn_uint32	jobQueueState_high;
	sn_uint32	jobQueueState_low;
	sn_uint32	eaCommandQueue;
	sn_uint16	isSyncMask;
	sn_uint16	hasBlockedJobMask;
	sn_uint16	depth;

	sn_uint8	commandQueueState_high;
	unsigned int phaseRead;
	sn_uint16	masterReadPointer;
	sn_uint16	readPointer;
	sn_uint16	writePointer;

	unsigned	isEnd;
	sn_address	commandQueueTop;
	unsigned	type;

	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	PS3FlushMemoryCache(uProcessID, uThreadID);

	eaCommandQueue = readPpuUint32Array(jobQueue, 4);
	depth = readPpuUint16Array(jobQueue, 10);
	jobQueueState_high = readPpuUint32Array(jobQueue, 24);
	jobQueueState_low = readPpuUint32Array(jobQueue, 25);
	eaSpursAndIdWorkload = readPpuUint32Array(jobQueue, 27);
	readPointer = readPpuUint16Array(jobQueue, 0);

	printf("* JobQueue Info\n");
	printf("\tEA of JobQueue        \t%#x\n", jobQueue.word[0]);
	printf("\tEA of SPURS           \t%#x\n", eaSpursAndIdWorkload & 0xffffff80UL);
	printf("\tWorkload ID           \t%d\n", eaSpursAndIdWorkload & 0x0f);
	printf("\tIs halted?            \t%s\n", (jobQueueState_low & (0x01 << 13)) ? "TRUE" : "FALSE");
	printf("\tCurrent status code   \t%#x\n", readPpuUint32Array(jobQueue, 128 * 30 / 4 + 1));
	printf("\t(cause)               \t%#x\n", readPpuUint32Array(jobQueue, 128 * 30 / 4 + 3));
	printf("\tmax grab            \t%d\n", (jobQueueState_low >> (8 * 2)) & 0xff);
	printf("\tmax job size        \t%d\n", readPpuUint32Array(jobQueue, 128 * 30 / 4 + 5));
	printf("\tIs busy waiting	    \t%s\n", (jobQueueState_low & (0x01 << 15)) ? "TRUE": "FALSE");
	printf("\tnumber of opened handles	\t%d\n", readPpuUint16Array(jobQueue, 104 / 2 + 4));
	printf("\tLv2 event flag[0]	\t%#x\n", readPpuUint32Array(jobQueue, 32));
	printf("\tLv2 event flag[1]	\t%#x\n", readPpuUint32Array(jobQueue, 33));

	/* job information for each tag */
	printf("\t*** Current Execution Status ***\n");
	printf("\t[Tag]\tReadPointer\tisSynchronizing\tnumWaitingJobs\thasJobSpuMask\n");
	isSyncMask = readPpuUint16Array(jobQueue, (116 + 4 + 4) / 2);
	hasBlockedJobMask = readPpuUint16Array(jobQueue, (116 + 4 + 4) / 2 + 1);
	masterReadPointer = readPpuUint16Array(jobQueue, (128 * 3 + 32 + 32) / 2);
	masterReadPointer = (hasBlockedJobMask) ? masterReadPointer : readPointer;

	for(i = 0; i < 16; i++) {
		sn_uint16	localReadPointer;
		unsigned	isSync;
		unsigned	hasBlockedJob;
		isSync = isSyncMask & (0x01 << i);
		hasBlockedJob = hasBlockedJobMask & (0x01 << i);
		localReadPointer = (hasBlockedJob) ? readPpuUint16Array(jobQueue, (128 * 3) / 2 + i) : masterReadPointer;
		printf("\t%2u\t\t\t%d\t\t\t%s\t\t\t\t%u\t\t\t\t%2x\n", i,
			localReadPointer,
			(isSync) ? "YES" : "NO",
			readPpuUint16Array(jobQueue, (48 + 16) / 2 + (15 - i)),
			readPpuUint8Array(jobQueue, 48 + (15 - i)));
	}
	printf("\t*** Command Queue ***\n");
	commandQueueState_high = readPpuUint8Array(jobQueue, 7);
	phaseRead = (commandQueueState_high >> 6) & 0x01;
	writePointer = readPpuUint16Array(jobQueue, 1);
	printf("\tEA of Queue	\t%#x\n", eaCommandQueue);
	printf("\tread pointer	\t%d\n", readPointer);
	printf("\twrite pointer	\t%d\n", writePointer);

	isEnd = 0;
	commandQueueTop.word[0] = eaCommandQueue;
	commandQueueTop.word[1] = 0;
	while(isEnd == 0) {
		sn_uint32 command_high;
		sn_uint32 command_low;

		command_high = readPpuUint32Array(commandQueueTop, readPointer* 2);
		command_low = readPpuUint32Array(commandQueueTop, readPointer * 2 + 1);
		if((command_low & 0x01) == phaseRead) {
			/* valid command */
			command_low = command_low & ~0x01UL;
			printf("\t[%d] %08x_%08x: ", readPointer, command_high, command_low);
			type = command_high & 0x03;
			switch(type) {
			case 0x00:
				/* job command */
				{
					sn_uint32 eaJobDescriptor;
					sn_uint32 eaSemaphore;
					eaJobDescriptor = (command_low & 0xfffffff0UL);
					eaSemaphore = command_high & 0xffffff80UL;
					printf("JOB: Descriptor %#x, Semaphore %#x, Tag %02u\n", eaJobDescriptor, eaSemaphore, (command_high >> 3) & 0x0fUL);
				}
				break;
			case 0x03:
				/* extension command */
				{
					unsigned exType = command_high & 0xff;
					if(exType == 0x03) {
						printf("FLUSH command\n");
					} else if (exType == 0x07) {
						sn_uint16 tagMask = command_high >> 16;
						printf("SYNC command: tag mask = %04x\n", tagMask);
					} else {
						printf("UNKNOWN EXTENSION COMMAND !!\n");
					}
				}
				break;
			default:
				printf("UNKOWN COMMAND!!!\n");
				break;
			}
		} else {
			isEnd = 1;
		}
		readPointer = readPointer + 1;
		phaseRead = (readPointer == depth) ? ((phaseRead) ? 0 : 1) : phaseRead;
		readPointer = (readPointer == depth) ? 0 : readPointer;
	}
	printf("\n\n");
}

void dumpPort(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address port) {
	sn_uint32	eaBuffer;
	sn_uint32	port_atomic_high;
	sn_uint32	port_atomic_low;

	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	PS3FlushMemoryCache(uProcessID, uThreadID);

	port_atomic_high = readPpuUint32Array(port, 4);
	port_atomic_low = readPpuUint32Array(port, 5);
	eaBuffer = readPpuUint32Array(port, 4 + 2);

	printf("* JobQueuePort Info\n");
	printf("\tEA of JobQueuePort           \t%#x\n", port.word[0]);
	printf("\tEA of JobQueue               \t%#x\n", readPpuUint32Array(port, 4 + 4));
	printf("\tJob Count                    \t%u\n", port_atomic_high);
	printf("\tEA of Descriptor Buffer      \t%#x\n", eaBuffer);
	if (eaBuffer) {
		printf("\tDepth of Buffer              \t%d\n", readPpuUint16Array(port, 8 + 6));
		printf("\tNumber of Used Entries       \t%d\n", port_atomic_low >> 16);
		printf("\tSize of Descriptor           \t%d\n", readPpuUint16Array(port, 8 + 7));
	}
	printf("\tJobQueue Handle              \t%d\n", readPpuUint32Array(port, 4 + 5));
	printf("\n\n");
}

void dumpJobQueueSemaphore(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address sem) 
{
	sn_uint32	atomic_high;
	sn_uint32	atomic_low;
	sn_uint32	count;
	unsigned waiterType;

	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	PS3FlushMemoryCache(uProcessID, uThreadID);

	atomic_high = readPpuUint32Array(sem, 0);
	atomic_low = readPpuUint32Array(sem, 1);
	count = atomic_high & 0x1fffffffUL;
	count = ((count) & 0x10000000) ? (count | 0xf0000000UL) : count;

	printf("* JobQueueSemaphore Info\n");
	printf("\tEA of JobQueueSemaphore   \t%#x\n", sem.word[0]);
	printf("\tEA of JobQueue            \t%#x\n", readPpuUint32Array(sem, 2));
	printf("\tcount	                   \t%d\n", count);

	waiterType = (atomic_high >> 29) & 0x03;

	if (atomic_high & 0x80000000UL) {
		/* someone is waiting */
		switch(waiterType) {
		case 0:
			printf("\tWAITER: PPU FIBER(%08x)\n", atomic_low);
			/* FIBER */
			break;
		case 1:
			/* SPURS task */
			printf("\tWAITER: SPURS Task(Taskset 0x%08x, TaskID %d)\n", atomic_low & ~0x7f, atomic_low & 0x7f);
			break;
		case 2:
			/* PPU Thread */
			printf("\tWAITER: PPU Thread (Lv2 EventFlag Bit %d)\n", atomic_low);
			break;
		}
	} else {
		printf("\tWaiter: NONE\n");
	}
	printf("\n\n");
}


void dumpTaskSet(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaTaskset)
{
	unsigned int i;
	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	PS3FlushMemoryCache(uProcessID, uThreadID);

	printf("* Taskset Info\n");
	printf("\tEA of Taskset         \t%#x\n",	eaTaskset.word[0]);
	printf("\tRunning Tasks         \t%08x_%08x_%08x_%08x\n",
			readPpuUint32Array(eaTaskset, 0), readPpuUint32Array(eaTaskset, 1), readPpuUint32Array(eaTaskset, 2), readPpuUint32Array(eaTaskset, 3)
		);
	printf("\tReady Tasks           \t%08x_%08x_%08x_%08x\n",
			readPpuUint32Array(eaTaskset, 4), readPpuUint32Array(eaTaskset, 5), readPpuUint32Array(eaTaskset, 6), readPpuUint32Array(eaTaskset, 7)
		);
	printf("\tDormant Tasks         \t%08x_%08x_%08x_%08x\n",
			readPpuUint32Array(eaTaskset, 8), readPpuUint32Array(eaTaskset, 9), readPpuUint32Array(eaTaskset, 10), readPpuUint32Array(eaTaskset, 11)
		);
	printf("\tExistent Tasks        \t%08x_%08x_%08x_%08x\n",
			readPpuUint32Array(eaTaskset, 12), readPpuUint32Array(eaTaskset, 13), readPpuUint32Array(eaTaskset, 14), readPpuUint32Array(eaTaskset, 15)
		);
	printf("\tSignal of Tasks       \t%08x_%08x_%08x_%08x\n",
			readPpuUint32Array(eaTaskset, 16), readPpuUint32Array(eaTaskset, 17), readPpuUint32Array(eaTaskset, 18), readPpuUint32Array(eaTaskset, 19)
		);
	printf("\tWaiting Tasks         \t%08x_%08x_%08x_%08x\n",
			readPpuUint32Array(eaTaskset, 20), readPpuUint32Array(eaTaskset, 21), readPpuUint32Array(eaTaskset, 22), readPpuUint32Array(eaTaskset, 23)
		);
	printf("\tEA of Spurs           \t0x%08x_%08x\n",	readPpuUint32Array(eaTaskset, 24), readPpuUint32Array(eaTaskset, 25));
	printf("\tTaskset Argument      \t0x%08x_%08x\n",	readPpuUint32Array(eaTaskset, 26), readPpuUint32Array(eaTaskset, 27));
	printf("\tLast Scheduled Task ID\t%u\n",	readPpuUint32Array(eaTaskset, 28));
	printf("\tWorkload ID           \t%u\n",	readPpuUint32Array(eaTaskset, 29));

	printf("\n");

	printf(    "Task ID\tTask Argument\t\t\t\tEA of ELF\tEA of Context\n");
	for (i = 0; i < 128; i++) {
		unsigned int slot = i / 32;
		sn_uint32 bit = 0x80000000 >> (i % 32);
		sn_uint32 val = readPpuUint32Array(eaTaskset, 12 + slot);
		if (val & bit) {
			sn_address eaTask;
			eaTask.word[0] = eaTaskset.word[0] + 128 + 48 * i;
			eaTask.word[1] = eaTaskset.word[1];
			printf("%3u\t0x%08x_%08x_%08x_%08x\t0x%x\t0x%x\n", i, 
					readPpuUint32Array(eaTask, 0), 
					readPpuUint32Array(eaTask, 1), 
					readPpuUint32Array(eaTask, 2), 
					readPpuUint32Array(eaTask, 3), 
					readPpuUint32Array(eaTask, 5) & ~0x07,
					readPpuUint32Array(eaTask, 7) & ~0x7f
				);
		}
	}
}

void dumpTask(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaTaskset, unsigned int idTask)
{
	sn_address eaTask;
	unsigned int i;
	unsigned int slot = idTask / 32;
	sn_uint32 bit = 0x80000000 >> (idTask % 32);
	sn_uint32 val = readPpuUint32Array(eaTaskset, 12 + slot);
	sn_uint32 eaContext;
	sn_uint32 sizeContext;

	PS3FlushMemoryCache(uProcessID, uThreadID);

	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	if (!(val & bit)) {
		printf("No such task\n");
		return;
	}
	
	eaTask.word[0] = eaTaskset.word[0] + 128 + 48 * idTask;
	eaTask.word[1] = eaTaskset.word[1];

	printf("* Task Info\n");
	printf("\tTask Argument\t\t%08x_%08x_%08x_%08x\n",
					readPpuUint32Array(eaTask, 0), 
					readPpuUint32Array(eaTask, 1), 
					readPpuUint32Array(eaTask, 2), 
					readPpuUint32Array(eaTask, 3)
				);
	printf("\tEA of ELF      \t\t0x%x\n", readPpuUint32Array(eaTask, 5) & ~0x07);

	eaContext = readPpuUint32Array(eaTask, 7);
	sizeContext = 0;
	if (eaContext != 0) {
		sizeContext = (eaContext & 0x7f) * 2 + 1;
		eaContext = eaContext & ~0x7f; 
	}
	printf("\tEA of Context  \t\t0x%x\n", eaContext);
	printf("\tSize of Context\t\t%u Kbytes\n", sizeContext);
	printf("\tLS Save Pattern\t\t0x%08x_%08x_%08x_%08x\n",
					readPpuUint32Array(eaTask, 8), 
					readPpuUint32Array(eaTask, 9), 
					readPpuUint32Array(eaTask, 10), 
					readPpuUint32Array(eaTask, 11)
				);

	val = readPpuUint32Array(eaTaskset, 16 + slot);
	if (val & bit) {
		printf("\tSignal         \t\ttrue\n");
	}
	else {
		printf("\tSignal         \t\tfalse\n");
	}

	val = readPpuUint32Array(eaTask, 5);
	if (val & 0x01) {
		printf("\tExit Code Container\tattached\n");
	}
	else {
		printf("\tExit Code Container\tN/A\n");
	}

	val = readPpuUint32Array(eaTaskset, 0 + slot);
	if (val & bit) {
		printf("\tState              \tRUNNING\n");
	}
	else {
		val = readPpuUint32Array(eaTaskset, 4 +  slot);
		if (val & bit) {
			printf("\tState              \tREADY\n");
		}
		else {
			printf("\tState              \tWAITING\n");
		}
	} 

	val = readPpuUint32Array(eaTaskset, 20 + slot);
	if (val & bit) {
		sn_address eaSavedContext;
		sn_address eaWaitingCause;
		sn_address eaFpscr;
		sn_address eaFramePtr;
		sn_uint32 linkReg;
		sn_uint32 ls2eaOffset;
		sn_uint32 waitingCause;
		sn_uint32 syncObjectAddress;

		printf("* Saved Task Context\n");
		eaWaitingCause.word[0] = eaContext + 856;
		eaWaitingCause.word[1] = 0;
		waitingCause =      readPpuUint32Array(eaWaitingCause, 1) &  0x7f;
		syncObjectAddress = readPpuUint32Array(eaWaitingCause, 1) & ~0x7f;

		switch (waitingCause) {
		case 1:
			printf("\twait cause\tSPURS Semaphore (ea: 0x%x)\n", syncObjectAddress);
			break;
		case 2:
			printf("\twait cause\tSPURS Barrier (ea: 0x%x)\n", syncObjectAddress);
			break;
		case 3:
			printf("\twait cause\tSPURS EventFlag (ea: 0x%x)\n", syncObjectAddress);
			break;
		case 4:
			printf("\twait cause\tSPURS Queue (ea: 0x%x)\n", syncObjectAddress);
			break;
		case 5:
			printf("\twait cause\tSPURS LFQueue (ea: 0x%x)\n", syncObjectAddress);
			break;
		case 6:
			printf("\twait cause\tSPURS TaskExitCodeContainer (ea: 0x%x)\n", syncObjectAddress);
			break;
		}

		eaSavedContext.word[0] = eaContext;
		eaSavedContext.word[1] = 0;
		printf("\t$sp\t%08x_%08x_%08x_%08x\n",
				readPpuUint32Array(eaSavedContext, 4), readPpuUint32Array(eaSavedContext, 5),
				readPpuUint32Array(eaSavedContext, 6), readPpuUint32Array(eaSavedContext, 7)
			);
		printf("\t$lr\t%08x_%08x_%08x_%08x\n",
				readPpuUint32Array(eaSavedContext, 0), readPpuUint32Array(eaSavedContext, 1),
				readPpuUint32Array(eaSavedContext, 2), readPpuUint32Array(eaSavedContext, 3)
			);

		for (i = 80; i < 128; i++) {
			sn_address eaReg;
			eaReg.word[0] = eaSavedContext.word[0] + 32 + (i-80) * 16;
			eaReg.word[1] = 0;
			printf("\t$%u\t%08x_%08x_%08x_%08x\n", i,
					readPpuUint32Array(eaReg, 0), readPpuUint32Array(eaReg, 1),
					readPpuUint32Array(eaReg, 2), readPpuUint32Array(eaReg, 3)
				);
		}

		eaFpscr.word[0] = eaContext + 32 + 48 * 16;
		eaFpscr.word[1] = 0;
		printf("\t$fpscr\t%08x_%08x_%08x_%08x\n",
				readPpuUint32Array(eaFpscr, 0), readPpuUint32Array(eaFpscr, 1),
				readPpuUint32Array(eaFpscr, 2), readPpuUint32Array(eaFpscr, 3)
			);
		printf("\t$tagmask\t%08x\n",	readPpuUint32Array(eaFpscr, 4));
		printf("\t$eventmask\t%08x\n",	readPpuUint32Array(eaFpscr, 5));

		printf("* Backtrace\n");
		eaSavedContext.word[0] = eaContext;
		eaSavedContext.word[1] = 0;
		linkReg = readPpuUint32Array(eaSavedContext, 0);
		printf("\t0x%08x\n", linkReg - 4);
		ls2eaOffset = eaContext + sizeContext * 1024 - 0x40000;
		eaFramePtr.word[0] = readPpuUint32Array(eaSavedContext, 4) + ls2eaOffset;
		eaFramePtr.word[1] = 0;
		eaFramePtr.word[0] = readPpuUint32Array(eaFramePtr, 0) + ls2eaOffset;
		do {
			sn_uint32 framePtr  = readPpuUint32Array(eaFramePtr, 0);
			linkReg             = readPpuUint32Array(eaFramePtr, 4);
			if (linkReg != 0) {
				printf("\t0x%08x\n", linkReg - 4);
			}
			eaFramePtr.word[0]  = framePtr + ls2eaOffset;
		} while (linkReg != 0);
	}
	else {
		printf("* No Saved Context\n");
	}
}

void dumpSpursInstance(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaSpurs)
{
	unsigned int i;
	sn_address eaAtomicNotifier;
	sn_address eaSecretService;
	sn_address eaWorkload;
	sn_address eaWorkloadEvent;
	sn_address eaTraceInfo;
	sn_address eaPpuInfo;
	sn_address eaWorkloadName;
	
	PS3FlushMemoryCache(uProcessID, uThreadID);
	
	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	printf("* Resource Info\n");
	eaPpuInfo.word[0] = eaSpurs.word[0] + 128*2 + 128*16 + 128*2 + 256 + 32*17;
	eaPpuInfo.word[1] = 0;

	printf("\tSPU thread group\t0x%x\n", readPpuUint32Array(eaPpuInfo, 4));
	for (i = 0; i < 6; i++) {
		printf("\tSPU thread[%u]\t\t0x%x\n", i, readPpuUint32Array(eaPpuInfo, 5+i));
	}

	printf("* Trace Info\n");
	eaTraceInfo.word[0] = eaSpurs.word[0] + 128*2 + 128*16;
	eaTraceInfo.word[1] = 0;
	printf("\tlog start address\t0x%x\n", readPpuUint32Array(eaTraceInfo, 0));
	for (i = 0; i < 6; i++) {
		printf("\toffset[%u]\t\t0x%x\n", readPpuUint32Array(eaTraceInfo, 1 + i));
	}
	
	printf("* Workload Info\n");
	eaSecretService.word[0] = eaSpurs.word[0] + 128;
	eaSecretService.word[1] = 0;

	for (i = 0; i < 16; i++) {
		if (readPpuUint32Array(eaSecretService, 12) & (0x80000000 >> i)) {
			sn_uint32 eaClassName;
			sn_uint32 eaInstanceName;
			
			switch (readPpuUint8Array(eaSecretService, i)) {
			case 1:
				printf("\tWorkload[%2u] state:\t\tPREPARING\n", i);
				break;			
			case 2:
				printf("\tWorkload[%2u] state:\t\tRUNNABLE\n", i);
				break;			
			case 3:
				printf("\tWorkload[%2u] state:\t\tSHUTTING DOWN\n", i);
				break;			
			case 4:
				printf("\tWorkload[%2u] state:\t\tREMOVABLE\n", i);
				break;
			default:
				printf("\tWorkload[%2u] state:\t\t*INVALID*\n", i);
				break;
			}

			eaWorkloadName.word[0] = eaSpurs.word[0] + 128*2 + 128*16 + 128*2 + 256 + 128*6 + 16*i;
			eaWorkloadName.word[1] = 0;
			
			eaClassName    = readPpuUint32Array(eaWorkloadName, 1);
			eaInstanceName = readPpuUint32Array(eaWorkloadName, 3);
			
			if (eaClassName) {
				eaWorkloadName.word[0] = eaClassName;
				printf("\t             class name:\t%s\n", readPpuString(eaWorkloadName));
			}

			if (eaInstanceName) {
				eaWorkloadName.word[0] = eaInstanceName;
				printf("\t             instance name:\t%s\n", readPpuString(eaWorkloadName));
			}

			eaWorkload.word[0] = eaSpurs.word[0] + 128*2 + 128*16 + 128*2 + 256 + 32*i;
			eaWorkload.word[1] = 0;

			printf("\t             priority table:\t%2u %2u %2u %2u %2u %2u %2u %2u\n",
					readPpuUint8Array(eaWorkload, 24),
					readPpuUint8Array(eaWorkload, 25),
					readPpuUint8Array(eaWorkload, 26),
					readPpuUint8Array(eaWorkload, 27),
					readPpuUint8Array(eaWorkload, 28),
					readPpuUint8Array(eaWorkload, 29),
					readPpuUint8Array(eaWorkload, 30),
					readPpuUint8Array(eaWorkload, 31)
				);

			printf("\t             min contention:\t%u\n", readPpuUint8Array(eaSpurs, 16*4 + i));
			printf("\t             max contention:\t%u\n", readPpuUint8Array(eaSpurs, 16*5 + i));
			printf("\t             ready count:\t%u\n",    readPpuUint8Array(eaSpurs, i));
			printf("\t             contention:\t%u\n",     readPpuUint8Array(eaSpurs, 16*2 + i));
			printf("\t             status:\t\t0x%02x\n",     readPpuUint8Array(eaSecretService, 16 + i));
			printf("\t             event:\t\t0x%02x\n",      readPpuUint8Array(eaSecretService, 16*2 + i)); 
			printf("\t             binary address:\t0x%x\n", readPpuUint32Array(eaWorkload, 1));
			printf("\t             argument:\t\t0x%08x%08x\n",  readPpuUint32Array(eaWorkload, 2), readPpuUint32Array(eaWorkload, 3));
		}
		else {
			printf("\tWorkload[%2u] state:\t\tNON EXISTENT\n", i);
		}
	}
}

void dumpSpursQueue(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaSpursQueue)
{
	unsigned int i;
	sn_uint32 eaSpurs;
	sn_uint32 eaTaskset;
	sn_int32  head;
	sn_int32  tail;
	sn_uint32 size;
	sn_uint32 depth;
	sn_uint32 count;
	BOOL rlock;
	BOOL wlock;

	PS3FlushMemoryCache(uProcessID, uThreadID);

	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	head  = (sn_int32)readPpuUint32Array(eaSpursQueue, 0);
	tail  = (sn_int32)readPpuUint32Array(eaSpursQueue, 1);
	size  = readPpuUint32Array(eaSpursQueue, 2);
	depth = readPpuUint32Array(eaSpursQueue, 3);
	rlock = (head < 0);  
	wlock = (tail < 0);
	
	if (rlock) head = ~head;
	if (wlock) tail = ~tail;
	count = (tail >= head) ? tail - head : (tail + depth) - (head - depth);
	if (head >= depth) head -= depth;
	if (tail >= depth) tail -= depth;

	eaSpurs   = readPpuUint32Array(eaSpursQueue, 27);
	eaTaskset = readPpuUint32Array(eaSpursQueue, 25);

	printf("\tSPURS:\t\t0x%x\n", eaSpurs);
	if (eaTaskset) {
		printf("\tTaskset:\t0x%x\n", eaTaskset);
	}
	else {
		printf("\tTaskset:\t*Inter Workload*\n");
	}
	printf("\tBuffer:\t\t0x%x\n",       	readPpuUint32Array(eaSpursQueue, 5));
	printf("\tSize:\t\t%u\n",	size);
	printf("\tDepth:\t\t%u\n", depth);
	printf("\tPush ptr:\t%u %s\n", tail, wlock ? "(locked)" : "");
	printf("\tPop ptr:\t%u %s\n", head, rlock ? "(locked)" : "");
	printf("\tCount:\t\t%u\n", count);

	switch (readPpuUint32Array(eaSpursQueue, 7)) {
	case 0:
		printf("\tDirection:\tCELL_SPURS_QUEUE_SPU2SPU\n");
		break;
	case 1:
		printf("\tDirection:\tCELL_SPURS_QUEUE_SPU2PPU\n");
		printf("\tEvent Queue ID:\t0x%x\n", readPpuUint32Array(eaSpursQueue, 29));
		printf("\tSPU Event Port:\t%d\n", readPpuUint32Array(eaSpursQueue, 6));
		if (rlock && count == 0) {
			printf("\tPPU thread is waiting\n");
		}
		break;
	case 2:
		printf("\tDirection:\tCELL_SPURS_QUEUE_PPU2SPU\n");
		printf("\tEvent Queue ID:\t0x%x\n", readPpuUint32Array(eaSpursQueue, 29));
		printf("\tSPU Event Port:\t%d\n", readPpuUint32Array(eaSpursQueue, 6));
		if (wlock && count == 0) {
			printf("\tPPU thread is waiting\n");
		}
		break;
	}
	
	printf("\tWaiting tasks\n");

	printf("\t\tpop:\t");
	for (i = 0; i < readPpuUint8Array(eaSpursQueue, 32); i++) {
		printf("[Wid:%u, Tid:%u] ", 
				readPpuUint8Array(eaSpursQueue, 65+i),
				readPpuUint8Array(eaSpursQueue, 33+i)
			);
	}
	printf("\n");
	
	printf("\t\tpush:\t");
	for (i = 0; i < readPpuUint8Array(eaSpursQueue, 48); i++) {
		printf("[Wid:%u, Tid:%u] ", 
				readPpuUint8Array(eaSpursQueue, 81+i),
				readPpuUint8Array(eaSpursQueue, 49+i)
			);
	}
	printf("\n");
}

void dumpSpursBarrier(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaSpursBarrier)
{
	PS3FlushMemoryCache(uProcessID, uThreadID);
	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	printf("\tTaskset:\t0x%x\n",	readPpuUint32Array(eaSpursBarrier, 13));
	printf("\tCount:\t\t%u\n",		readPpuUint32Array(eaSpursBarrier, 0));
	printf("\tTotal:\t\t%u\n", 	    readPpuUint32Array(eaSpursBarrier, 1));
	printf("\tWaiting Task:\t0x%08x_0x%08x_0x%08x_0x%08x\n",
			readPpuUint32Array(eaSpursBarrier, 4),
			readPpuUint32Array(eaSpursBarrier, 5),
			readPpuUint32Array(eaSpursBarrier, 6),
			readPpuUint32Array(eaSpursBarrier, 7)
		);
	printf("\tNotifying Task:\t0x%08x_0x%08x_0x%08x_0x%08x\n",
			readPpuUint32Array(eaSpursBarrier, 8),
			readPpuUint32Array(eaSpursBarrier, 9),
			readPpuUint32Array(eaSpursBarrier, 10),
			readPpuUint32Array(eaSpursBarrier, 11)
		);
}

void dumpSpursSemaphore(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaSpursSemaphore)
{
	unsigned int i;
	PS3FlushMemoryCache(uProcessID, uThreadID);
	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	printf("\tSPURS:\t\t0x%x\n",		readPpuUint32Array(eaSpursSemaphore, 13));
	printf("\tTaskset:\t0x%x\n",	readPpuUint32Array(eaSpursSemaphore, 15));
	printf("\tCount:\t\t%d\n",		readPpuUint32Array(eaSpursSemaphore, 0));
	printf("\tWaiting Tasks:\t");
	for (i = 0; i < readPpuUint8Array(eaSpursSemaphore, 16); i++) {
		printf("[Wid:%u, Tid:%u] ", 
				readPpuUint8Array(eaSpursSemaphore, 33+i),
				readPpuUint8Array(eaSpursSemaphore, 17+i)
			);
	}
	printf("\n");
}

void dumpSpursEventFlag(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaSpursEventFlag)
{
	unsigned int i;
	PS3FlushMemoryCache(uProcessID, uThreadID);
	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	if (readPpuUint8Array(eaSpursEventFlag, 13)) {
		printf("\tSPURS:\t\t0x%x\n",	readPpuUint32Array(eaSpursEventFlag, 29));	
	}
	else {
		printf("\tTaskset:\t0x%x\n",	readPpuUint32Array(eaSpursEventFlag, 29));
	}

	switch (readPpuUint8Array(eaSpursEventFlag, 14)) {
	case 0:
		printf("\tDirection:\tSPU -> SPU\n");
		break;
	case 1:
		printf("\tDirection:\tSPU -> PPU\n");
		printf("\tEvent Queue ID:\t0x%x\n",	readPpuUint32Array(eaSpursEventFlag, 31));
		printf("\tSPU Event Port:\t%d\n",	readPpuUint8Array(eaSpursEventFlag, 12));
		break;
	case 2:
		printf("\tDirection:\tPPU -> SPU\n");
		break;
	default:
		printf("\tDirection:\t*INVALID*\n");
		break;
	}

	switch (readPpuUint8Array(eaSpursEventFlag, 15)) {
	case 0:
		printf("\tClear Mode:\tAUTO\n");
		break;
	case 1:
		printf("\tClear Mode:\tMANUAL\n");
		break;
	default:
		printf("\tClear Mode:\t*INVALID*\n");
		break;
	}
	
	printf("\tFlag Value:\t0x%02x%02x\n",
			readPpuUint8Array(eaSpursEventFlag, 0), 
			readPpuUint8Array(eaSpursEventFlag, 1)
		);
	
	if (readPpuUint8Array(eaSpursEventFlag, 14) == 1) {
		if (readPpuUint16Array(eaSpursEventFlag, 2)) {
			printf("\tPPU Waiting:\tRequest\tReceived\tMode\n");
			printf("\t\t\t%04x\t%04x\t", 
					readPpuUint16Array(eaSpursEventFlag, 8),
					readPpuUint16Array(eaSpursEventFlag, 24)
				);
			switch (readPpuUint8Array(eaSpursEventFlag, 6)) {
			case 0:
				printf("EVENTFLAG_OR\n");
				break;
			case 1:
				printf("EVENTFLAG_AND\n");
				break;
			default:
				printf("*INVALID*\n");
				break;
			}
		} 
	}
	else {
		printf("\tWaiting SPURS Task\n");
		printf("\t[Wid, Tid]\tRequest\tReceived\tIsWakedUp\tMode\n");
		for (i = 0; i < 16; i++) {
			if (readPpuUint16Array(eaSpursEventFlag, 4) & (0x8000 >> i)) {
				printf("\t[%u, %u]\t\t0x%04x\t0x%04x\t\t",
						readPpuUint8Array(eaSpursEventFlag, 80+i),
						readPpuUint8Array(eaSpursEventFlag, 96+i),
						readPpuUint16Array(eaSpursEventFlag, 8+i),
						readPpuUint16Array(eaSpursEventFlag, 24+i)
					);
				if (readPpuUint16Array(eaSpursEventFlag, 1) & (0x8000 >> i)) {
					printf("Yes\t\t");
				}
				else {
					printf("No\t\t");
				}
				if (readPpuUint16Array(eaSpursEventFlag, 5) & (0x8000 >> i)) {
					printf("EVENTFLAG_AND\n");
				}
				else {
					printf("EVENTFLAG_OR\n");
				}
			}
		}	
	} 
}

void dumpSpursTaskExitCode(sn_uint32 uProcessID, sn_uint64 uThreadID, sn_address eaSpursTaskExitCode)
{
	unsigned int i;
	sn_uint32 state;
	PS3FlushMemoryCache(uProcessID, uThreadID);
	guProcessID = uProcessID;
	guPpuThreadID = uThreadID;

	state = readPpuUint32Array(eaSpursTaskExitCode, 8);

	if (state & 0x00000001) {
		sn_uint64 eaTasksetOfJoined = readPpuUint64Array(eaSpursTaskExitCode, 10);
		printf("\tAttached:\tyes  (Taskset=%#x, TaskID=%d\n",  eaTasksetOfJoined.word[0],  (sn_int8)readPpuUint8Array(eaSpursTaskExitCode, 57));
	}
	else {
		printf("\tAttached:\tno\n");
	}

	if (state & 0x00000004) {
		if (state & 0x00000008) {
			printf("\tExit Code:\t<task has been abort>\n");
		}
		else {
			printf("\tExit Code:\t%d\n", readPpuUint32Array(eaSpursTaskExitCode, 10));
		} 
	}
	else {
		printf("\tExit Code:\t<not available yet>\n");
	}

	if (state & 0x00010000) {
		printf("\tWaiter:\tPPU thread\n");
	}
	else {
		if (state & 0x00020000) {
			printf("\tWaiter:\tSPURS task (taskset = %p, id = %u)\n",
					readPpuUint32Array(eaSpursTaskExitCode, 13),
					(sn_int8)readPpuUint8Array(eaSpursTaskExitCode, 56)
				);
		}
		else {
			printf("\tWaiter:\tnone\n");
		}
	}
}

void dumpSPURS(sn_uint32 uProcessID)
{
	int i;
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	guProcessID = uProcessID;
	pPPUThreadID = (sn_uint64 *)malloc(sizeof(sn_uint64) * 256);
	pSPUThreadID = (sn_uint64 *)malloc(sizeof(sn_uint64) * 256);
	if (SN_SUCCEEDED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		for(i = 0; i < NumSPUThreads; i++) {
			int j;
			sn_uint8 pPmGuid[16];
			sn_address pm_top_addr;
			pm_top_addr.word[0] = 0xa00;
			dumpKernel(pSPUThreadID[i]);
			/* fetch GUID of policy module */
			if (SN_FAILED(PS3GetMemory(uProcessID, pSPUThreadID[i], (void *)pPmGuid, pm_top_addr, 16))) {
				printf("\n10 Error: %s", GetLastErrorString());
				break;
			}
			for(j = 0; j < 16; j++) {
				if (pPmGuid[j] != gpJmGuid[j]) break;
			}
			if (j == 16) {
				printf("Workload -> SPURS job chain\n");
				dumpJM(uProcessID, pSPUThreadID[i]);
			}
			for(j = 0; j < 16; j++) {
				if (pPmGuid[j] != gpJqmGuid[j]) break;
			}
			if (j == 16) {
				printf("Workload -> SPURS job queue\n");
				dumpJQM(uProcessID, pSPUThreadID[i]);
			}
			for(j = 0; j < 16; j++) {
				if (pPmGuid[j] != gpTmGuid[j]) break;
			}
			if (j == 16) {
				printf("Workload -> SPURS taskset\n");
			}
		}
	}

	if (guPpuThreadID.word[0]) {
		BOOL hasJobChain = FALSE;
		for(i = 0; i < 16; i++) {
			if (gJobChains[i].word[0]) {
				if (!hasJobChain) {
					printf("\n\n");
					printf("- PPU --------------------------------\n");
					hasJobChain = TRUE;
				}
				dumpJobChain(uProcessID, pPPUThreadID[0], gJobChains[i]);
			}
		}
	}

	free(pPPUThreadID);
	free(pSPUThreadID);
}

BOOL isPPUThread(sn_uint32 uProcessID, sn_uint64 uThreadID) {

	int i;
	sn_uint32 NumPPUThreads;
	sn_uint64* pPPUThreadID = NULL;
	sn_uint32 NumSPUThreads;
	sn_uint64* pSPUThreadID = NULL;

	pPPUThreadID = (sn_uint64*)malloc(sizeof(sn_uint64) * 256);
	pSPUThreadID = (sn_uint64*)malloc(sizeof(sn_uint64) * 256);
	if (SN_FAILED(PS3GetThreadList(uProcessID, &NumPPUThreads, pPPUThreadID, &NumSPUThreads, pSPUThreadID)))
	{
		printf("\n11 Error: %s", GetLastErrorString());
		return FALSE;
	}

	for(i = 0; i < NumPPUThreads; i++) {
		if ((pPPUThreadID[i].word[1] == uThreadID.word[1]) &&
			(pPPUThreadID[i].word[0] == uThreadID.word[0]))
		{
			free(pPPUThreadID);
			free(pSPUThreadID);
			return TRUE;
		}
	}

	free(pPPUThreadID);
	free(pSPUThreadID);
	return FALSE;
}

/*
 * Local Variables:
 * mode: C
 * c-file-style: "stroustrup"
 * tab-width: 4
 * End:
 * vim:sw=4:sts=4:ts=4
 */
