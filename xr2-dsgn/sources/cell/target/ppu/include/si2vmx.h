/*
 * SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#ifndef __SI2VMX_H__
#define __SI2VMX_H__

/* SNC defined both __SNC__ and __GCC__. Need to detect __SNC__ first */
#if defined (__SNC__)
#include <si2vmx_snc.h>
#elif defined (__GNUC__)
#include <si2vmx_gcc.h>
#else
#error "Cannot find the implementation of si2vmx.h."
#endif

#endif /* __SI2VMX_H__ */

