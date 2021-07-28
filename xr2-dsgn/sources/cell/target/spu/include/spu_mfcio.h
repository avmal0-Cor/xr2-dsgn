/*
 * SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#ifndef __SPU_MFCIO_H__
#define __SPU_MFCIO_H__

/* SNC defined both __SNC__ and __GCC__. Need to detect __SNC__ first */
#if defined (__SNC__)
#include <spu_mfcio_snc.h>
#elif defined (__GNUC__)
#include <spu_mfcio_gcc.h>
#else
#error "Cannot find the implementation of spu_mfcio.h."
#endif

#endif /* __SPU_MFCIO_H__ */
