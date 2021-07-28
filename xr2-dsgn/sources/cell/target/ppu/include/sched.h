/*
 *   SCE CONFIDENTIAL
 *   PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *   Copyright (C) 2005 Sony Computer Entertainment Inc.
 *   All Rights Reserved.
 */

/* Copyright 2005  Sony Corporation */

#ifndef _SCHED_H_
#define _SCHED_H_

#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h>

_C_STD_BEGIN

#define SCHED_OTHER 1
#define SCHED_FIFO  2
#define SCHED_RR    3

struct sched_param {
  int sched_priority ;
} ;

_C_LIB_DECL

_END_C_LIB_DECL

_C_STD_END

#if defined(_STD_USING)
using _CSTD sched_param ;
#endif /* defined(_STD_USING) */

#endif /* _SCHED_H_ */
