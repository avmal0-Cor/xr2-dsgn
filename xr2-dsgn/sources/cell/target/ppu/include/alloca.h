/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2005 Sony Computer Entertainment Inc. 
 * All Rights Reserved.
 */

#ifndef _ALLOCA_H
#define _ALLOCA_H

#ifdef __SNC__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void* alloca (unsigned int size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#else /* __SNC__ */

#define alloca(size) __builtin_alloca(size)

#endif /* __SNC__ */

#endif
