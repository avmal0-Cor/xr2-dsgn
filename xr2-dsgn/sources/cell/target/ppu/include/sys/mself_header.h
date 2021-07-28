/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#ifndef _SYS_MSELF_HEADER_H_INCLUDED
#define _SYS_MSELF_HEADER_H_INCLUDED

#include <sys/types.h>


/* mself header constant definition */
#define MSELF_MAGIC				(0x4D534600)
#define MSELF_CURRENT_FORMAT_VERSION		(1)
#define MSELF_HEADER_RESERVE_SIZE		(40)

/* mself entry constant definition */
#define MSELF_ENTRY_NAME_SIZE			(32)
#define MSELF_ENTRY_RESERVE_SIZE		(16)


struct mself_header {
	uint32_t m_magic;
	uint32_t m_format_version;
	uint64_t m_file_size;
	uint32_t m_entry_num;
	uint32_t m_entry_size;
	uint8_t m_reserve[MSELF_HEADER_RESERVE_SIZE];
};


struct mself_entry {
	uint8_t m_name[MSELF_ENTRY_NAME_SIZE];
	uint64_t m_offset;
	uint64_t m_size;
	uint8_t m_reserve[MSELF_ENTRY_RESERVE_SIZE];
};


#endif /* _SYS_MSELF_HEADER_H_INCLUDED */
