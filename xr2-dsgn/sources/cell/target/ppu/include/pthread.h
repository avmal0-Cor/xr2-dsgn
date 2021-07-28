/*
 *   SCE CONFIDENTIAL
 *   PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *   Copyright (C) 2007 Sony Computer Entertainment Inc.
 *   All Rights Reserved.
 */

/* Copyright 2005  Sony Corporation */

#ifndef _PTHREAD_H_
#define _PTHREAD_H_

#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <sys/types.h>
#include <time.h>
#include <sched.h>

#include "pthread_types.h"

_C_STD_BEGIN


#define PTHREAD_INHERIT_SCHED       0
#define PTHREAD_EXPLICIT_SCHED      1

#define PTHREAD_CREATE_JOINABLE     0
#define PTHREAD_CREATE_DETACHED     1

#define PTHREAD_SCOPE_SYSTEM        0
#define PTHREAD_SCOPE_PROCESS       1

#define PTHREAD_PROCESS_PRIVATE     0
#define PTHREAD_PROCESS_SHARED      1	

/*
 * Flags for read/write lock attributes
 */
#define PTHREAD_PROCESS_PRIVATE     0
#define PTHREAD_PROCESS_SHARED      1	

/*
 * Flags for once initialization.
 */
#define PTHREAD_NEEDS_INIT  0
#define PTHREAD_DONE_INIT   1

/*
 * Static once initialization values. 
 */
#define PTHREAD_ONCE_INIT  PTHREAD_NEEDS_INIT


#define PTHREAD_PRIO_NONE	0
#define PTHREAD_PRIO_INHERIT	1
#define PTHREAD_PRIO_PROTECT	2

/*
 * Mutex types (Single UNIX Specification, Version 2, 1997).
 */

#define PTHREAD_MUTEX_ERRORCHECK  1
#define PTHREAD_MUTEX_RECURSIVE   2
#define PTHREAD_MUTEX_NORMAL      3
#define PTHREAD_MUTEX_DEFAULT     PTHREAD_MUTEX_NORMAL
#define MUTEX_TYPE_MAX    (PTHREAD_MUTEX_NORMAL+1)

#define PTHREAD_MUTEX_ADAPTIVE_NP   0
#define PTHREAD_MUTEX_RECURSIVE_NP  PTHREAD_MUTEX_RECURSIVE
#define PTHREAD_MUTEX_ERRORCHECK_NP PTHREAD_MUTEX_ERRORCHECK
#define PTHREAD_MUTEX_TIMED_NP      PTHREAD_MUTEX_NORMAL

/*
 * Static initialization values. 
 */
#define X_PTHREAD_MUTEXATTR_INITIALIZER \
  {  PTHREAD_PRIO_NONE ,     /* protocol */ \
     PTHREAD_PROCESS_PRIVATE,/* pshared  */ \
    PTHREAD_MUTEX_NORMAL,   /* type */ \
    0,                      /* prioceiling */ \
  } 

#define X_PTHREAD_CONDATTR_INITIALIZER \
  { 0,                      /* rsvd0 */ \
    PTHREAD_PROCESS_PRIVATE  /* pshared  */ \
  }

#define X_PTHREAD_RWLOCKATTR_INITIALIZER \
  { 0,                      /* type */ \
    PTHREAD_PROCESS_PRIVATE  /* pshared  */ \
  }

#define PTHREAD_MUTEX_INITIALIZER	\
    { X_PTHREAD_MUTEXATTR_INITIALIZER , \
      PTHREAD_MUTEX_NORMAL,0,0 }

#define PTHREAD_COND_INITIALIZER	\
    { X_PTHREAD_CONDATTR_INITIALIZER ,0,0,0 }

#define PTHREAD_RWLOCK_INITIALIZER	\
    { X_PTHREAD_RWLOCKATTR_INITIALIZER ,0, \
      PTHREAD_MUTEX_INITIALIZER,\
      PTHREAD_COND_INITIALIZER,\
      PTHREAD_COND_INITIALIZER,\
      0,0}

/* This should be in limits.h according to the POSIX spec.. */

/* maximum number of data keys that can be created by a process */
#undef  PTHREAD_KEYS_MAX
#define PTHREAD_KEYS_MAX                        (1024)

/* minimum size in bytes of thread stack storage */
#undef  PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN                       (4096)

/* maximum number of threads that can be created per process */
#undef  PTHREAD_THREADS_MAX
#define PTHREAD_THREADS_MAX                     (16384)



/*
 * Thread function prototype definitions:
 */

_C_LIB_DECL
int pthread_attr_destroy(pthread_attr_t *);
int pthread_attr_getdetachstate(const pthread_attr_t *, int *);
int pthread_attr_getinheritsched(const pthread_attr_t *, int *);
int pthread_attr_getschedparam(const pthread_attr_t *,struct sched_param *);
int pthread_attr_getschedpolicy(const pthread_attr_t *, int *);
int pthread_attr_getscope(const pthread_attr_t *, int *);
int pthread_attr_getstackaddr(const pthread_attr_t *, void **);
int pthread_attr_getstacksize(const pthread_attr_t *, size_t *);
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_attr_setinheritsched(pthread_attr_t *, int);
int pthread_attr_setschedparam(pthread_attr_t *,struct sched_param *);
int pthread_attr_setschedpolicy(pthread_attr_t *, int);
int pthread_attr_setscope(pthread_attr_t *, int);
int pthread_attr_setstackaddr(pthread_attr_t *, void *);
int pthread_attr_setstacksize(pthread_attr_t *, size_t);

int pthread_cond_broadcast(pthread_cond_t *);
int pthread_cond_destroy(pthread_cond_t *);
int pthread_cond_init(pthread_cond_t *,const pthread_condattr_t *) ;
int pthread_cond_signal(pthread_cond_t *);
int pthread_cond_timedwait(pthread_cond_t *,pthread_mutex_t *,const struct timespec *);
int pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
int pthread_condattr_init(pthread_condattr_t *);
int pthread_condattr_destroy(pthread_condattr_t *);
int pthread_condattr_setpshared(pthread_condattr_t *, int);
int pthread_condattr_getpshared(const pthread_condattr_t *, int *);

int pthread_create(pthread_t *, const pthread_attr_t *,void *(*) (void *),void *);
int pthread_detach(pthread_t);
int pthread_equal(pthread_t, pthread_t);
void pthread_exit(void *);
int pthread_getschedparam(pthread_t pthread, int *,struct sched_param *) ;
int pthread_setschedparam(pthread_t, int,const struct sched_param *) ;
pthread_t pthread_self(void);
int pthread_join(pthread_t, void **);
int pthread_once(pthread_once_t *, void (*) (void));

int pthread_mutex_init(pthread_mutex_t *,const pthread_mutexattr_t *) ;
int pthread_mutex_destroy(pthread_mutex_t *);
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_trylock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);

int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_destroy(pthread_mutexattr_t *);
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *,int *) ;
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *, int *);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *, int *);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *,int) ;
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
int pthread_mutexattr_setkind_np(pthread_mutexattr_t *, int);
int pthread_mutexattr_settype(pthread_mutexattr_t *, int);

int  pthread_key_create(pthread_key_t *,void (*) (void *)) ;
int  pthread_key_delete(pthread_key_t);
void *pthread_getspecific(pthread_key_t);
int  pthread_setspecific(pthread_key_t, const void *);

void pthread_cleanup_pop(int);
void pthread_cleanup_push(void (*) (void *), void *routine_arg);

int pthread_rwlock_init(pthread_rwlock_t *,const pthread_rwlockattr_t *);
int pthread_rwlock_destroy(pthread_rwlock_t *);
int pthread_rwlock_rdlock(pthread_rwlock_t *);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int pthread_rwlock_wrlock(pthread_rwlock_t *);
int pthread_rwlock_trywrlock(pthread_rwlock_t *);
int pthread_rwlock_unlock(pthread_rwlock_t *);
int pthread_rwlockattr_init(pthread_rwlockattr_t *);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *, int *);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);

_END_C_LIB_DECL
_C_STD_END

#if defined(_STD_USING)
using _CSTD pthread_attr_destroy ;
using _CSTD pthread_attr_getdetachstate ;
using _CSTD pthread_attr_getinheritsched ;
using _CSTD pthread_attr_getschedparam ;
using _CSTD pthread_attr_getschedpolicy ;
using _CSTD pthread_attr_getscope ;
using _CSTD pthread_attr_getstackaddr ;
using _CSTD pthread_attr_getstacksize ;
using _CSTD pthread_attr_init ;
using _CSTD pthread_attr_setdetachstate ;
using _CSTD pthread_attr_setinheritsched ;
using _CSTD pthread_attr_setschedparam ;
using _CSTD pthread_attr_setschedpolicy ;
using _CSTD pthread_attr_setscope ;
using _CSTD pthread_attr_setstackaddr ;
using _CSTD pthread_attr_setstacksize ;
using _CSTD pthread_cond_broadcast ;
using _CSTD pthread_cond_destroy ;
using _CSTD pthread_cond_init ;
using _CSTD pthread_cond_signal ;
using _CSTD pthread_cond_timedwait ;
using _CSTD pthread_cond_wait ;
using _CSTD pthread_condattr_init ;
using _CSTD pthread_condattr_destroy ;
using _CSTD pthread_condattr_setpshared ;
using _CSTD pthread_condattr_getpshared ;
using _CSTD pthread_create ;
using _CSTD pthread_detach ;
using _CSTD pthread_equal ;
using _CSTD pthread_exit ;
using _CSTD pthread_getschedparam ;
using _CSTD pthread_setschedparam ;
using _CSTD pthread_self ;
using _CSTD pthread_join ;
using _CSTD pthread_once ;
using _CSTD pthread_mutex_init ;
using _CSTD pthread_mutex_destroy ;
using _CSTD pthread_mutex_lock ;
using _CSTD pthread_mutex_trylock ;
using _CSTD pthread_mutex_unlock ;
using _CSTD pthread_mutexattr_init ;
using _CSTD pthread_mutexattr_destroy ;
using _CSTD pthread_mutexattr_getprotocol ;
using _CSTD pthread_mutexattr_getpshared ;
using _CSTD pthread_mutexattr_setprotocol ;
using _CSTD pthread_mutexattr_setpshared ;
using _CSTD pthread_mutexattr_setkind_np ;
using _CSTD pthread_mutexattr_settype ;
using _CSTD pthread_key_create ;
using _CSTD pthread_key_delete ;
using _CSTD pthread_getspecific ;
using _CSTD pthread_setspecific ;
using _CSTD pthread_cleanup_pop ;
using _CSTD pthread_cleanup_push ;
using _CSTD pthread_rwlock_init ;
using _CSTD pthread_rwlock_destroy ;
using _CSTD pthread_rwlock_rdlock ;
using _CSTD pthread_rwlock_tryrdlock ;
using _CSTD pthread_rwlock_wrlock ;
using _CSTD pthread_rwlock_trywrlock ;
using _CSTD pthread_rwlock_unlock ;
using _CSTD pthread_rwlockattr_init ;
using _CSTD pthread_rwlockattr_destroy ;
using _CSTD pthread_rwlockattr_getpshared ;
using _CSTD pthread_rwlockattr_setpshared ;

#endif /* defined(_STD_USING) */

#endif
