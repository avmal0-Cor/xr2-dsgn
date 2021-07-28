/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * JSGcmFifo.h - api for jsGcmFifo structure
 */

#ifndef _H_JSGCM_FIFO_H_
#define _H_JSGCM_FIFO_H_

#include <cell/gcm.h>

// contains the state memory addressing fifo and gcm config.
struct jsGcmState;
extern jsGcmState _jsGcmState;

int32_t _jsOutOfSpaceCallback( struct CellGcmContextData *con, uint32_t space );

typedef uint32_t jsFifoData;

// some other useful push buf defines/commands
#define JSGCM_NON_INCREMENT        (0x40000000)
#define JSGCM_NOP()                (0x00000000)
#define JSGCM_JUMP(addr)           (0x20000000 | (addr))
#define JSGCM_CALL(addr)           (0x00000002 | (addr))
#define JSGCM_RETURN()             (0x00020000)
#define JSGCM_MAX_METHOD_COUNT     (2047)
#define JSGCM_COUNT_SHIFT          (18)
#define JSGCM_SUBCHANNEL_SHIFT     (13)
#define JSGCM_METHOD_SHIFT         (0)

#ifdef JSGCM_DEBUG_CHANNEL_ERROR
void _jsGcmDebugFinish();
#define JSGCM_DEBUG_FINISH() _jsGcmDebugFinish();
#else
#define JSGCM_DEBUG_FINISH() ((void)0)
#endif

#define DEFAULT_FIFO_BLOCK_SIZE     (0x10000)   // 64KB
#define FIFO_RESERVE_SIZE   8 // reserved words needed at the beginning of the fifo

// all fifo related data is kept here
struct jsGcmFifo: public CellGcmContextData
{
    // dmaControl for allocated channel
    jsGcmControlDma *dmaControl;

    // for storing the start of the push buffer
    // begin will be moving around
    jsFifoData *dmaPushBufferBegin;
    jsFifoData *dmaPushBufferEnd;

    // Fifo block size
    GLuint  fifoBlockSize;

    // pushbuffer location etc.
    // members around here were moved up
    unsigned long dmaPushBufferOffset;
    GLuint dmaPushBufferSizeInWords;

    // the last Put value we wrote
    jsFifoData *lastPutWritten;

    // the last Get value we know of
    jsFifoData *lastGetRead;

    // cached value of last reference write
    GLuint lastSWReferenceWritten;

    // cached value of lastSWReferenceWritten at most
    // recent fifo flush
    GLuint lastSWReferenceFlushed;

    // cached value of last reference read
    GLuint lastHWReferenceRead;

    jsFifoData *dmaPushBufferGPU;
    int spuid;

public:
    inline void updateLastGetRead()
    {
        jsFifoData* tmp = ( jsFifoData * )(( char* )dmaPushBufferBegin - dmaPushBufferOffset + ( *(( volatile GLuint* ) & dmaControl->Get ) ) );
        // _only_ update lastGetRead if the Get is in our pushbuffer
        if (( tmp >= begin ) && ( tmp <= end ) ) lastGetRead = tmp;
    }
};



//#define _JS_ENABLE_FIFO_PRINT 1
#ifdef _JS_ENABLE_FIFO_PRINT
void _jsGcmFifoPrint( const char* msg, jsGcmFifo* fifo, const char* file, int line );
#define _JS_PRINT_FIFO(x) _jsGcmFifoPrint(x,&_jsGcmState.fifo,__FILE__,__LINE__)
#else
#define _JS_PRINT_FIFO(x)  ;
#endif


// fifo-interface
void        _jsGcmFifoInit( jsGcmFifo *fifo, void *dmaControl, unsigned long dmaPushBufferOffset, jsFifoData *dmaPushBuffer, GLuint dmaPushBufferSize );
jsFifoData* _jsGcmFifoWaitForFreeSpace( jsGcmFifo *fifo, GLuint spaceInWords );
void        _jsGcmFifoFlush( jsGcmFifo *fifo );
void        _jsGcmFifoFinish( jsGcmFifo *fifo );
GLuint      _jsGcmFifoPutReference( jsGcmFifo *fifo );
GLuint      _jsGcmFifoReadReference( jsGcmFifo *fifo );
GLboolean   _jsGcmFifoReferenceInUse( jsGcmFifo *fifo, GLuint reference );
void        _jsGcmFifoDelayRegisterRead( jsGcmFifo *fifo );

void		_jsGcmFifoPushMarker( const GLubyte* szName );
void		_jsGcmFifoPopMarker();

#endif
