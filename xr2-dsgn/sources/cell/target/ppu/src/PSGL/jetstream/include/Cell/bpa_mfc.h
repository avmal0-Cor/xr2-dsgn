/* --------------------------------------------------------------  */
/* PLEASE DO NOT MODIFY THIS SECTION                               */
/* This prolog section is automatically generated.                 */
/*                                                                 */
/* (C)Copyright                                                    */
/* Sony Computer Entertainment, Inc.,                              */
/* Toshiba Corporation,                                            */
/* International Business Machines Corporation,                    */
/* 2001,2003. All rights reserved.                                 */
/* S/T/I Confidential Information                                  */
/* --------------------------------------------------------------  */
/* PROLOG END TAG zYx                                              */
#ifndef _BPA_MFC_H_
#define _BPA_MFC_H_

/* This header file contains various definitions related to the Memory Flow
   Controller (MFC) portion of the Broadband Processor Architecture (BPA).

   The structures defining the mapping of the MFC from the PU side and
   various other registers in the BPA are located in bpa_map.h.
 */

/**************************************/
/* MFC DMA Command Opcode Definitions */
/**************************************/

/****************************************************************************/
/* MFC DMA Command flags which identify classes of operations. */
/****************************************************************************/
/* Note: These flags may be used in conjunction with the base command types
     (i.e. MFC_PUT_CMD, MFC_PUTR_CMD, MFC_GET_CMD, and MFC_SNDSIG_CMD)
     to construct the various command permutations.
 */

#define MFC_BARRIER_ENABLE      0x01
#define MFC_FENCE_ENABLE        0x02
#define MFC_LIST_ENABLE         0x04  /* SPU Only */
#define MFC_START_ENABLE        0x08  /*  PU Only */
#define MFC_RESULT_ENABLE       0x10

/****************************************************************************/
/* MFC DMA Put Commands */
/****************************************************************************/

#define MFC_PUT_CMD             0x20
#define MFC_PUTS_CMD            0x28  /*  PU Only */
#define MFC_PUTR_CMD            0x30
#define MFC_PUTF_CMD            0x22
#define MFC_PUTB_CMD            0x21
#define MFC_PUTFS_CMD           0x2A  /*  PU Only */
#define MFC_PUTBS_CMD           0x29  /*  PU Only */
#define MFC_PUTRF_CMD           0x32
#define MFC_PUTRB_CMD           0x31
#define MFC_PUTL_CMD            0x24  /* SPU Only */
#define MFC_PUTRL_CMD           0x34  /* SPU Only */
#define MFC_PUTLF_CMD           0x26  /* SPU Only */
#define MFC_PUTLB_CMD           0x25  /* SPU Only */
#define MFC_PUTRLF_CMD          0x36  /* SPU Only */
#define MFC_PUTRLB_CMD          0x35  /* SPU Only */

/****************************************************************************/
/* MFC DMA Get Commands */
/****************************************************************************/

#define MFC_GET_CMD             0x40
#define MFC_GETS_CMD            0x48  /*  PU Only */
#define MFC_GETF_CMD            0x42
#define MFC_GETB_CMD            0x41
#define MFC_GETFS_CMD           0x4A  /*  PU Only */
#define MFC_GETBS_CMD           0x49  /*  PU Only */
#define MFC_GETL_CMD            0x44  /* SPU Only */
#define MFC_GETLF_CMD           0x46  /* SPU Only */
#define MFC_GETLB_CMD           0x45  /* SPU Only */

/****************************************************************************/
/* MFC DMA Storage Control Commands */
/****************************************************************************/
/* Note: These are only supported on implementations with a SL1 cache
     They are no-ops on the initial BE implementation.
 */

#define MFC_SDCRT_CMD           0x80
#define MFC_SDCRTST_CMD         0x81
#define MFC_SDCRZ_CMD           0x89
#define MFC_SDCRS_CMD           0x8D
#define MFC_SDCRF_CMD           0x8F

/****************************************************************************/
/* MFC Synchronization Commands */
/****************************************************************************/

#define MFC_GETLLAR_CMD         0xD0  /* SPU Only */
#define MFC_PUTLLC_CMD          0xB4  /* SPU Only */
#define MFC_PUTLLUC_CMD         0xB0  /* SPU Only */
#define MFC_SNDSIG_CMD          0xA0
#define MFC_SNDSIGB_CMD         0xA1
#define MFC_SNDSIGF_CMD         0xA2
#define MFC_FENCE_CMD           0xC0
#define MFC_EIEIO_CMD           0xC8
#define MFC_SYNC_CMD            0xCC

/********************************************/
/* MFC PU Side Register Utility Definitions */
/********************************************/

/****************************************************************************/
/* Definitions related to the PU DMA Command registers.
   (DMA_LSA, DMA_EA, DMA_Size, DMA_Tag, DMA_BWclassID, DMA_RclassID, DMA_CMD)
 */
/****************************************************************************/

/****************************************************************************/
/* Definitions for constructing a 32-bit command word including the bandwidth
   and reservation class id and the command opcode.
 */
/****************************************************************************/
#define MFC_BCLASS(_bid)       ((_bid) << 24)
#define MFC_RCLASS(_rid)       ((_rid) << 16)

#define MFC_CMD_WORD(_bid, _rid, _cmd) \
 (MFC_BCLASS(_bid) | MFC_RCLASS(_rid) | (_cmd))

/****************************************************************************/
/* Definitions for constructing a 64-bit command word including the size, tag,
   bandwidth and reservation class id and the command opcode.
 */
/****************************************************************************/
#define MFC_SIZE(_size)        ((unsigned long long)(_size) << 48)
#define MFC_TAG(_tag_id)       ((unsigned long long)(_tag_id) << 32)
#define MFC_BR_CMD(_brcmd)     ((unsigned long long)(_brcmd))

#define MFC_CMD_DWORD(_size, _tag_id, _brcmd)  \
 (MFC_SIZE(_size) | MFC_TAG(_tag_id) | MFC_BR_CMD(_brcmd))

/****************************************************************************/
/* Mask definitions for obtaining DMA commands and class ids from packed words.
 */
/****************************************************************************/
#define MFC_CMD_MASK       0x0000FFFF
#define MFC_CLASS_MASK     0x000000FF

/****************************************************************************/
/* DMA max/min size definitions. */
/****************************************************************************/
#define MFC_MIN_DMA_SIZE_SHIFT  4   /* 16 bytes */
#define MFC_MAX_DMA_SIZE_SHIFT  14  /* 16384 bytes */

#define MFC_MIN_DMA_SIZE_MASK   (MFC_MIN_DMA_SIZE - 1)
#define MFC_MAX_DMA_SIZE_MASK   (MFC_MAX_DMA_SIZE - 1)

#define MFC_MIN_DMA_SIZE    (1 << MFC_MIN_DMA_SIZE_SHIFT)
#define MFC_MAX_DMA_SIZE    (1 << MFC_MAX_DMA_SIZE_SHIFT)

#define MFC_MIN_DMA_LIST_SIZE   0x0008  /*   8 bytes */
#define MFC_MAX_DMA_LIST_SIZE   0x4000  /* 16K bytes */

/****************************************************************************/
/* Mask definition for checking proper address alignment. */
/****************************************************************************/
#define MFC_ADDR_MATCH_MASK     0xF

/****************************************************************************/
/* Definitions related to the PU DMA Command Status register (DMA_CMDStatus).
 */
/****************************************************************************/
#define MFC_PU_DMA_CMD_ENQUEUE_SUCCESSFUL     0x00
#define MFC_PU_DMA_CMD_SEQUENCE_ERROR         0x01
#define MFC_PU_DMA_QUEUE_FULL                 0x02

/****************************************************************************/
/* Definitions related to the DMA Queue Status register (DMA_QStatus). */
/****************************************************************************/
#define MFC_PU_MAX_QUEUE_SPACE      0x08
#define MFC_PU_DMA_Q_EMPTY      0x80000000
#define MFC_PU_DMA_Q_FREE_SPACE_MASK    0x0000FFFF

// #define MFC_SPU_MAX_QUEUE_SPACE  0x10
#define MFC_SPU_MAX_QUEUE_SPACE     0xF

/****************************************************************************/
/* Definitions related to the PU DMA Query Type register (DMA_QueryType). */
/****************************************************************************/
#define MFC_PU_DMA_QUERYTYPE_ANY    0x1
#define MFC_PU_DMA_QUERYTYPE_ALL    0x2

/****************************************************************************/
/* Definitions related to the PU Tag Group Query Mask (DMA_QueryMask) and
   PU Tag Status (DMA_TagStatus) registers.

   NOTE: The only use the bottom 5 bits of the tag id value passed to insure
         a valid tag id is used.
 */
/****************************************************************************/

#define MFC_TAGID_TO_TAGMASK(tag_id)  (1 << (tag_id & 0x1F))

/****************************************************************************/
/* Definitions related to the Mailbox Status register (MB_stat). */
/****************************************************************************/
#define MFC_SPU_TO_PU_MAILBOX_STATUS_MASK       0x000000FF
#define MFC_SPU_TO_PU_MAILBOX_STATUS_SHIFT      0x0
#define MFC_PU_TO_SPU_MAILBOX_STATUS_MASK       0x0000FF00
#define MFC_PU_TO_SPU_MAILBOX_STATUS_SHIFT      0x8
#define MFC_PU_TO_SPU_MAILBOX_MAX           0x4
#define MFC_SPU_TO_PU_INT_MAILBOX_STATUS_MASK       0x00FF0000
#define MFC_SPU_TO_PU_INT_MAILBOX_STATUS_SHIFT      0x10

/****************************************************************************
* MFC Channels
****************************************************************************/
/*******************************************
* SPU Event and Signalling Channels (0 - 4)
*******************************************/

// Events for Channels 0-2
#define MFC_DMA_TAG_STATUS_UPDATE_EVENT 0x00000001
#define MFC_DMA_TAG_CMD_STALL_NOTIFY_EVENT  0x00000002
#define MFC_DMA_QUEUE_VACANCY_EVENT     0x00000008
#define MFC_SPU_MAILBOX_WRITTEN_EVENT   0x00000010
#define MFC_DECREMENTER_EVENT       0x00000020
#define MFC_PU_INT_MAILBOX_AVAIL_EVENT  0x00000040
#define MFC_PU_MAILBOX_AVAIL_EVENT      0x00000080
#define MFC_SIGNAL_2_EVENT          0x00000100
#define MFC_SIGNAL_1_EVENT          0x00000200
#define MFC_LLR_LOST_EVENT          0x00000400
#define MFC_PRIV_ATTN_EVENT         0x00000800

// Channel 0
#define MFC_RD_EVENT_STATUS     0
#define MFC_EVENT_NOT_TRIGGERED     0x0
#define MFC_EVENT_TRIGGERED     0x1

// Channel 1
#define MFC_WR_EVENT_MASK       1
#define MFC_EVENT_MASKED        0x0
#define MFC_EVENT_ENABLED       0x1

// Channel 2
#define MFC_WR_EVENT_ACK        2
#define MFC_EVENT_NO_ACK        0x0
#define MFC_EVENT_ACK           0x1

// Channels 3-4
#define MFC_RD_SIGNAL_1         3
#define MFC_RD_SIGNAL_2         4

/**********************************
* SPU Decrementer Channels (7 - 8)
**********************************/
#define MFC_WR_DECR_COUNT       7
#define MFC_RD_DECR_COUNT       8

/*********************************
* DMA Command Channels (16 - 21)
*********************************/
#define MFC_LS_ADDR         16
#define MFC_EA_HI           17
#define MFC_EA_LOW          18
#define MFC_DMA_SIZE            19
#define MFC_TAG_ID          20
#define MFC_CMD_QUEUE           21

/***********************************
* DMA TAG Status Channels (22 - 27)
***********************************/
#define MFC_WR_TAG_MASK         22
#define MFC_WR_TAG_UPDATE       23
#define MFC_TAG_UPDATE_IMMED        0x0
#define MFC_TAG_UPDATE_ANY      0x1
#define MFC_TAG_UPDATE_ALL      0x2
#define MFC_RD_TAG_STATUS       24
#define MFC_RD_DMAL_STATUS      25
#define MFC_WR_DMAL_ACK         26
#define MFC_RD_LLAR_STATUS      27
#define MFC_PUTLLC_STATUS            0x00000001
#define MFC_PUTLLC_FAILED       0x1
#define MFC_PUTLLC_SUCCESS      0x0
#define MFC_PUTLLUC_STATUS           0x00000002
#define MFC_GETLLAR_STATUS           0x00000004

/******************************************
* PU/SPU Communication Mailboxes (28 - 30)
******************************************/
#define MFC_WR_MAILBOX          28
#define MFC_RD_MAILBOX          29
#define MFC_WR_INTR_MAILBOX     30

#endif /* #ifndef _BPA_MFC_H_ */
