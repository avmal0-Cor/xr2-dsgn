/*
 *   SCE CONFIDENTIAL
 *   PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *   Copyright (C) 2007 Sony Computer Entertainment Inc.
 *   All Rights Reserved.
 */

/* Copyright 2005  Sony Corporation */

#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

#include <time.h>
#include <pthread_types.h>
#include <sys/synchronization.h>

_C_STD_BEGIN

/* 
 **************************************************************
                  Type definitions
 **************************************************************
 */
typedef struct s_sem {
  sys_semaphore_t sys_sem ;

} sem_t ;
  
/* 
 **************************************************************
                  Macro definitions
 **************************************************************
 */
#define SEM_VALUE_MAX  ((int)((~0U)>>1))
#define SEM_FAILED     (sem_t *0) ;

/* 
 **************************************************************
             POSIX semaphore function definitions
 **************************************************************
 */
_C_LIB_DECL

int sem_init(sem_t *sem,int pshared,unsigned int value) ;
int sem_destroy(sem_t *sem) ;
int sem_wait(sem_t *sem) ;
int sem_trywait(sem_t *sem) ;
int sem_timedwait(sem_t *sem,const struct timespec *abstime) ;
int sem_post(sem_t *sem) ;
int sem_getvalue(sem_t *sem,int *sval) ;

/* 
 *  Not implemented (Named semaphpre) 
 *
 *  sem_t *sem_open(const char *name,int flag,...) ;
 *  int    sem_close(sem_t *sem) ;
 *  int sem_unlink(const char *name) ;
 */

_END_C_LIB_DECL
_C_STD_END
#if defined(_STD_USING)
using _CSTD sem_t ;
using _CSTD sem_init ;
using _CSTD sem_destroy ;
using _CSTD sem_wait ;
using _CSTD sem_trywait ;
using _CSTD sem_timedwait ;
using _CSTD sem_post ;
using _CSTD sem_getvalue ;
#endif /* defined(_STD_USING) */

#endif
