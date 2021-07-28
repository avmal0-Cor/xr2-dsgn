/* Copyright (C)2001-2006 by NVIDIA Corporation.  All rights reserved.
 * Modified by Sony Computer Entertainment Inc under license from NVIDIA Corporation.
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * 
 * SCEI CONFIDENTIAL
 */


/*
 * Struct definitions for resource management.
 */

#ifndef _H_GCM_RESOURCE_H_
#define _H_GCM_RESOURCE_H_

typedef volatile struct
{
    struct
    {                   /*                                   0000-    */
        GLuint nanoseconds[2];  /* nanoseconds since Jan. 1, 1970       0-   7*/
    }
    timeStamp;               /*                                       -0007*/
    GLuint value;               /* info returned depends on method   0008-000b*/
    GLuint zero;                /* always written to zero            000c-000f*/
}
jsGcmGetReport;

/* memory data structures */
typedef volatile struct
{
    struct
    {                      /*                                   0000-    */
        GLuint nanoseconds[2];        /* nanoseconds since Jan. 1, 1970       0-   7*/
    }
    timeStamp;                  /*                                       -0007*/
    GLuint      info32;                 /* info returned depends on method   0008-000b*/
    GLushort    info16;                 /* info returned depends on method   000c-000d*/
    GLushort    status;                 /* user sets bit 15, NV sets status  000e-000f*/
}
jsGcmNotification;

/* pio flow control data structure */
typedef volatile struct
{
    GLuint Ignored00[0x010];
    GLuint Put;                     /* put offset, write only           0040-0043*/
    GLuint Get;                     /* get offset, read only            0044-0047*/
    GLuint Reference;               /* reference value, read only       0048-004b*/
    GLuint Ignored01[0x1];
    GLuint SetReference;            /* set reference value              0050-0053*/
    GLuint TopLevelGet;             /* top level get offset, read only  0054-0057*/
    GLuint Ignored02[0x2];
    GLuint SetContextDmaSemaphore;  /* set sema ctxdma, write only      0060-0063*/
    GLuint SetSemaphoreOffset;      /* set sema offset, write only      0064-0067*/
    GLuint SetSemaphoreAcquire;     /* set sema acquire, write only     0068-006b*/
    GLuint SetSemaphoreRelease;     /* set sema release, write only     006c-006f*/
    GLuint Ignored03[0x7e4];
}
jsGcmControlDma;

// max amount of semaphore we allocate space for
#define JSGCM_MAX_USER_SEMAPHORES                   256
#define JSGCM_PAGE_SIZE                             0x1000 // 4KB

#define JSGCM_LM_MAX_TOTAL_QUERIES						          800
#define JSGCM_LM_MAX_ZPASS_REPORTS						          (JSGCM_LM_MAX_TOTAL_QUERIES - 10)
#define JSGCM_LM_MAX_USER_QUERIES						          (JSGCM_LM_MAX_ZPASS_REPORTS)

#define JSGCM_LM_ZCULL_STAT0							          (JSGCM_LM_MAX_ZPASS_REPORTS + 1)
#define JSGCM_LM_ZCULL_STAT1							          (JSGCM_LM_MAX_ZPASS_REPORTS + 2)
#define JSGCM_LM_ZCULL_STAT2							          (JSGCM_LM_MAX_ZPASS_REPORTS + 3)
#define JSGCM_LM_ZCULL_STAT3							          (JSGCM_LM_MAX_ZPASS_REPORTS + 4) 

// For main memory query PSGL is going to enable 5000 at any given time 
// compared to the 800 currently for the Local memory queries 
// However, if you really need more than 5k then change the line below 
// and recompile [RSTENSON]
// Maximum value for JSGCM_MM_MAX_TOTAL_QUERIES is 65,000 
#define JSGCM_MM_MAX_TOTAL_QUERIES						          5000 // Should be plenty.  
#define JSGCM_MM_MAX_ZPASS_REPORTS								  (JSGCM_MM_MAX_TOTAL_QUERIES - 10)
#define JSGCM_MM_MAX_USER_QUERIES						          (JSGCM_MM_MAX_ZPASS_REPORTS)

#define JSGCM_MM_ZCULL_STAT0							          (JSGCM_MM_MAX_ZPASS_REPORTS + 1)
#define JSGCM_MM_ZCULL_STAT1							          (JSGCM_MM_MAX_ZPASS_REPORTS + 2)
#define JSGCM_MM_ZCULL_STAT2							          (JSGCM_MM_MAX_ZPASS_REPORTS + 3)
#define JSGCM_MM_ZCULL_STAT3							          (JSGCM_MM_MAX_ZPASS_REPORTS + 4) 

// For 2.50 PSGL will use reports in main memory by default 
// To revert to reports in local memory comment out this define 
#define JSGCM_USE_MAIN_MEMORY_REPORTS
#ifdef JSGCM_USE_MAIN_MEMORY_REPORTS
	#define JSGCM_MAX_USER_QUERIES JSGCM_MM_MAX_USER_QUERIES
#else
	#define JSGCM_MAX_USER_QUERIES JSGCM_LM_MAX_USER_QUERIES
#endif 


#define JSGCM_357C_NOTIFIERS_MAXCOUNT               11

// notifiers in host memory
struct jsGcmHostNotifierMemory
{
    // signals channel errors
    jsGcmNotification channelDmaError[2];

    // flip notifier
    jsGcmNotification flip[JSGCM_357C_NOTIFIERS_MAXCOUNT];

    // never really used: remove eventually
    jsGcmNotification scratch[2];
};


// 16 byte aligned semaphores
struct  jsGcmSemaphore
{
    GLuint val;
    GLuint pad0;
    GLuint pad1;
    GLuint pad2;
};

// semaphore storage
struct jsGcmSemaphoreMemory
{
    jsGcmSemaphore userSemaphores[JSGCM_MAX_USER_SEMAPHORES];
};

enum
{
    // dma contexts
    _JSGCM_CHANNEL_DMA_SCRATCH_NOTIFIER,
    _JSGCM_CONTEXT_DMA_MEMORY_FRAME_BUFFER,
    _JSGCM_CONTEXT_DMA_TO_MEMORY_GET_REPORT,
    _JSGCM_CONTEXT_DMA_MEMORY_HOST_BUFFER,
    _JSGCM_CONTEXT_DMA_MEMORY_SEMAPHORE_RW,
    _JSGCM_CONTEXT_DMA_MEMORY_SEMAPHORE_RO,

    // classes
    _JSGCM_CURIE_PRIMITIVE,
    _JSGCM_MEM2MEM_HOST_TO_VIDEO,

    _JSGCM_IMAGEFROMCPU,
    _JSGCM_SCALEDIMAGE,
    _JSGCM_CONTEXT_2D_SURFACE,
    _JSGCM_CONTEXT_SWIZ_SURFACE,

    _JSGCM_HANDLE_COUNT
};

struct jsGcmResource
{
    char *localAddress;
	GLuint localSize;
    GLuint MemoryClock;
    GLuint GraphicsClock;

    GLuint handles[_JSGCM_HANDLE_COUNT];

    unsigned long long ioifMappings[32];

    char  * linearMemory;
    unsigned int persistentMemorySize;

    // host memory window the gpu can access
    char *  hostMemoryBase;
    GLuint  hostMemorySize;
    GLuint  hostMemoryReserved;

    // offset of dmaPushBuffer relative to its DMA CONTEXT
    unsigned long dmaPushBufferOffset;
    char *  dmaPushBuffer;
    GLuint  dmaPushBufferSize;
    void*   dmaControl;

    // all kind of notifers
    jsGcmHostNotifierMemory  *hostNotifierBuffer;

    // semaphores
    jsGcmSemaphoreMemory    *semaphores;
};

#endif
