/* SCE CONFIDENTIAL
 PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */  

#ifndef __CELL_DMUX_H__
#define __CELL_DMUX_H__

#include <types.h>
#include <stdbool.h>
#include <stdint.h>
#include <cell/error.h>
#include <cell/spurs.h>
#include <cell/codec/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** error code **/

/*
	CELL_ERROR_FACILITY_CODEC				0x061
		libdmux		: 0x8061_0201 - 0x8061_02ff
*/

#define CELL_DMUX_ERROR_ARG       CELL_ERROR_CAST(0x80610201)
#define CELL_DMUX_ERROR_SEQ       CELL_ERROR_CAST(0x80610202)
#define CELL_DMUX_ERROR_BUSY      CELL_ERROR_CAST(0x80610203)
#define CELL_DMUX_ERROR_EMPTY     CELL_ERROR_CAST(0x80610204)
#define CELL_DMUX_ERROR_FATAL     CELL_ERROR_CAST(0x80610205)


/** enums **/

/**
 *
 */
typedef enum {
    CELL_DMUX_STREAM_TYPE_UNDEF = 0,
    CELL_DMUX_STREAM_TYPE_PAMF,
    CELL_DMUX_STREAM_TYPE_TERMINATOR,
} CellDmuxStreamType;


/**
 *
 */
typedef enum {
    CELL_DMUX_MSG_TYPE_DEMUX_DONE = 0,
    CELL_DMUX_MSG_TYPE_FATAL_ERR,
    CELL_DMUX_MSG_TYPE_PROG_END_CODE,
} CellDmuxMsgType;

typedef enum {
    CELL_DMUX_ES_MSG_TYPE_AU_FOUND = 0,
    CELL_DMUX_ES_MSG_TYPE_FLUSH_DONE,
} CellDmuxEsMsgType;


/**
 * opaque handle to Demuxer
 */
typedef void *CellDmuxHandle;

/**
 * opaque handle to ES
 */
typedef void *CellDmuxEsHandle;


/* callback message structs and function types */

/**
 * CellDmuxMsg
 * msgType
 *  - CELL_DMUX_MSG_TYPE_DEMUX_DONE
 *  - CELL_DMUX_MSG_TYPE_FATAL_ERR
 *  - CELL_DMUX_MSG_TYPE_PROG_END_CODE
 */
typedef struct {
    uint32_t msgType;
    uint64_t supplementalInfo;
} CellDmuxMsg;

/**
 * CellDmuxEsMsg
 * msgType
 * - CELL_DMUX_ES_MSG_TYPE_AU_FOUND
 * - CELL_DMUX_ES_MSG_TYPE_FLUSH_DONE
 */
typedef struct {
    uint32_t msgType;
    uint64_t supplementalInfo;
} CellDmuxEsMsg;

/**
 *
 */
typedef uint32_t (*CellDmuxCbMsg)(CellDmuxHandle     demuxerHandle,
                                  const CellDmuxMsg *demuxerMsg,
                                  void              *cbArg);

/**
 *
 */
typedef uint32_t (*CellDmuxCbEsMsg)(CellDmuxHandle       demuxerHandle,
                                    CellDmuxEsHandle     esHandle,
                                    const CellDmuxEsMsg *esMsg,
                                    void                *cbArg);


/** structs **/

/**
 *
 */
typedef struct {
    CellDmuxStreamType streamType; /**< demuxed stream type */
    int32_t            reserved1;  /**< reserved for future use. must be 0 */
    int32_t            reserved2;  /**< reserved for future use. must be 0 */
} CellDmuxType;

typedef struct {
    CellDmuxStreamType  streamType;
    void               *streamSpecificInfo;
} CellDmuxType2;

/**
 *
 */
typedef struct {
    void    *memAddr;
    size_t   memSize;
    uint32_t ppuThreadPriority;
    size_t   ppuThreadStackSize;
    uint32_t spuThreadPriority;
    uint32_t numOfSpus;
} CellDmuxResource;

typedef struct {
    void      *memAddr;
    size_t     memSize;
    uint32_t   ppuThreadPriority;
    size_t     ppuThreadStackSize;
    CellSpurs *spurs;
    uint8_t    priority[8];
    uint32_t   maxContention;
} CellDmuxResourceEx;

typedef struct {
    bool       isResourceEx;
    union {
        CellDmuxResource   resource;
        CellDmuxResourceEx resourceEx;
    } res;
} CellDmuxResource2;

/**
 *
 */
typedef struct {
    CellDmuxCbMsg  cbFunc;
    void          *cbArg;
} CellDmuxCb;

/**
 *
 */
typedef struct {
    CellDmuxCbEsMsg  cbFunc;
    void            *cbArg;
} CellDmuxEsCb;

/**
 *
 */
typedef struct {
    size_t   memSize;
    uint32_t demuxerVerUpper;
	uint32_t demuxerVerLower;
} CellDmuxAttr;

/**
 *
 */
typedef struct {
    size_t memSize;
} CellDmuxEsAttr;

/**
 * for API compatibility
 */
typedef CellCodecEsFilterId CellDmuxEsFilterId;

/**
 * memory resource for each ES
 */
typedef struct {
    void  *memAddr;
    size_t memSize;
} CellDmuxEsResource;

/**
 * AU(access unit) information
 */
typedef struct {
    void    *auAddr;
    size_t   auSize;
    size_t   auMaxSize;
    uint64_t userData;
    uint32_t ptsUpper;
    uint32_t ptsLower;
    uint32_t dtsUpper;
    uint32_t dtsLower;
} CellDmuxAuInfo;

/**
 * AU(access unit) information (extended version)
 */
typedef struct {
    void               *auAddr;
    size_t              auSize;
    uint32_t            reserved;
    bool                isRap;
    uint64_t            userData;
    CellCodecTimeStamp  pts;
    CellCodecTimeStamp  dts;
} CellDmuxAuInfoEx;


/** functions **/

/**
 *
 */
int32_t
cellDmuxQueryAttr(const CellDmuxType *demuxerType, 
                  CellDmuxAttr       *demuxerAttr);

int32_t
cellDmuxQueryAttr2(const CellDmuxType2 *demuxerType2,
                   CellDmuxAttr        *demuxerAttr);

/**
 *
 */
int32_t
cellDmuxOpen(const CellDmuxType     *demuxerType,
             const CellDmuxResource *demuxerResource,
             const CellDmuxCb       *demuxerCb,
             CellDmuxHandle         *demuxerHandle);

int32_t
cellDmuxOpenEx(const CellDmuxType       *demuxerType,
               const CellDmuxResourceEx *demuxerResourceEx,
               const CellDmuxCb         *demuxerCb,
               CellDmuxHandle           *demuxerHandle);

int32_t
cellDmuxOpen2(const CellDmuxType2     *demuxerType2,
              const CellDmuxResource2 *demuxerResource2,
              const CellDmuxCb        *demuxerCb,
              CellDmuxHandle          *demuxerHandle);

/**
 *
 */
int32_t
cellDmuxClose(CellDmuxHandle demuxerHandle);

/**
 *
 */
int32_t
cellDmuxSetStream(CellDmuxHandle demuxerHandle,
                  const void    *streamAddress,
                  size_t         streamSize,
                  bool           discontinuity,
                  uint64_t       userData);

/**
 *
 */
int32_t
cellDmuxResetStream(CellDmuxHandle demuxerHandle);

int32_t
cellDmuxResetStreamAndWaitDone(CellDmuxHandle demuxerHandle);

/**
 *
 */
int32_t
cellDmuxQueryEsAttr(const CellDmuxType        *demuxerType,
                    const CellCodecEsFilterId *esFilterId,
                    const void                *esSpecificInfo,
                    CellDmuxEsAttr            *esAttr);

int32_t
cellDmuxQueryEsAttr2(const CellDmuxType2       *demuxerType2,
                     const CellCodecEsFilterId *esFilterId,
                     const void                *esSpecificInfo,
                     CellDmuxEsAttr            *esAttr);

/**
 *
 */
int32_t
cellDmuxEnableEs(CellDmuxHandle             demuxerHandle,
                 const CellCodecEsFilterId *esFilterId,
                 const CellDmuxEsResource  *esResource,
                 const CellDmuxEsCb        *esCb,
                 const void                *esSpecificInfo,
                 CellDmuxEsHandle          *esHandle);

/**
 *
 */
int32_t
cellDmuxDisableEs(CellDmuxEsHandle esHandle);

/**
 *
 */
int32_t
cellDmuxGetAu(CellDmuxEsHandle       esHandle,
              const CellDmuxAuInfo **auInfo,
              void                 **auSpecificInfo);

int32_t
cellDmuxPeekAu(CellDmuxEsHandle       esHandle,
               const CellDmuxAuInfo **auInfo,
               void                 **auSpecificInfo);

/**
 *
 */
int32_t
cellDmuxGetAuEx(CellDmuxEsHandle         esHandle,
                const CellDmuxAuInfoEx **auInfoEx,
                void                   **auSpecificInfo);

int32_t
cellDmuxPeekAuEx(CellDmuxEsHandle         esHandle,
                 const CellDmuxAuInfoEx **auInfoEx,
                 void                   **auSpecificInfo);

/**
 *
 */
int32_t
cellDmuxReleaseAu(CellDmuxEsHandle esHandle);

/**
 *
 */
int32_t
cellDmuxFlushEs(CellDmuxEsHandle esHandle);

/**
 *
 */
int32_t
cellDmuxResetEs(CellDmuxEsHandle esHandle);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__CELL_DMUX_H__*/
/*[eof]*/
