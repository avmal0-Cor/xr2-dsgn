/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2009 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/
/*
 * ProDG script to watch SPURS status
 */

#include "spurs_prodg.h"

int main()
{

	int i;
	sn_val addrCreateJobChain, addrCreateJobChainWithAttribute, addrShutdownJobChain;
	BOOL createJobChainHasBP, createJobChainWithAttributeHasBP, shutdownJobChainHasBP;
	FILE *pFile;
	char *sce_ps3_root ;
	char spurs_jm_path[256], spurs_jq_path[256], spurs_tm_path[256];
	guPpuThreadID.word[0] = 0;

	/* read SPURS job module GUID */
	sce_ps3_root = getenv("SCE_PS3_ROOT");
	strcpy(spurs_jm_path, sce_ps3_root);
	strcat(spurs_jm_path, "\\target\\images\\spurs_jm2.elf");
	if ((pFile = fopen(spurs_jm_path, "r")) == 0) {
		char file_path[256], file_name[256];
		printf("Cannot open spurs_jm2.elf\n");
		if (SN_FAILED(FileSelectionDialog(file_path, file_name))) {
			return -1;
		}
		if ((pFile = fopen(file_path, "r")) == 0) {
			return -1;
		}
	}
	fseek(pFile, 0x100L, SEEK_SET);
	fread(gpJmGuid, 1, 16, pFile);

	/* read SPURS job module GUID */
	sce_ps3_root = getenv("SCE_PS3_ROOT");
	strcpy(spurs_jq_path, sce_ps3_root);
	strcat(spurs_jq_path, "\\target\\images\\spurs_jq.elf");
	if ((pFile = fopen(spurs_jq_path, "r")) == 0) {
		char file_path[256], file_name[256];
		printf("Cannot open spurs_jq.elf\n");
		if (SN_FAILED(FileSelectionDialog(file_path, file_name))) {
			return -1;
		}
		if ((pFile = fopen(file_path, "r")) == 0) {
			return -1;
		}
	}
	fseek(pFile, 0x100L, SEEK_SET);
	fread(gpJqmGuid, 1, 16, pFile);

	/* read SPURS task module GUID */
	sce_ps3_root = getenv("SCE_PS3_ROOT");
	strcpy(spurs_tm_path, sce_ps3_root);
	strcat(spurs_tm_path, "\\target\\images\\spurs_tm.elf");
	if ((pFile = fopen(spurs_tm_path, "r")) == 0) {
		char file_path[256], file_name[256];
		printf("Cannot open spurs_tm.elf\n");
		if (SN_FAILED(FileSelectionDialog(file_path, file_name))) {
			return -1;
		}
		if ((pFile = fopen(file_path, "r")) == 0) {
			return -1;
		}
	}
	fseek(pFile, 0x100L, SEEK_SET);
	fread(gpTmGuid, 1, 16, pFile);

	addrCreateJobChain.val.Address.word[0]   = addrCreateJobChain.val.Address.word[1]   = 0;
	addrCreateJobChainWithAttribute.val.Address.word[0]   = addrCreateJobChainWithAttribute.val.Address.word[1]   = 0;
	addrShutdownJobChain.val.Address.word[0] = addrShutdownJobChain.val.Address.word[1] = 0;
	createJobChainHasBP = FALSE;
	createJobChainWithAttributeHasBP = FALSE;
	shutdownJobChainHasBP = FALSE;
	for(i = 0; i < 16; i++) gJobChains[i].word[0]= gJobChains[i].word[1] = 0;

	
	RequestNotification(NT_KEYDOWN);		//  Allows exit from script
	RequestNotification(NT_THREAD_STOP);
	RequestNotification(NT_BREAK);

	while(1)
	{
		sn_notify Notify;
		GetNotification(NT_ANY, &Notify);	
		switch (Notify.eNotifyType) {
		case NT_BREAK:
		{
			sn_val val;
			sn_uint32 uProcessID = ((BP_PARAM_INFO *)Notify.pParam2)->uProcessID;
			sn_uint64 uThreadID = ((BP_PARAM_INFO *)Notify.pParam2)->uThreadID;
			sn_uint32 pc = ((BP_PARAM_INFO *)Notify.pParam2)->uPC;
			if (pc == addrCreateJobChain.val.Address.word[0] ||
				pc == addrCreateJobChainWithAttribute.val.Address.word[0]) {
				for(i = 0; i < 16; i++) {
					if (gJobChains[i].word[0] == 0 && gJobChains[i].word[1] == 0) break;
				}
				if (SN_FAILED(PS3EvaluateExpression(uProcessID, uThreadID, &val, "$r4"))) {
					printf("\n12 Error: %s",	GetLastErrorString());
				}
				gJobChains[i].word[0] = val.val.Address.word[0];
				gJobChains[i].word[1] = val.val.Address.word[1];
				printf("\n\n!! JobChain created !!\tea=%#x\n\n", gJobChains[i].word[0]);
			} else {
				if (SN_FAILED(PS3EvaluateExpression(uProcessID, uThreadID, &val, "$r3"))) {
					printf("\n13 Error: %s",	GetLastErrorString());
				}
				for(i = 0; i < 16; i++) {
					if (gJobChains[i].word[0] == val.val.u64.word[0]) {
						printf("\n\n!! JobChain shutdown !!\tea=%#x\n\n", gJobChains[i].word[0]);
						gJobChains[i].word[0] = 0;
						gJobChains[i].word[1] = 0;
					}
				}
			}
			if (SN_FAILED(PS3ThreadStart(uProcessID, uThreadID))) {
				printf("\n14 Error: %s",	GetLastErrorString());
			}
			break;
		}
		case NT_THREAD_STOP:
		{
			sn_uint32 uProcessID;
			sn_uint64 uThreadID;
			memcpy(&uThreadID, Notify.pParam1, sizeof(sn_uint64));
			memcpy(&uProcessID, Notify.pParam2, sizeof(sn_uint32));
			if (isPPUThread(uProcessID, uThreadID) && (!createJobChainHasBP || !shutdownJobChainHasBP)) {
				guPpuThreadID = uThreadID;
				if (!createJobChainHasBP) {
					if (SN_SUCCEEDED(PS3EvaluateExpression(uProcessID, uThreadID, &addrCreateJobChain,
														   "cellSpursCreateJobChain")))
					{
						if (SN_SUCCEEDED(PS3AddBreakPoint(uProcessID, uThreadID, addrCreateJobChain.val.Address, 1))) {
							createJobChainHasBP = TRUE;
						} else {
							printf("\n15 Error: %s",	GetLastErrorString());
						}
					}
				}
				if (!createJobChainWithAttributeHasBP) {
					if (SN_SUCCEEDED(PS3EvaluateExpression(uProcessID, uThreadID, &addrCreateJobChainWithAttribute,
														   "cellSpursCreateJobChainWithAttribute")))
					{
						if (SN_SUCCEEDED(PS3AddBreakPoint(uProcessID, uThreadID, addrCreateJobChainWithAttribute.val.Address, 1))) {
							createJobChainWithAttributeHasBP = TRUE;
						} else {
							printf("\n15 Error: %s",	GetLastErrorString());
						}
					}
				}
				if (!shutdownJobChainHasBP) {
					if (SN_SUCCEEDED(PS3EvaluateExpression(uProcessID, uThreadID, &addrShutdownJobChain,
														   "cellSpursShutdownJobChain")))
					{
						if (SN_SUCCEEDED(PS3AddBreakPoint(uProcessID, uThreadID, addrShutdownJobChain.val.Address, 1))) {
							shutdownJobChainHasBP = TRUE;
						} else {
							printf("\n17 Error: %s",	GetLastErrorString());
						}
					} else {
						printf("\n18 tid=%x Error: %s",	uThreadID.word[0], GetLastErrorString());
					}
				}
			}
			dumpSPURS(uProcessID);
			break;
		}
		case NT_KEYDOWN:
			if ((sn_int32)Notify.pParam1 == 'X')
			{
				printf("Exiting\n");
				PS3RemoveBreakPoint(guProcessID, guPpuThreadID, addrCreateJobChain.val.Address);
				PS3RemoveBreakPoint(guProcessID, guPpuThreadID, addrCreateJobChainWithAttribute.val.Address);
				PS3RemoveBreakPoint(guProcessID, guPpuThreadID, addrShutdownJobChain.val.Address);
				return 0;
			}
		default:
			printf("Type: %d", Notify.eNotifyType);
			break;
		}
	}

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
