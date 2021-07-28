/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#ifndef __CELL_SAIL_SOURCE_CHECK_H__
#define __CELL_SAIL_SOURCE_CHECK_H__

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/******************************************************************************
 * public declarations
 *****************************************************************************/

#ifndef CELL_SAIL_SOURCE_CHECK_MEMSET
#define CELL_SAIL_SOURCE_CHECK_MEMSET memset
#endif

#ifndef CELL_SAIL_SOURCE_CHECK_PRINTF
#define CELL_SAIL_SOURCE_CHECK_PRINTF printf
#endif

#ifndef CELL_SAIL_SOURCE_CHECK_MALLOC
#define CELL_SAIL_SOURCE_CHECK_MALLOC(pArg, size) malloc(size)
#endif

#ifndef CELL_SAIL_SOURCE_CHECK_FREE
#define CELL_SAIL_SOURCE_CHECK_FREE(pArg, p) free(p)
#endif

typedef void (*CellSailSourceCheckFuncError)(const char* pMsg, int line);

typedef struct CellSailSourceCheckStream {
    int         streamType;
    void       *pMediaInfo;
    const char *pUri;
} CellSailSourceCheckStream;

typedef struct CellSailSourceCheckResource {
    CellSailSourceCheckStream ok;
    CellSailSourceCheckStream readError;
    CellSailSourceCheckStream openError;
    CellSailSourceCheckStream startError;
    CellSailSourceCheckStream runningError;
} CellSailSourceCheckResource;

static int
cellSailSourceCheck(CellSailSource              *pSource,
                    const CellSailSourceFuncs   *pFuncs,                    
                    void                        *pArg,
                    const char                  *pProtocolName,
                    CellSailSourceCheckResource *pRes,
                    CellSailSourceCheckFuncError pHandler,
                    void                        *pHandlerArg);


/******************************************************************************
 * private declarations and definitions
 *****************************************************************************/

static uint64_t sCellSailSourceCheckBarProgressing = 0;

#define CELL_SAIL_SOURCE_CHECK_P(...) \
    if (sCellSailSourceCheckBarProgressing) { \
        CELL_SAIL_SOURCE_CHECK_PRINTF("\n"); \
        sCellSailSourceCheckBarProgressing = 0; \
    } \
    CELL_SAIL_SOURCE_CHECK_PRINTF("[check] " __VA_ARGS__)

// gcc cpp hack
#define CELL_SAIL_CHECK_N_TO_STR0(n) #n
#define CELL_SAIL_CHECK_N_TO_STR(n) CELL_SAIL_CHECK_N_TO_STR0(n)
#define CELL_SAIL_CHECK_ERRORP(...) \
    if (sCellSailSourceCheckBarProgressing) { \
        CELL_SAIL_SOURCE_CHECK_PRINTF("\n"); \
        sCellSailSourceCheckBarProgressing = 0; \
    } \
    CELL_SAIL_SOURCE_CHECK_PRINTF("[check:L" CELL_SAIL_CHECK_N_TO_STR(__LINE__) "] ** " __VA_ARGS__)

#define CELL_SAIL_CHECK(pEnv, x, pMsg, exp) \
    if (! (x)) { \
        if ((pEnv)->pHandler) { \
            (*((pEnv)->pHandler))(pMsg, __LINE__); \
        } else { \
            CELL_SAIL_CHECK_ERRORP("failed!!! %s\n", pMsg); \
        } \
        exp; \
    }

#define CELL_SAIL_CHECK_RETVAL(pEnv, ret, pMsg, exp) \
    if (0 > (ret)) { \
        if ((pEnv)->pHandler) { \
            (*((pEnv)->pHandler))(pMsg, __LINE__); \
        } else { \
            CELL_SAIL_CHECK_ERRORP("failed!!! err=0x%X %s\n", ret, pMsg); \
        } \
        exp; \
    }

#define CELL_SAIL_SOURCE_CHECK_MAX_MESSAGES (32)
#define CELL_SAIL_SOURCE_CHECK_MUTEX_NAME "_sai_m0"
#define CELL_SAIL_SOURCE_CHECK_COND_NAME  "_sai_c0"

#define CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_0 (1)
#define CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_1 (1)
#define CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_2 (1)
#define CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_3_0 (1)
#define CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_3_1 (1)
#define CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_3_2 (1)

#define CELL_SAIL_SOURCE_CHECK_MAX_ITEMS (32)

#define CELL_SAIL_SOURCE_CHECK_PROBABILITY_6() \
    (0 == (15 & cellSailSourceCheckGetRandomValue()))

#define CELL_SAIL_SOURCE_CHECK_PROBABILITY_13() \
    (0 == (7 & cellSailSourceCheckGetRandomValue()))

#define CELL_SAIL_SOURCE_CHECK_PROBABILITY_25() \
    (0 == (3 & cellSailSourceCheckGetRandomValue()))

#define CELL_SAIL_SOURCE_CHECK_PROBABILITY_50() \
    (0 == (8 & cellSailSourceCheckGetRandomValue()))

#define CELL_SAIL_SOURCE_CHECK_PROBABILITY_75() \
    (0 != (3 & cellSailSourceCheckGetRandomValue()))

#define CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv, pStream, ret, exp) \
    if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_13()) { \
        ret = cellSailSourceCheck_read_sync(pEnv, pStream); \
        if (0 > ret) { \
            exp; \
        } \
    }

typedef struct CellSailSourceCheckMessage {
    int type;
    int arg0;
    int arg1;
    int reserved;
} CellSailSourceCheckMessage;

typedef struct CellSailSourceCheckQueue {
    uint32_t            max;
    volatile uint32_t   count;
    volatile uint32_t   head;
    volatile uint32_t   tail;
    sys_mutex_t         mutex;
    sys_cond_t          cond;
    CellSailSourceCheckMessage messages[CELL_SAIL_SOURCE_CHECK_MAX_MESSAGES];
} CellSailSourceCheckQueue;

typedef struct CellSailSourceCheckEnv {
    CellSailSource                 *pSource;
    const CellSailSourceFuncs      *pFuncs;
    void                           *pArg;
    const char                     *pProtocolName;
    CellSailSourceCheckResource    *pRes;
    CellSailSourceCheckQueue        queue;
    CellSailSourceStreamingProfile  profile;
    union {
        struct {
            CellPamfReader          reader;
            uint8_t                *pHeader;
            uint64_t                headerSize;
            uint64_t                streamSize;
            uint64_t                duration;
            uint32_t                maxBitrate;
        } pamf;
    } db;
    CellSailSourceCheckFuncError    pHandler;
    void                           *pHandlerArg;

    const CellSailSourceBufferItem *pItems[CELL_SAIL_SOURCE_CHECK_MAX_ITEMS];
    struct {
        int numStreamOut;
        int numCheckedOut;
        int checkOutIndex;
        int checkInIndex;
        uint64_t checkedOutSize;
        uint64_t checkedInSize;
    } runningContext;
} CellSailSourceCheckEnv;


static uint32_t
cellSailSourceCheckGetRandomValue(void)
{
    // xor128
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w;
    return( w=(w^(w>>19))^(t^(t>>8)) );
}

static int
cellSailSourceCheckQueueInitialize(CellSailSourceCheckQueue *pQueue)
{
    CELL_SAIL_SOURCE_CHECK_MEMSET(pQueue,
                                  0,
                                  sizeof(CellSailSourceCheckQueue));
    int ret = CELL_OK;
    {
        sys_mutex_attribute_t mutexAttr;
        sys_mutex_attribute_initialize(mutexAttr);
        sys_mutex_attribute_name_set(mutexAttr.name,
                                     CELL_SAIL_SOURCE_CHECK_MUTEX_NAME);
        ret = sys_mutex_create(&pQueue->mutex, &mutexAttr);

        if (0 > ret) {
            return -1;
        }
    }
    {
        sys_cond_attribute_t condAttr;
        sys_cond_attribute_initialize(condAttr);
        sys_cond_attribute_name_set(condAttr.name,
                                    CELL_SAIL_SOURCE_CHECK_COND_NAME);
        ret = sys_cond_create(&pQueue->cond,
                              pQueue->mutex,
                              &condAttr);
        if (0 > ret) {
            return -1;
        }
    }
    pQueue->max = CELL_SAIL_SOURCE_CHECK_MAX_MESSAGES;
    return CELL_OK;
}

static int
cellSailSourceCheckQueueFinalize(CellSailSourceCheckQueue *pQueue)
{
    (void)sys_cond_destroy(pQueue->cond);
    (void)sys_mutex_destroy(pQueue->mutex);
    return CELL_OK;
}

static void
cellSailSourceCheckQueuePut(CellSailSourceCheckQueue *pQueue,
                            int                       type,
                            int                       arg0,
                            int                       arg1)
{
    (void)sys_mutex_lock(pQueue->mutex, 0);

    while (pQueue->max <= pQueue->count) {
        (void)sys_cond_wait(pQueue->cond, 0);
        // should not reach here
    }
    CellSailSourceCheckMessage *pMessage;
    pMessage = &(pQueue->messages[pQueue->head]);
    pMessage->type = type;
    pMessage->arg0 = arg0;
    pMessage->arg1 = arg1;
    pQueue->head = (pQueue->head + 1) % pQueue->max;
    if (0 == pQueue->count++) {
        (void)sys_cond_signal(pQueue->cond);
    }
    (void)sys_mutex_unlock(pQueue->mutex);
}

static void
cellSailSourceCheckQueueGet(CellSailSourceCheckQueue *pQueue,
                            int                      *pType,
                            int                      *pArg0,
                            int                      *pArg1,
                            unsigned                  timeout)
{
    (void)sys_mutex_lock(pQueue->mutex, 0);

    while (pQueue->count <= 0) {
        int ret = CELL_OK;
        ret = sys_cond_wait(pQueue->cond, timeout);
        if (ETIMEDOUT == ret) {
            *pType = -1;
            goto label_fin;
    }    }
    CellSailSourceCheckMessage *pMessage;
    pMessage = &pQueue->messages[pQueue->tail];
    *pType = pMessage->type;
    *pArg0 = pMessage->arg0;
    *pArg1 = pMessage->arg1;
    pQueue->tail = (pQueue->tail + 1) % pQueue->max;

    if (pQueue->max == pQueue->count--) {
        (void)sys_cond_signal(pQueue->cond);
        // should not reach here
    }
 label_fin:
    (void)sys_mutex_unlock(pQueue->mutex);
}

static void
cellSailSourceCheckDiagNotifyHandler(void *pHandlerArg,
                                     int   type,
                                     int   arg0,
                                     int   arg1)
{
    CellSailSourceCheckEnv *pEnv = (CellSailSourceCheckEnv*)pHandlerArg;
    cellSailSourceCheckQueuePut(&pEnv->queue,
                                type,
                                arg0,
                                arg1);
}

static int
cellSailSourceCheck_initialize(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    ret = cellSailSourceInitialize(pEnv->pSource,
                                   pEnv->pFuncs,
                                   pEnv->pArg);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "cellSailSourceInitialize() failed",
                           return ret);
    (void)cellSailSourceSetDiagHandler(pEnv->pSource,
                                       cellSailSourceCheckDiagNotifyHandler,
                                       pEnv);
    return CELL_OK;
}

    
static int
cellSailSourceCheck_finalize(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    (void)cellSailSourceSetDiagHandler(pEnv->pSource, 0, 0);
    ret = cellSailSourceFinalize(pEnv->pSource);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "cellSailSourceFinalize() failed",
                           ;);
    return CELL_OK;
}

static int
cellSailSourceCheck_makeup(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    ret = (*(pEnv->pFuncs->pMakeup))(pEnv->pArg,
                                     pEnv->pProtocolName);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncMakeup() failed",
                           return ret);
    return CELL_OK;
}

static int
cellSailSourceCheck_cleanup(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    ret = (*(pEnv->pFuncs->pCleanup))(pEnv->pArg);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncCleanup() failed",
                           return ret);
    return CELL_OK;
}


static int
cellSailSourceCheck_read_sync(CellSailSourceCheckEnv    *pEnv,
                              CellSailSourceCheckStream *pStream)
{
    static const int default_header_size = 2048;
    int ret = CELL_OK;

    uint64_t  offset  = 0;
    uint32_t  size    = default_header_size;
    uint8_t  *pHeader = (uint8_t *)
                        CELL_SAIL_SOURCE_CHECK_MALLOC(pEnv->pArg, size);
    CELL_SAIL_CHECK(pEnv,
                    pHeader,
                    "malloc failed",
                    return -1);

    uint64_t totalSize;
    ret = (*(pEnv->pFuncs->pReadSync))(pEnv->pArg,
                                       pStream->streamType,
                                       pStream->pMediaInfo,
                                       pStream->pUri,
                                       offset,
                                       pHeader,
                                       size,
                                       &totalSize);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncReadSync() failed",
                           goto label_fin);
    ret = CELL_OK;
 label_fin:
    CELL_SAIL_SOURCE_CHECK_FREE(pEnv->pArg, pHeader);
    return ret;
}

static int
cellSailSourceCheck_getCapabilities(CellSailSourceCheckEnv    *pEnv,
                                    CellSailSourceCheckStream *pStream,
                                    uint64_t                  *pCapabilities)
{
    int ret = CELL_OK;

    CELL_SAIL_CHECK(
        pEnv,
        CELL_SAIL_STREAM_PAMF == pStream->streamType,
        "source should support pamf for the moment",
        return -1);

    uint64_t capabilities;
    ret = (*(pEnv->pFuncs->pGetCapabilities))(pEnv->pArg,
                                              pStream->streamType,
                                              pStream->pMediaInfo,
                                              pStream->pUri,
                                              &capabilities);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncGetCapabilities() failed",
                           return ret);
    CELL_SAIL_CHECK(
        pEnv,
        0 ==
        ((~(uint64_t)
          (CELL_SAIL_SOURCE_CAPABILITY_SEEK_ABSOLUTE_BYTE_POSITION |
           CELL_SAIL_SOURCE_CAPABILITY_PAUSE                       |
           CELL_SAIL_SOURCE_CAPABILITY_GAPLESS                     |
           CELL_SAIL_SOURCE_CAPABILITY_EOS)) &
         capabilities),
        "unknown capability",
        return -1);
    CELL_SAIL_CHECK(
        pEnv,
        ((CELL_SAIL_STREAM_PAMF != pStream->streamType) ||
         (CELL_SAIL_SOURCE_CAPABILITY_SEEK_ABSOLUTE_BYTE_POSITION & capabilities)),
        "abosolute- byte-seek must be specified for pamf",
        return -1);
    CELL_SAIL_CHECK(
        pEnv,
        ((CELL_SAIL_STREAM_PAMF != pStream->streamType) ||
         (CELL_SAIL_SOURCE_CAPABILITY_EOS & capabilities)),
        "eos must be specified for pamf",
        return -1);
    *pCapabilities = capabilities;
    return CELL_OK;
}

static int
cellSailSourceCheck_open(CellSailSourceCheckEnv    *pEnv,
                         CellSailSourceCheckStream *pStream)
{

    int ret = CELL_OK;
    int type;
    int arg0 = 0;
    int arg1 = 0;
    CellSailSourceStreamingProfile  profile;
    CELL_SAIL_SOURCE_CHECK_MEMSET(&profile, 0, sizeof(profile));
    profile.numItems   = 2;
    profile.streamSize = pEnv->db.pamf.streamSize;
    profile.maxBitrate = pEnv->db.pamf.maxBitrate;
    profile.duration   = pEnv->db.pamf.duration;

    /* open */
    (*(pEnv->pFuncs->pOpen))(pEnv->pArg,
                             pStream->streamType,
                             pStream->pMediaInfo,
                             pStream->pUri,
                             &profile);

    /* read sync */
    CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv,
                                     &pEnv->pRes->ok,
                                     ret,
                                     return ret);

    cellSailSourceCheckQueueGet(&pEnv->queue,
                                &type,
                                &arg0,
                                &arg1,
                                /*timeout*/ 0);
    CELL_SAIL_CHECK(pEnv,
                    CELL_SAIL_SOURCE_DIAG_NOTIFY_CALL_COMPLETED == type &&
                    /*status*/ arg0 >= 0,
                    "CellSailSourceFuncOpen() notify failed",
                    return -1);
    return CELL_OK;
}

static int
cellSailSourceCheck_close(CellSailSourceCheckEnv *pEnv)
{
    /* close */
    int ret = CELL_OK;
    int type;
    int arg0 = 0;
    int arg1 = 0;

    (*(pEnv->pFuncs->pClose))(pEnv->pArg);

    /* read sync */
    CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv,
                                     &pEnv->pRes->ok,
                                     ret,
                                     return ret);

    cellSailSourceCheckQueueGet(&pEnv->queue,
                                &type,
                                &arg0,
                                &arg1,
                                /*timeout*/ 0);
    CELL_SAIL_CHECK(pEnv,
                    CELL_SAIL_SOURCE_DIAG_NOTIFY_CALL_COMPLETED == type &&
                    /*status*/ arg0 >= 0,
                    "CellSailSourceFuncClose() notify failed",
                    return -1);
    CELL_SAIL_SOURCE_CHECK_MEMSET(&pEnv->runningContext,
                                  0,
                                  sizeof(pEnv->runningContext));
    return CELL_OK;
}

static int
cellSailSourceCheck_inquireCapability(CellSailSourceCheckEnv     *pEnv,
                                      CellSailSourceCheckStream  *pStream,
                                      CellSailSourceStartCommand *pCommand)
{
    int ret = CELL_OK;
    ret = (*(pEnv->pFuncs->pInquireCapability))(pEnv->pArg,
                                                pStream->streamType,
                                                pStream->pMediaInfo,
                                                pStream->pUri,
                                                pCommand);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncInquireCapability() failed",
                           return ret);
    return CELL_OK;
}

static int
cellSailSourceCheck_start(CellSailSourceCheckEnv     *pEnv,
                          CellSailSourceStartCommand *pCommand,
                          uint32_t                    sessionId)
{
    int ret = CELL_OK;
    int type;
    int arg0 = 0;
    int arg1 = 0;
    /* start */
    (*(pEnv->pFuncs->pStart))(pEnv->pArg,
                              pCommand,
                              sessionId);

    /* read sync */
    CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv,
                                     &pEnv->pRes->ok,
                                     ret,
                                     return ret);

    cellSailSourceCheckQueueGet(&pEnv->queue,
                                &type,
                                &arg0,
                                &arg1,
                                /*timeout*/ 0);
    CELL_SAIL_CHECK(pEnv,
                    CELL_SAIL_SOURCE_DIAG_NOTIFY_CALL_COMPLETED == type &&
                    /*status*/ arg0 >= 0,
                    "CellSailSourceFuncStart() notify failed",
                    return -1);
    return CELL_OK;
}

static int
cellSailSourceCheck_stop(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    /* stop */
    (*(pEnv->pFuncs->pStop))(pEnv->pArg);

    /* read sync */
    CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv,
                                     &pEnv->pRes->ok,
                                     ret,
                                     return ret);
    return CELL_OK;
}

static int
cellSailSourceCheck_checkout(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    if (0 >= pEnv->runningContext.numStreamOut  ||
        CELL_SAIL_SOURCE_CHECK_MAX_ITEMS <= pEnv->runningContext.numCheckedOut) {
        return CELL_OK;
    }

    const CellSailSourceBufferItem **ppItem;
    ppItem = &pEnv->pItems[pEnv->runningContext.checkOutIndex];
    ret = (*(pEnv->pFuncs->pCheckout))(pEnv->pArg, ppItem);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncCheckout() failed",
                           return ret);
    pEnv->runningContext.checkOutIndex =
        (pEnv->runningContext.checkOutIndex + 1) % CELL_SAIL_SOURCE_CHECK_MAX_ITEMS;
    --pEnv->runningContext.numStreamOut;
    ++pEnv->runningContext.numCheckedOut;
    pEnv->runningContext.checkedOutSize += (*ppItem)->size;
    return CELL_OK;
}


static int
cellSailSourceCheck_checkin(CellSailSourceCheckEnv *pEnv)
{
    if (0 >= pEnv->runningContext.numCheckedOut) {
        return CELL_OK;
    }

    int ret = CELL_OK;
    const CellSailSourceBufferItem *pItem;
    pItem = pEnv->pItems[pEnv->runningContext.checkInIndex];
    ret = (*(pEnv->pFuncs->pCheckin))(pEnv->pArg, pItem);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncCheckout() failed",
                           return ret);
    pEnv->runningContext.checkInIndex =
        (pEnv->runningContext.checkInIndex + 1) % CELL_SAIL_SOURCE_CHECK_MAX_ITEMS;
    --pEnv->runningContext.numCheckedOut;
    pEnv->runningContext.checkedInSize += pItem->size;
    return CELL_OK;
}


static int
cellSailSourceCheck_clear(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    ret = (*(pEnv->pFuncs->pClear))(pEnv->pArg);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "CellSailSourceFuncClear() failed",
                           return ret);
    return CELL_OK;
}


static int
cellSailSourceCheck_running_stop(CellSailSourceCheckEnv *pEnv,
                                 bool                    stop,
                                 uint64_t                stopSize)
{
    int ret = CELL_OK;
    bool eos = false;
    for (;;) {
        int type;
        int arg0 = 0;
        int arg1 = 0;
        cellSailSourceCheckQueueGet(&pEnv->queue,
                                    &type,
                                    &arg0,
                                    &arg1,
                                    /*timeout usec*/ 4000);
        if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_50()) {
            ret = cellSailSourceCheck_checkout(pEnv);
            if (0 > ret) {
                return ret;
            }
        }

        if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_50()) {
            ret = cellSailSourceCheck_checkin(pEnv);
            if (0 > ret) {
                return ret;
            }
        }

        if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_6()) {
            CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv,
                                             &pEnv->pRes->ok,
                                             ret,
                                             return ret);
        }

        switch (type) {
        case CELL_SAIL_SOURCE_DIAG_NOTIFY_CALL_COMPLETED:
            CELL_SAIL_CHECK(pEnv,
                            stop,
                            "unexpected call completed",
                            return -1);
            CELL_SAIL_CHECK(pEnv,
                            /*status*/ arg0 >= 0,
                            "CellSailSourceFuncStop() notify failed",
                            return -1);
            CELL_SAIL_SOURCE_CHECK_PRINTF("* stop\n");
            sCellSailSourceCheckBarProgressing = 0;
            goto label_fin;
            break;

        case CELL_SAIL_SOURCE_DIAG_NOTIFY_INPUT_EOS:
            CELL_SAIL_SOURCE_CHECK_PRINTF("* eos\n");
            sCellSailSourceCheckBarProgressing = 0;
            CELL_SAIL_CHECK(pEnv,
                            !eos,
                            "eos has occured twice",
                            return -1);
            goto label_fin;

        case CELL_SAIL_SOURCE_DIAG_NOTIFY_STREAM_OUT:
            CELL_SAIL_CHECK(pEnv,
                            1 == arg0,
                            "arg \"num\" in notifyStreamOut must be 1",
                            return -1);
            ++pEnv->runningContext.numStreamOut;
            CELL_SAIL_SOURCE_CHECK_PRINTF("*");
            sCellSailSourceCheckBarProgressing = 1;
            if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_50()) {
                ret = cellSailSourceCheck_checkout(pEnv);
                if (0 > ret) {
                    return ret;
                }
            }

            if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_50()) {
                ret = cellSailSourceCheck_checkin(pEnv);
                if (0 > ret) {
                    return ret;
                }
            }

            break;

        case CELL_SAIL_SOURCE_DIAG_NOTIFY_SESSION_ERROR:
            break;
        case CELL_SAIL_SOURCE_DIAG_NOTIFY_MEDIA_STATE_CHANGED:
            CELL_SAIL_CHECK(pEnv,
                            (CELL_SAIL_MEDIA_STATE_FINE == arg1 ||
                             CELL_SAIL_MEDIA_STATE_BAD  == arg1 ||
                             CELL_SAIL_MEDIA_STATE_LOST == arg1),
                            "unknown media state",
                            return -1);
            break;
        default:
            ; // timeout
        }

        if (stop &&
            ((CELL_SAIL_SOURCE_CHECK_PROBABILITY_50() &&
              (pEnv->runningContext.checkedOutSize >= stopSize)) ||
             (pEnv->runningContext.checkedInSize >= stopSize))) {
            ret = cellSailSourceCheck_stop(pEnv);
            if (0 > ret) {
                return ret;
            }
        }
    }

 label_fin:

    if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_50()) {
        ret = cellSailSourceCheck_clear(pEnv);
        if (0 > ret) {
            return ret;
        }
    } else {

        if (0 < pEnv->runningContext.numCheckedOut) {
            ret = cellSailSourceCheck_checkin(pEnv);
            if (0 > ret) {
                return ret;
            }
        }

        while (0 < pEnv->runningContext.numStreamOut) {
            ret = cellSailSourceCheck_checkout(pEnv);
            if (0 > ret) {
                return ret;
            }
            ret = cellSailSourceCheck_checkin(pEnv);
            if (0 > ret) {
                return ret;
            }
        }

        while (0 < pEnv->runningContext.numCheckedOut) {
            ret = cellSailSourceCheck_checkin(pEnv);
            if (0 > ret) {
                return ret;
            }
        }

        if (! stop) {
            CELL_SAIL_CHECK(pEnv,
                            pEnv->runningContext.checkedOutSize == stopSize &&
                            pEnv->runningContext.checkedInSize  == stopSize,
                            "actual read size is different from start-command size",
                            return -1);
        }

        if (CELL_SAIL_SOURCE_CHECK_PROBABILITY_50()) {
            ret = cellSailSourceCheck_clear(pEnv);
            if (0 > ret) {
                return ret;
            }
        }
    }
    CELL_SAIL_SOURCE_CHECK_MEMSET(&pEnv->runningContext,
                                  0,
                                  sizeof(pEnv->runningContext));

    pEnv->queue.count = 0;
    pEnv->queue.head  = 0;
    pEnv->queue.tail  = 0;

    return CELL_OK;
}


static int
cellSailSourceCheck_open_database(CellSailSourceCheckEnv    *pEnv,
                                  CellSailSourceCheckStream *pStream)
{
    static const int default_header_size = 2048;
    int ret = CELL_OK;

    uint64_t  offset  = 0;
    uint32_t  size    = default_header_size;
    uint8_t  *pHeader;

    uint64_t totalSize;
    CellPamfReader *pReader = &pEnv->db.pamf.reader;
    for (;;) {
        pHeader = (uint8_t*)
                  CELL_SAIL_SOURCE_CHECK_MALLOC(pEnv->pArg, size);
        CELL_SAIL_CHECK(pEnv,
                        pHeader,
                        "malloc failed",
                        return -1);

        ret = (*(pEnv->pFuncs->pRead))(pEnv->pArg,
                                       pStream->streamType,
                                       pStream->pMediaInfo,
                                       pStream->pUri,
                                       offset,
                                       pHeader,
                                       size,
                                       &totalSize);
        CELL_SAIL_CHECK_RETVAL(pEnv,
                               ret,
                               "CellSailSourceFuncRead() failed",
                               goto label_fin);
        /* read sync */
        CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv,
                                         &pEnv->pRes->ok,
                                         ret,
                                         return ret);

        int type;
        int arg0 = 0;
        int arg1 = 0;
        cellSailSourceCheckQueueGet(&pEnv->queue,
                                    &type,
                                    &arg0,
                                    &arg1,
                                    /*timeout*/ 0);

        CELL_SAIL_CHECK(pEnv,
                        CELL_SAIL_SOURCE_DIAG_NOTIFY_CALL_COMPLETED == type &&
                        /*status*/ arg0 >= 0,
                        "CellSailSourceFuncRead() notify failed",
                        ret = -1;
                        goto label_fin);

        uint64_t headerSize;
        uint64_t streamSize;
        ret = cellPamfGetStreamOffsetAndSize(pHeader,
                                             totalSize,
                                             &headerSize,
                                             &streamSize);
        CELL_SAIL_CHECK_RETVAL(pEnv,
                               ret,
                               "cellPamfGetStreamOffsetAndSize() failed",
                               goto label_fin);

        if (headerSize <= size) {
            pEnv->db.pamf.headerSize = headerSize;
            pEnv->db.pamf.streamSize = streamSize;
            break;
        }
        size = headerSize;
        CELL_SAIL_SOURCE_CHECK_FREE(pEvn->pArg, pHeader);
    }

    ret = cellPamfReaderInitialize(pReader,
                                   pHeader,
                                   totalSize,
                                   CELL_PAMF_INITIALIZE_VERIFY_ON);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "cellPamfReaderInitialize() failed",
                           goto label_fin);
    pEnv->db.pamf.maxBitrate = cellPamfReaderGetMuxRateBound(pReader);
    pEnv->db.pamf.maxBitrate = 48000000;

    CellPamfTimeStamp start;
    ret = cellPamfReaderGetPresentationStartTime(pReader, &start);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "cellPamfReaderGetPresentationStartTime() failed",
                           goto label_fin);
    CellPamfTimeStamp end;
    ret = cellPamfReaderGetPresentationEndTime(pReader, &end);
    CELL_SAIL_CHECK_RETVAL(pEnv,
                           ret,
                           "cellPamfReaderGetPresentationEndTime() failed",
                           goto label_fin);
    pEnv->db.pamf.pHeader = pHeader;
    pEnv->db.pamf.duration = ((uint64_t)(end.upper - start.upper) << 32) |
                             (end.lower - start.lower);
    return CELL_OK;

 label_fin:
    CELL_SAIL_SOURCE_CHECK_FREE(pEnv->pArg, pHeader);
    return ret;
}

static int
cellSailSourceCheck_close_database(CellSailSourceCheckEnv *pEnv)
{
    CELL_SAIL_SOURCE_CHECK_FREE(pEnv->pArg, pEnv->db.pamf.pHeader);
    pEnv->db.pamf.pHeader = NULL;
    return CELL_OK;
}

static int
cellSailSourceCheck_seq0_initialize_finalize(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    for (unsigned i = 0; i < CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_0; ++i) {
        ret = cellSailSourceCheck_initialize(pEnv);
        if (0 > ret) {
            return ret;
        }

        ret = cellSailSourceCheck_finalize(pEnv);
        if (0 > ret) {
            return ret;
        }
    }
    return CELL_OK;
}


static int
cellSailSourceCheck_seq1_makeup_cleanup(CellSailSourceCheckEnv *pEnv)
{
    int ret = CELL_OK;
    ret = cellSailSourceCheck_initialize(pEnv);
    if (0 > ret) {
        return ret;
    }

    for (unsigned i = 0; i < CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_1; ++i) {
        /* makeup */
        ret = cellSailSourceCheck_makeup(pEnv);
        if (0 > ret) {
            goto label_fin;
        }

        /* read sync */
        CELL_SAIL_SOURCE_CHECK_READ_SYNC(pEnv,
                                         &pEnv->pRes->ok,
                                         ret,
                                         goto label_fin);

        /* cleanup */
        ret = cellSailSourceCheck_cleanup(pEnv);
        if (0 > ret) {
            goto label_fin;
        }
    }

 label_fin:
    {
        int ret0 = cellSailSourceCheck_finalize(pEnv);
        if (0 > ret0 && CELL_OK == ret) {
            ret = ret0;
        }
    }
    return ret;
}

static int
cellSailSourceCheck_seq2_open_close(CellSailSourceCheckEnv    *pEnv,
                                    CellSailSourceCheckStream *pStream)
{
    int ret = CELL_OK;
    ret = cellSailSourceCheck_initialize(pEnv);
    if (0 > ret) {
        return ret;
    }

    ret = cellSailSourceCheck_makeup(pEnv);
    if (0 > ret) {
        return ret;
    }

    ret = cellSailSourceCheck_open_database(pEnv,
                                            pStream);
    if (0 > ret) {
        goto label_fin0;
    }

    for (unsigned i = 0; i < CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_2; ++i) {

        uint64_t capabilities;
        ret = cellSailSourceCheck_getCapabilities(pEnv,
                                                  pStream,
                                                  &capabilities);
        if (0 > ret) {
            goto label_fin1;
        }

        ret = cellSailSourceCheck_open(pEnv, pStream);
        if (0 > ret) {
            goto label_fin1;
        }

        ret = cellSailSourceCheck_close(pEnv);
        if (0 > ret) {
            goto label_fin1;
        }
    }
    ret = CELL_OK;

 label_fin1:
    {
        int ret0 = cellSailSourceCheck_close_database(pEnv);
        if (0 > ret0 && CELL_OK == ret) {
            ret = ret0;
        }
    }

 label_fin0:
    {
        int ret0 = cellSailSourceCheck_cleanup(pEnv);
        if (0 > ret0 && CELL_OK == ret) {
            ret = ret0;
        }
    }
    {
        int ret0 = cellSailSourceCheck_finalize(pEnv);
        if (0 > ret0 && CELL_OK == ret) {
            ret = ret0;
        }
    }
    return ret;
}

static int
cellSailSourceCheck_start_running_stop(CellSailSourceCheckEnv     *pEnv,
                                       CellSailSourceCheckStream  *pStream,
                                       CellSailSourceStartCommand *pCommand,
                                       uint32_t                    sessionId,
                                       bool                        stop,
                                       uint64_t                    stopSize)
{
    int ret;

    CELL_SAIL_SOURCE_CHECK_P("running: startOffset %lld, readSize %lld\n",
                             pCommand->startArg,
                             pCommand->lengthArg);
    ret = cellSailSourceCheck_inquireCapability(pEnv,
                                                pStream,
                                                pCommand);
    if (0 > ret) {
        return ret;
    }

    ret = cellSailSourceCheck_start(pEnv,
                                    pCommand,
                                    sessionId);
    if (0 > ret) {
        return ret;
    }

    ret = cellSailSourceCheck_running_stop(pEnv, stop, stopSize);
    if (0 > ret) {
        return ret;
    }
    return CELL_OK;
}

static int
cellSailSourceCheck_start_stop_impl(CellSailSourceCheckEnv    *pEnv,
                                    CellSailSourceCheckStream *pStream,
                                    uint64_t                   capabilities,
                                    bool                       stop,
                                    float                      runningRatio)
{

    int ret;
    uint64_t headerSize;
    uint64_t streamSize;
    ret = cellPamfGetStreamOffsetAndSize(pEnv->db.pamf.pHeader,
                                         0,
                                         &headerSize,
                                         &streamSize);
    if (0 > ret) {
        return ret;
    }

    CellPamfReader *pReader = &pEnv->db.pamf.reader;

    ret = cellPamfReaderSetStreamWithTypeAndIndex(
        pReader,
        CELL_PAMF_STREAM_TYPE_VIDEO,
        /*video index*/ 0);
    if (0 > ret) {
        return ret;
    }

    uint32_t sessionId = 1;

    {
        CellSailSourceStartCommand command;
        CELL_SAIL_SOURCE_CHECK_MEMSET(&command, 0, sizeof(command));
        command.startFlags = CELL_SAIL_SOURCE_SEEK_ABSOLUTE_BYTE_POSITION;
        command.startArg   = headerSize;

        /* length = streamSize */
        command.lengthArg  = streamSize;
        ret = cellSailSourceCheck_start_running_stop(pEnv,
                                                     pStream,
                                                     &command,
                                                     sessionId++,
                                                     stop,
                                                     streamSize * runningRatio);
        if (0 > ret) {
            return ret;
        }

        /* length = 0; */
        command.lengthArg  = 0;
        ret = cellSailSourceCheck_start_running_stop(pEnv,
                                                     pStream,
                                                     &command,
                                                     sessionId++,
                                                     stop,
                                                     0);
        if (0 > ret) {
            return ret;
        }

        /* length = ~(uint64_t)0; */
        command.lengthArg  = ~(uint64_t)0;
        ret = cellSailSourceCheck_start_running_stop(pEnv,
                                                     pStream,
                                                     &command,
                                                     sessionId++,
                                                     stop,
                                                     streamSize * runningRatio);
        if (0 > ret) {
            return ret;
        }

    }

    uint32_t numEp = cellPamfReaderGetNumberOfEp(pReader);
    if ((0 >= numEp) ||
        (0 == (CELL_SAIL_SOURCE_CAPABILITY_SEEK_ABSOLUTE_BYTE_POSITION & capabilities))) {
        return CELL_OK;
    }
    uint32_t startEpIndex;
    if (numEp < 12) {
        startEpIndex = 0;
    } else {
        startEpIndex = numEp - 12;
    }

    CellPamfEpIterator startIt;
    ret = cellPamfReaderGetEpIteratorWithIndex(pReader,
                                               startEpIndex,
                                               &startIt);
    if (0 > ret) {
        return ret;
    }

    CellPamfEp startEp;
    ret = cellPamfEpIteratorGetEp(&startIt, &startEp);
    if (0 > ret) {
        return ret;
    }

    for (;;) {
        CellSailSourceStartCommand command;
        CELL_SAIL_SOURCE_CHECK_MEMSET(&command, 0, sizeof(command));
        command.startFlags = CELL_SAIL_SOURCE_SEEK_ABSOLUTE_BYTE_POSITION;
        command.startArg   = startEp.rpnOffset + headerSize;
        command.lengthArg  = ~(uint64_t)0;
        uint64_t stopPosition;
        stopPosition = (uint64_t)((streamSize - startEp.rpnOffset) * runningRatio);
        {
            /* length = streamSize - startEp.rpnOffset */
            command.lengthArg  = streamSize - startEp.rpnOffset;
            ret = cellSailSourceCheck_start_running_stop(pEnv,
                                                         pStream,
                                                         &command,
                                                         sessionId++,
                                                         stop,
                                                         stopPosition);
            if (0 > ret) {
                return ret;
            }

#if 0
            /* length = 0; */
            command.lengthArg  = 0;
            ret = cellSailSourceCheck_start_running_stop(pEnv,
                                                         pStream,
                                                         &command,
                                                         sessionId++,
                                                         stop,
                                                         stopPosition);
            if (0 > ret) {
                return ret;
            }

            /* length = ~(uint64_t)0; */
            command.lengthArg  = ~(uint64_t)0;
            ret = cellSailSourceCheck_start_running_stop(pEnv,
                                                         pStream,
                                                         &command,
                                                         sessionId++,
                                                         stop,
                                                         stopPosition);
            if (0 > ret) {
                return ret;
            }
#endif
        }

        CellPamfEpIterator endIt;
        ret = cellPamfReaderGetEpIteratorWithIndex(pReader,
                                                   startIt.index,
                                                   &endIt);
        if (0 > ret) {
            return ret;
        }

        CellPamfEp endEp;
        ret = cellPamfEpIteratorGetEp(&endIt, &endEp);
        if (0 > ret) {
            return ret;
        }

        for (;;) {

            command.lengthArg  = endEp.rpnOffset - startEp.rpnOffset;
            stopPosition = (uint64_t)(command.lengthArg * runningRatio);
            ret = cellSailSourceCheck_start_running_stop(pEnv,
                                                         pStream,
                                                         &command,
                                                         sessionId++,
                                                         stop,
                                                         stopPosition);

            if (0 == cellPamfEpIteratorMove(&endIt, 4, &endEp)) {
                break;
            }
        }

        if (0 == cellPamfEpIteratorMove(&startIt, 4, &startEp)) {
            break;
        }
    }

    return CELL_OK;
}


static int
cellSailSourceCheck_seq3_start_stop(CellSailSourceCheckEnv    *pEnv,
                                    CellSailSourceCheckStream *pStream)
{
    int ret;
    ret = cellSailSourceCheck_initialize(pEnv);
    if (0 > ret) {
        return ret;
    }

    ret = cellSailSourceCheck_makeup(pEnv);
    if (0 > ret) {
        return ret;
    }

    ret = cellSailSourceCheck_open_database(pEnv,
                                            pStream);
    if (0 > ret) {
        goto label_fin0;
    }

    uint64_t capabilities;
    ret = cellSailSourceCheck_getCapabilities(pEnv,
                                              pStream,
                                              &capabilities);
    if (0 > ret) {
        goto label_fin1;
    }

    ret = cellSailSourceCheck_open(pEnv, pStream);
    if (0 > ret) {
        goto label_fin1;
    }


#if 1
    CELL_SAIL_SOURCE_CHECK_P("sequence3-0 is starting up now\n");
    for (unsigned i = 0; i < CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_3_0; ++i) {
        ret = cellSailSourceCheck_start_stop_impl(pEnv,
                                                  pStream,
                                                  capabilities,
                                                  /*stop*/ false,
                                                  /*runningRatio*/ 1.0);
        if (0 > ret) {
            CELL_SAIL_SOURCE_CHECK_P("sequence3-0 was failed\n");
            goto label_fin1;
        }
    }
    CELL_SAIL_SOURCE_CHECK_P("sequence3-0 was finished successfully\n");
#endif

#if 1
    CELL_SAIL_SOURCE_CHECK_P("sequence3-1 is starting up now\n");
    for (unsigned i = 0; i < CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_3_1; ++i) {
        ret = cellSailSourceCheck_start_stop_impl(pEnv,
                                                  pStream,
                                                  capabilities,
                                                  /*stop*/ true,
                                                  /*runningRatio*/ 0.5);
        if (0 > ret) {
            CELL_SAIL_SOURCE_CHECK_P("sequence3-1 was failed\n");
            goto label_fin1;
        }
    }
    CELL_SAIL_SOURCE_CHECK_P("sequence3-1 was finished successfully\n");
#endif

#if 1
    CELL_SAIL_SOURCE_CHECK_P("sequence3-2 is starting up now\n");
    for (float r = 0.97; r < 1.0; r += 0.01) {
        for (unsigned i = 0; i < CELL_SAIL_SOURCE_CHECK_ITERATION_NUM_3_2; ++i) {
            ret = cellSailSourceCheck_start_stop_impl(pEnv,
                                                      pStream,
                                                      capabilities,
                                                      /*stop*/ true,
                                                      r);
            if (0 > ret) {
                CELL_SAIL_SOURCE_CHECK_P("sequence3-2 was failed\n");
                goto label_fin1;
            }
        }
    }
    CELL_SAIL_SOURCE_CHECK_P("sequence3-2 was finished successfully\n");
#endif

    ret = cellSailSourceCheck_close(pEnv);
    if (0 > ret) {
        ;
    }
    ret = CELL_OK;

 label_fin1:
    {
        int ret0 = cellSailSourceCheck_close_database(pEnv);
        if (0 > ret0 && CELL_OK == ret) {
            ret = ret0;
        }
    }

 label_fin0:
    {
        int ret0 = cellSailSourceCheck_cleanup(pEnv);
        if (0 > ret0 && CELL_OK == ret) {
            ret = ret0;
        }
    }

    {
        int ret0 = cellSailSourceCheck_finalize(pEnv);
        if (0 > ret0 && CELL_OK == ret) {
            ret = ret0;
        }
    }
    return ret;
}


static int
cellSailSourceCheck(CellSailSource              *pSource,
                    const CellSailSourceFuncs   *pFuncs,                    
                    void                        *pArg,
                    const char                  *pProtocolName,
                    CellSailSourceCheckResource *pRes,
                    CellSailSourceCheckFuncError pHandler,
                    void                        *pHandlerArg)
{
    CELL_SAIL_SOURCE_CHECK_P("* source_check 2.00 *\n");

    CellSailSourceCheckEnv *pEnv;
    pEnv = (CellSailSourceCheckEnv*)
           CELL_SAIL_SOURCE_CHECK_MALLOC(pArg, sizeof(CellSailSourceCheckEnv));
    if (NULL == pEnv) {
        if (pHandler) {
            (*pHandler)("malloc failed", __LINE__);
        } else {
            CELL_SAIL_CHECK_ERRORP("malloc failed!!!\n") ;
        }
        return -1;
    }

    pEnv->pSource       = pSource;
    pEnv->pFuncs        = pFuncs;
    pEnv->pArg          = pArg;
    pEnv->pProtocolName = pProtocolName;
    pEnv->pRes          = pRes;
    pEnv->pHandler      = pHandler;
    pEnv->pHandlerArg   = pHandlerArg;

    int ret;

    ret = cellSailSourceCheckQueueInitialize(&pEnv->queue);
    if (0 > ret) {
        CELL_SAIL_CHECK_ERRORP("initializing failed\n");
        goto label_fin0;
    }
    
    // test case 0
    CELL_SAIL_SOURCE_CHECK_P("sequence0 is starting up now\n");
    ret = cellSailSourceCheck_seq0_initialize_finalize(pEnv);
    if (0 > ret) {
        CELL_SAIL_SOURCE_CHECK_P("sequence0 was failed\n");
        goto label_fin1;
    }
    CELL_SAIL_SOURCE_CHECK_P("sequence0 was finished successfully\n");

    // test case 1
    CELL_SAIL_SOURCE_CHECK_P("sequence1 is starting up now\n");
    ret = cellSailSourceCheck_seq1_makeup_cleanup(pEnv);
    if (0 > ret) {
        CELL_SAIL_SOURCE_CHECK_P("sequence1 was failed\n");
        goto label_fin1;
    }
    CELL_SAIL_SOURCE_CHECK_P("sequence1 was finished successfully\n");

    // test case 2
    CELL_SAIL_SOURCE_CHECK_P("sequence2 is starting up now\n");
    ret = cellSailSourceCheck_seq2_open_close(pEnv,
                                              &pEnv->pRes->ok);
    if (0 > ret) {
        CELL_SAIL_SOURCE_CHECK_P("sequence3 was failed\n");
        goto label_fin1;
    }
    CELL_SAIL_SOURCE_CHECK_P("sequence2 was finished successfully\n");

    // test case 3
    CELL_SAIL_SOURCE_CHECK_P("sequence3 is starting up now\n");
    ret = cellSailSourceCheck_seq3_start_stop(pEnv,
                                              &pEnv->pRes->ok);
    if (0 > ret) {
        CELL_SAIL_SOURCE_CHECK_P("sequence3 was failed\n");
        goto label_fin1;
    }
    CELL_SAIL_SOURCE_CHECK_P("sequence3 was finished successfully\n");

    CELL_SAIL_SOURCE_CHECK_P("* source_check done *\n");
    ret = CELL_OK;

 label_fin1:
    (void)cellSailSourceCheckQueueFinalize(&pEnv->queue);

 label_fin0:
    CELL_SAIL_SOURCE_CHECK_FREE(pArg, pEnv);
    return ret;
}

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /*__CELL_SAIL_SOURCE_CHECK_H__*/
