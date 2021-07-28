/*
 *   SCE CONFIDENTIAL
 *   PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *   Copyright (C) 2007 Sony Computer Entertainment Inc.
 *   All Rights Reserved.
 */

/* Copyright 2005  Sony Corporation */

#ifndef _PTHREAD_TYPES_H_
#define _PTHREAD_TYPES_H_
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/synchronization.h>

_C_STD_BEGIN

struct pthread ;
typedef struct	pthread	*pthread_t;

typedef struct s_pthread_attr_t 
{
  int    schedpolicy ;
  int    sched_priority ;
  int    detachstate ;
  int    scope ;
  int    inheritsched ;
  void*  stackaddr;
  size_t stacksize;
  char*  name ;
} pthread_attr_t ;

typedef struct s_pthread_mutexattr_t 
{
  int   protocol ;
  int   pshared ;
  int   type ; 
  int   prioceiling ;
} pthread_mutexattr_t ;

typedef struct s_pthread_mutex_t 
{
  pthread_mutexattr_t attr ;
  int type ;
  int status ;
  sys_mutex_t sys_mutex ;
} pthread_mutex_t ;

typedef struct s_pthread_condattr_t 
{
  long int rsvd0 ;
  int  pshared ;
} pthread_condattr_t ;

typedef struct s_pthread_cond_t 
{
  pthread_condattr_t attr ;
  int status ;
  sys_cond_t  sys_cond ;
  sys_mutex_t sys_mutex ;
} pthread_cond_t ;

typedef int pthread_key_t;
typedef int pthread_once_t ;

typedef struct s_pthread_rwlockattr_t 
{
  int type ;
  int pshared;
} pthread_rwlockattr_t;

typedef struct s_pthread_rwlock_t 
{
  pthread_rwlockattr_t attr ;
  int     type ;
  pthread_mutex_t mutex;
  pthread_cond_t read_cond;
  pthread_cond_t write_cond;
  int blocked_writer;
  signed int state;
} pthread_rwlock_t ;

struct timespec
{	/* POSIX.4, needed for pthread_cond_timedwait */
  long tv_sec;
  long tv_nsec;
};

_C_LIB_DECL

_END_C_LIB_DECL

_C_STD_END
#if defined(_STD_USING)
using _CSTD pthread_t ;
using _CSTD pthread_attr_t ;
using _CSTD pthread_mutex_t ;
using _CSTD pthread_mutexattr_t ;
using _CSTD pthread_condattr_t ;
using _CSTD pthread_cond_t ;
using _CSTD pthread_key_t;
using _CSTD pthread_once_t ;
using _CSTD pthread_rwlockattr_t;
using _CSTD  pthread_rwlock_t ;
using _CSTD timespec;
#endif /* defined(_STD_USING) */

#endif /* */
