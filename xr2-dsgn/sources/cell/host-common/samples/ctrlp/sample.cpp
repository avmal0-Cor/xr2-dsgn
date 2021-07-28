/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2008 Sony Computer Entertainment Inc.
* All Rights Reserved.
*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef WIN32
#else
#include <unistd.h>
#endif

#include <assert.h>
#include "dtlib.h"
#include "dtlib_error.h"
#include "ctrlp.h"

#define DT_DEFAULT_TARGET_PORT     8530
#define DT_DEFAULT_TARGET_NAME     "PS3_LPAR"
#define DT_DEFAULT_NETM_ENV_NAME   "DTNETM"

#define PROTOCOL_NO     0x0310
#define PROTOCOL_PORT   0

#define PADDATA_NUM    (240)

using namespace std;
M__USING_NAMESPACE

int     flag = 1;
char *dt_program_name;
dtlib       deci;

/*E
 * File: sample.cpp
 * Description:
 *   This source code is written for showing the usage of padrecorder
 *
 *   1. Connect to target
 *   2. Wait for initializing pad.
 *   3. Record pad data.
 *   4. Play pad data.
 *
 */


Deci3CtrlpPadData paddata[PADDATA_NUM];

BOOL CALLBACK dcmp_recv( HCONNECT connH, BYTE *data, int size )
{
    printf( "dcmp_recv %d\n", size );
    return true;
}

void DumpHex(unsigned char *buf, int cnt){
	unsigned char *bp;
	int i,j,n;
	for(bp = buf, i=0; i< cnt; i+= 16, bp +=16){
		if(16 < (n = cnt - i))
			n = 16;
		printf(" 0x%04x: ", bp - buf);
		for(j = 0; j < 16 && j < n; j++)
			printf("%02x%s", bp[j],  ((j == 7)? "  ": " "));
		for(printf(" "); j < 16; j++)
			printf("   ");
		for(j = 0; j < 16 && j < n; j++)
			printf("%c", (' ' <= bp[j] && bp[j] < 0x7f)?
			bp[j]: '.');
		printf("\n");

	}
}

void DumpPadData(unsigned char*buf, int n){
	Deci3CtrlpPadData * p = (Deci3CtrlpPadData*)buf;
	for(int i=0; i<n; i++){
		uint32_t lo = ntohl( p->pad_time_lo);
//		printf(" %d.%d:port 0x%x len 0x%x status 0x%x\n",lo/1000000, lo%1000000, p->port_no, p->len , p->port_status);
		DumpHex((unsigned char*)&p->button , p->len * 2);
		p++;
	}
}

int ctrlp_rec_start( HDECI hdeci){
	Deci3CtrlpRecStart res;
	res.header.code = htons(DECI3_CTRLP_CODE_REC_START);
	res.header.size = htons(sizeof(res));
	return deci.dt_send( hdeci, (BYTE *)&res, (int)sizeof(res));
}

int ctrlp_rec_stop( HDECI hdeci){
	Deci3CtrlpRecStop res;
	res.header.code = htons(DECI3_CTRLP_CODE_REC_STOP);
	res.header.size = htons(sizeof(res));
	return deci.dt_send( hdeci, (BYTE *)&res, (int)sizeof(res));
}

int ctrlp_play_start( HDECI hdeci){
	Deci3CtrlpPlayStart res;
	res.header.code = htons(DECI3_CTRLP_CODE_PLAY_START);
	res.header.size = htons(sizeof(res));
	return deci.dt_send( hdeci, (BYTE *)&res, (int)sizeof(res));
}

int ctrlp_play_stop( HDECI hdeci){
	Deci3CtrlpPlayStop res;
	res.header.code = htons(DECI3_CTRLP_CODE_PLAY_STOP);
	res.header.size = htons(sizeof(res));
	return deci.dt_send( hdeci, (BYTE *)&res, (int)sizeof(res));
}

int ctrlp_play_data( HDECI hdeci, Deci3CtrlpPadData *pdata, int n){
	Deci3CtrlpPlayData res;
	int i;
	res.header.code = htons(DECI3_CTRLP_CODE_PLAY_DATA);
	res.header.size = htons((uint16_t)(sizeof(res.header)+sizeof(Deci3CtrlpPadData)*n));
	Deci3CtrlpPadData *p = &res.data[0];
	for(i=0;i<n;i++){
		*p++= *pdata++;
	}
	return deci.dt_send( hdeci, (BYTE *)&res, (int)sizeof(res.header)+sizeof(Deci3CtrlpPadData)*n);
}


BOOL CALLBACK recv_ctrlp_func( HDECI hdeci, BYTE *data, int size, int status)
{
	static unsigned int reccount = 0;
	static unsigned int playcount = 0;
	if(status){
		return true;
	}

	Deci3CtrlpHeader* pCtrlpHeader = (Deci3CtrlpHeader*)data;
	pCtrlpHeader->code = ntohs(pCtrlpHeader->code);
	pCtrlpHeader->size = ntohs(pCtrlpHeader->size);
	switch(pCtrlpHeader->code){
	case DECI3_CTRLP_CODE_INITR:
		assert(pCtrlpHeader->size == sizeof(Deci3CtrlpInitr));
		printf("version %08x\n", ntohl(*(uint32_t*)(pCtrlpHeader+1)));
		ctrlp_rec_start(hdeci);
		break;
	case DECI3_CTRLP_CODE_REC_STARTR:
		assert(pCtrlpHeader->size == sizeof(Deci3CtrlpRecStartr));
		printf("rec startr ret %08x\n", ntohl(*(uint32_t*)(pCtrlpHeader+1)));
		break;
	case DECI3_CTRLP_CODE_REC_STOPR:
		assert(pCtrlpHeader->size == sizeof(Deci3CtrlpRecStopr));
		printf("rec stopr ret %08x\n", ntohl(*(uint32_t*)(pCtrlpHeader+1)));
		ctrlp_play_start(hdeci);
		break;
	case DECI3_CTRLP_CODE_REC_DATAR:
		assert(pCtrlpHeader->size >= sizeof(Deci3CtrlpRecDatar));
//		printf("%d : rec datar size %08x\n", reccount, pCtrlpHeader->size);
		{
			int n = (pCtrlpHeader->size - sizeof(Deci3CtrlpRecDatar)) / sizeof(Deci3CtrlpPadData);
			Deci3CtrlpPadData* ppaddata = (Deci3CtrlpPadData*)(pCtrlpHeader+1);
			if(reccount >= PADDATA_NUM){
				ctrlp_rec_stop(hdeci);
				reccount++;
			}else{
				while((n--> 0) && (reccount < PADDATA_NUM)){
					paddata[reccount++] = *ppaddata++;
				}
			}
		}
//		DumpHex((unsigned char*)(pCtrlpHeader+1), pCtrlpHeader->size-sizeof(Deci3CtrlpHeader));
//		DumpPadData((unsigned char*)(pCtrlpHeader+1), (pCtrlpHeader->size-sizeof(Deci3CtrlpHeader))/sizeof(Deci3CtrlpPadData));

		break;
	case DECI3_CTRLP_CODE_PLAY_STARTR:
		assert(pCtrlpHeader->size == sizeof(Deci3CtrlpPlayStartr));
		printf("play startr ret %08x\n", *(uint32_t*)(pCtrlpHeader+1));
		ctrlp_play_data(hdeci, &paddata[playcount], CTRLP_PAD_MAX_DATA);
		playcount += CTRLP_PAD_MAX_DATA;

		break;
	case DECI3_CTRLP_CODE_PLAY_STOPR:
		assert(pCtrlpHeader->size == sizeof(Deci3CtrlpPlayStopr));
		printf("play stopr ret %08x\n", *(uint32_t*)(pCtrlpHeader+1));
		playcount=0;
		reccount=0;
		return false;
		break;
	case DECI3_CTRLP_CODE_PLAY_DATAR:
		assert(pCtrlpHeader->size == sizeof(Deci3CtrlpPlayDatar));
//		printf("%d:play datar ret %08x\n", playcount ,*(uint32_t*)(pCtrlpHeader+1));
		if(playcount<PADDATA_NUM){
			ctrlp_play_data(hdeci, &paddata[playcount], CTRLP_PAD_MAX_DATA);
			playcount+= CTRLP_PAD_MAX_DATA;
		}else
			ctrlp_play_stop(hdeci);
		break;
	default:
		printf("unknown code %08x\n", pCtrlpHeader->code);
		break;
	}

	return true;
}


static int usage(int f_true){
	if(!f_true)
		return(0);
	printf("Usage: %s [<option>]...\n", dt_program_name);
	printf("  <option>:\n");
	printf("    -d <host>"
		"set hostname\n");
	exit(1);
	return 0;
}


static char* get_target(int argc, char**argv){
	char *target = getenv(DT_DEFAULT_NETM_ENV_NAME);
	for(--argc, ++argv; 0 < argc && *argv[0] == '-'; --argc, ++argv){
		if(!strcmp("-d", argv[0]))
			usage(--argc <= 0), target = *++argv;
		else
			usage(1);
	}
	usage(0 < argc);
	usage(!target);
	return target;
}

int     main( int argc, char **argv )
{
    HCONNECT    hconnect;
    HDECI       hdeci;
    
	dt_program_name = argv[0];
	char *dt_target = get_target(argc, argv);
	
    hconnect = deci.dt_connect(dt_target, DT_DEFAULT_TARGET_PORT, dt_program_name );

	if( NULL == hconnect )
    {
        printf( "dt_connect error\n" );
        return 1;
    }
    
    deci.dt_set_dcmp_status_function( hconnect, dcmp_recv );
    
    hdeci = deci.dt_register(hconnect, PROTOCOL_NO, PROTOCOL_PORT, TARGET, DT_DEFAULT_TARGET_NAME );
    if( (HDECI)NULL == hdeci )
    {
        printf( "dt_register error\n" );
        return 2;
    }
    
    deci.dt_add_recv_function( hdeci, recv_ctrlp_func );
        
    while(flag)
    {
        HANDLE handle = deci.dt_select( hconnect, 0 );

		if(handle == NULL){
			int error = deci.dt_getlasterror();

			if(error == DTLIB_WAIT_TIMEOUT) { //timeout
#ifdef WIN32
				Sleep( 100 );
#else
				usleep(100000);
#endif
			}
			else {
				std::string msg = deci.dt_geterrormsg(error);
				if(msg.size())
					printf("%s\n", msg.c_str());
				if(error == DTLIB_SOCKET_CLOSE) // close socket
					return 0;
				else
					break;
			}
		}
    }
    
    deci.dt_unregister(hdeci);
    deci.dt_disconnect(hconnect);
    return 0;
}

