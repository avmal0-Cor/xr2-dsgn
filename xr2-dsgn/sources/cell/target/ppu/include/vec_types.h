/*
 * SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2008 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */
#ifndef __VEC_TYPES_H__
#define __VEC_TYPES_H__

/* SNC defined both __SNC__ and __GCC__. Need to detect __SNC__ first */
#if defined (__SNC__)
#include <vec_types_snc.h>
#elif defined (__GNUC__)
#include <vec_types_gcc.h>
#else
#error "Cannot find the implementation of vec_types.h."
#endif

#endif /* __VEC_TYPES_H__ */
