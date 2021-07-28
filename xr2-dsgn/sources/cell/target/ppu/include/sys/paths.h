/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2008 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef __SYS_SYS_PATHS_H__
#define __SYS_SYS_PATHS_H__

/* The default mount point */
#define	SYS_APP_HOME	"/app_home"

/* The mount point for a file system on a remote host PC */
#define	SYS_HOST_ROOT	"/host_root"

/* The mount point for a file system on builtin HDD */
#define	SYS_DEV_HDD0	"/dev_hdd0"
#define	SYS_DEV_HDD1	"/dev_hdd1"	/* system use only */

/* The mount point for a file system on builtin memory stick */
#define	SYS_DEV_MS	"/dev_ms"

/* The mount point for a file system on builtin BDVD drive */
#define SYS_DEV_BDVD    "/dev_bdvd"

/* mount point used for debugging purpose such as core dump target */
#define SYS_DEV_USB	"/dev_usb"

#endif /* __SYS_SYS_PATHS_H__ */
