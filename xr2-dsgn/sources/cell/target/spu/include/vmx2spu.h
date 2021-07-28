/*
 * SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#ifndef __VMX2SPU_H__
#define __VMX2SPU_H__

/* SNC defined both __SNC__ and __GCC__. Need to detect __SNC__ first */
#if defined (__SNC__)
#include <vmx2spu_snc.h>
#elif defined (__GNUC__)
#include <vmx2spu_gcc.h>
#else
#error "Cannot find the implementation of vmx2spu.h."
#endif

#endif /* __VMX2SPU_H__ */
