/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SYS_SYS_SPU_STOP_AND_SIGNAL_H__
#define __SYS_SYS_SPU_STOP_AND_SIGNAL_H__

/* stop and signal type */
#define STOP_TYPE_MASK                0x3f00
#define STOP_TYPE_TERMINATE           0x0000
#define STOP_TYPE_MISC                0x0100
#define STOP_TYPE_SHARED_MUTEX        0x0200
#define STOP_TYPE_STOP_CALL           0x0400
#define STOP_TYPE_SYSTEM              0x1000
#define STOP_TYPE_RESERVE             0x2000

/* value */
#define STOP_VALUE_MASK               0x00ff

/* misc */

#define STOP_YIELD                    0x0100

/* misc for SDK 0.4.0 */
#define SYS_SPU_THREAD_STOP_YIELD                 0x0100
#define SYS_SPU_THREAD_STOP_GROUP_EXIT            0x0101
#define SYS_SPU_THREAD_STOP_THREAD_EXIT           0x0102
#define SYS_SPU_THREAD_STOP_RECEIVE_EVENT         0x0110
#define SYS_SPU_THREAD_STOP_TRY_RECEIVE_EVENT     0x0111
#define SYS_SPU_THREAD_STOP_SWITCH_SYSTEM_MODULE  0x0120

/* shared mutex */
#define STOP_SHARED_MUTEX_ID_MASK     0x001f
#define STOP_SHARED_MUTEX_TYPE_MASK   0x0060
#define STOP_SHARED_MUTEX_LOCK        0x0000
#define STOP_SHARED_MUTEX_UNLOCK      0x0020
#define STOP_SHARED_MUTEX_TRYLOCK     0x0040

/* skprintf */
#define STOP_SKPRINTF                 0x1112

/* break */
#define STOP_BREAK                    0x3fff

/* exit */
#define STOP_STDLIB_EXIT              0x2000

#endif /* __SYS_SYS_SPU_STOP_AND_SIGNAL_H__ */
