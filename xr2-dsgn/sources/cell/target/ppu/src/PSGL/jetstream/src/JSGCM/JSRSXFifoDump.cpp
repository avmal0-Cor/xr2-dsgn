/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


/*
 * Function definitions for dumping FIFO contents (related to PSGL / GCM HUD)
 */

#include <PSGL/private.h>

///////////////////////////////////////////////////////////////////////////
#include <sys/gpio.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/return_code.h>

#include <JSGCM/JSGcmUtils.h>

#ifdef JS_RSXFIFODUMP

#include <librsxfifodump.h>
#include <PSGL/JSRSXFifoDump.h>

///////////////////////////////////////////////////////////////////////////


typedef enum {
    FILE_TYPE_RAW1 = 0,
    FILE_TYPE_RAW2 = 1,
    FILE_TYPE_TXT1 = 2,
    FILE_TYPE_TXT2 = 3,
    FILE_TYPE_NUM_TYPES = 4
} FileType_t;

static FILE* s_pRSXFifoFiles[FILE_TYPE_NUM_TYPES];
static uint64_t s_uiLastSwitch;
static uint64_t s_uiSwitchOverride = 0x0;

uint32_t        jsFIFOCacheSize = 1024*1024*70;
uint32_t*       jsFIFOCache = NULL;
uint32_t*       jsFIFOCacheCur = NULL;

///////////////////////////////////////////////////////////////////////////

void _jsRSXFifoDumpBegin( _jsRSXFifoDumpMode_t eMode )
{
    JS_ASSERT( s_uiSwitchOverride == 0 );
    s_uiSwitchOverride = ( uint64_t )eMode;
}

void _jsRSXFifoDumpEnd()
{
    JS_ASSERT( s_uiSwitchOverride != 0 );
    s_uiSwitchOverride = 0;
}

///////////////////////////////////////////////////////////////////////////

void _jsRSXFifoDumpGetUniqueFilename( const char* szName, const char* szExtension, char* szDest )
{
    char szBuf[256];
    const int iMax = 1000;
    for ( int i = 1; i < iMax; ++i )
    {
        sprintf( szBuf, "%s%04i%s", szName, i, szExtension );
        FILE* f = fopen( szBuf, "r" );
        if ( f == NULL )
        {
            strcpy( szDest, szBuf );
            return;
        }

        fclose( f );
    }
    {
        sprintf( szBuf, "%s%04i%s", szName, iMax, szExtension );
        strcpy( szDest, szBuf );
        return;
    }
}

void _jsRSXFifoDumpUsage()
{
    printf( "Jetstream RSXFifoDump\n"
            "------------------------------------------------------------------\n"
            "Switch positions on the DEH faceplate control this system.\n"
            "Switch positions are printed as with bit 3 on the left, i.e.:\n"
            " 1 1 0 1   --->   sw3=1 sw2=1 sw1=0 sw0=1\n"
            "\n"
            " Switch State | Consequence\n"
            " 0 0 0 0      | No action\n"
            " 0 0 0 1      | Disassemble to stdout\n"
            " 0 0 1 0      | Disassemble to /app_home/fifo1_xxx.txt\n"
            " 0 0 1 1      | Write raw fifo to /app_home/fifo1_xxx.raw\n"
            "------------------------------------------------------------------\n"
          );
}


FILE* _jsRSXFifoDumpGetFile( FileType_t eType )
{
    if ( JS_UNLIKELY( s_pRSXFifoFiles[eType] == 0 ) )
    {
        const char* szPrefix = NULL;
        const char* szSuffix = NULL;
        bool bRaw = false;
        switch (( uint32_t )eType )
        {
            case FILE_TYPE_RAW1:
                szPrefix = "/app_home/fifo"; szSuffix = "a.raw"; bRaw = true;
                break;

            case FILE_TYPE_RAW2:
                szPrefix = "/app_home/fifo"; szSuffix = "b.raw"; bRaw = true;
                break;

            case FILE_TYPE_TXT1:
                szPrefix = "/app_home/fifo"; szSuffix = "a.txt"; bRaw = false;
                break;

            case FILE_TYPE_TXT2:
                szPrefix = "/app_home/fifo"; szSuffix = "b.txt"; bRaw = false;
                break;
        }
        char szFifoTxtName[128];
        _jsRSXFifoDumpGetUniqueFilename( szPrefix, szSuffix, szFifoTxtName );
        s_pRSXFifoFiles[eType] = fopen( szFifoTxtName, bRaw ? "wb" : "w" );
        fprintf( stdout, "Opened %s for fifo debugging\n", szFifoTxtName ); fflush( stdout );
    }
    return s_pRSXFifoFiles[eType];
}

///////////////////////////////////////////////////////////////////////////

#define JS_RSXFIFODUMP_GETSWITCHSTATE() \
  uint64_t uiSwitch = 0; \
  if(s_uiSwitchOverride != 0) { \
    uiSwitch = s_uiSwitchOverride; \
  } else { \
    sys_gpio_get(SYS_GPIO_DIP_SWITCH_DEVICE_ID,&uiSwitch); uiSwitch &= SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS; \
  } \
  if(JS_UNLIKELY(uiSwitch != s_uiLastSwitch)) _jsRSXFifoDumpOnSwitchStateChanged(uiSwitch);

//  printf("Switch: %x\n", (uint32_t)uiSwitch);

extern "C" void _jsRSXFifoDumpInit()
{
    sys_gpio_get( SYS_GPIO_DIP_SWITCH_DEVICE_ID, &s_uiLastSwitch ); s_uiLastSwitch &= SYS_GPIO_DIP_SWITCH_USER_AVAILABLE_BITS; \
    _jsRSXFifoDumpUsage();
    memset( s_pRSXFifoFiles, 0, sizeof( s_pRSXFifoFiles ) );
    fprintf( stdout, "jsrsxfifodump: allocating %i bytes of memory\n", jsFIFOCacheSize );
    jsFIFOCache = ( uint32_t* )malloc( jsFIFOCacheSize );
    jsFIFOCacheCur = jsFIFOCache;
}

void _jsRSXFifoDumpOnSwitchStateChanged( uint64_t uiNewSwitch )
{
    fprintf( stdout, "jsRSXFifoDump: Closing existing files\n" ); fflush( stdout );
    for ( int i = 0; i < FILE_TYPE_NUM_TYPES; ++i )
    {
        if ( s_pRSXFifoFiles[i] )
        {
            fclose( s_pRSXFifoFiles[i] );
            s_pRSXFifoFiles[i] = NULL;
        }
    }
    s_uiLastSwitch = uiNewSwitch;
}

extern "C" void _jsRSXFifoDumpOnFlush()
{
    jsGcmFifo& fifo = _jsGcmState.fifo;
    JS_RSXFIFODUMP_GETSWITCHSTATE();
    uint32_t* pNewPut = ( uint32_t* )fifo.current;
    uint32_t* pLastPut = ( uint32_t* )fifo.lastPutWritten;
    switch ( uiSwitch )
    {
        case 0x0:
            break;
        case 0x1:
        {
            cellRSXFifoDisassembleToFile( stderr, pLastPut, pNewPut, CELL_RSXFIFO_MODE_METHODS );
        }
        break;
        case 0x2:
        {
            FILE* pFile = _jsRSXFifoDumpGetFile( FILE_TYPE_TXT1 );
            cellRSXFifoDisassembleToFile( pFile, pLastPut, pNewPut, CELL_RSXFIFO_MODE_METHODS );
        }
        break;
        case 0x3:
        {
            FILE* pFile = _jsRSXFifoDumpGetFile( FILE_TYPE_RAW1 );
            cellRSXFifoFlattenToFile( pFile, pLastPut, pNewPut );
        }
        break;
        case 0x4:
        {
            uint32_t bytes_remaining = jsFIFOCacheSize - (( char* )jsFIFOCacheCur - ( char* )jsFIFOCache );
            uint32_t bytes_written = cellRSXFifoFlattenToMemory( jsFIFOCacheCur, bytes_remaining, pLastPut, pNewPut );
            jsFIFOCacheCur = ( uint32_t* )((( char* )jsFIFOCacheCur ) + bytes_written );
        }
        case 0x10: // do nothing
            break;
    }
}

extern "C" void _jsRSXFifoDumpDebugFinish()
{
}


extern "C" int32_t cellGcmIoOffsetToAddress( uint32_t ioOffset, void **address )
{
    *address = (( char* )_jsGcmState.hostMemoryBase + ioOffset );
    return CELL_OK;
}


// simply decode and dump the fifo from the the beginning to the current position
void _jsDecodeAndDumpFifo()
{
    uint32_t* pNewPut = ( uint32_t* )_jsGcmState.fifo.current;
    uint32_t* pHead = ( uint32_t* )_jsGcmState.fifo.lastPutWritten; //dmaPushBuffer;//lastPutWritten; //dmaPushBuffer;
    FILE* pFile = _jsRSXFifoDumpGetFile( FILE_TYPE_TXT1 );
    cellRSXFifoDisassembleToFile( pFile, pHead, pNewPut, CELL_RSXFIFO_MODE_METHODS );
}


#endif // JS_RSXFIFODUMP
