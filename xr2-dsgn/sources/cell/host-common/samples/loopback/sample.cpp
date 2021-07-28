/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc.
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


#include "dtlib.h"
#include "dtlib_error.h"

#define     TARGET_IP       "127.0.0.1"
#define     TARGET_PORT     8530
#define     TARGET_NAME     "PS3_LPAR"

#define     PROTOCOL_NO     0x1001
#define     PROTOCOL_PORT   0


using namespace std;
M__USING_NAMESPACE

int     flag = 1;


BOOL CALLBACK dcmp_recv( HCONNECT connH, BYTE *data, int size )
{
    printf( "dcmp_recv\n" );
    return true;
}



BOOL CALLBACK recv_func( HDECI hdeci, BYTE *data, int size, int status)
{
    printf( "%s\n",(char *)data );
    flag = 0;
    return true;
}


int     main( int argc, char **argv )
{
    HCONNECT    hconnect;
    HDECI       hdeci;
    dtlib       deci;
    char        *cpMessage;
    
    hconnect = deci.dt_connect( TARGET_IP, TARGET_PORT, argv[0] );
    if( NULL == hconnect )
    {
        printf( "dt_connect error\n" );
        return 1;
    }
    
    deci.dt_set_dcmp_status_function( hconnect, dcmp_recv );
    
    hdeci = deci.dt_register(hconnect, PROTOCOL_NO, PROTOCOL_PORT, TARGET, TARGET_NAME );
    if( (HDECI)NULL == hdeci )
    {
        printf( "dt_register error\n" );
        return 2;
    }
    
    deci.dt_add_recv_function( hdeci, recv_func );
    
    cpMessage = "Hello !!";
    deci.dt_send( hdeci, (BYTE *)cpMessage, (int)strlen(cpMessage) + 1 );
    
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

