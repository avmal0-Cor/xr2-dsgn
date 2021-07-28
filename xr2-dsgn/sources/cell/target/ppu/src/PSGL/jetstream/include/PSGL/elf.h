/* elf.h - subset of ELF definition
**
** This elf.h contains only ELF header and progaram header.
** Because those are enough for ELF loader.
** 
** (C)Copyright
** Sony Computer Entertainment, Inc.,
** Toshiba Corporation,
** International Business Machines Corporation,
** 2003. All rights reserved.
** S/T/I Confidential Information
** 
** Author: Keisuke Inoue
*/

#ifndef ELF_H
#define ELF_H 1

#include <PSGL/platform.h>

/*
 * Data Representation
 */

/* 32-bit Data Types */
typedef jsUint32	Elf32_Addr;
typedef jsUint32	Elf32_Off;
typedef jsUint16	Elf32_Half;
typedef jsUint32	Elf32_Word;
typedef JSint32		Elf32_Sword;

/* 64-bit Data Types */
typedef jsUint64	Elf64_Addr;
typedef jsUint64	Elf64_Off;
typedef jsUint16	Elf64_Half;
typedef jsUint32	Elf64_Word;
typedef JSint32		Elf64_Sword;
typedef jsUint64	Elf64_Xword;
typedef JSint64		Elf64_Sxword;

/*
 * ELF header
 */

/* e_ident[] Identification Indexes */
#define EI_MAG0		0	/* File identification */
#define EI_MAG1		1	/* File identification */
#define EI_MAG2		2	/* File identification */
#define EI_MAG3		3	/* File identification */
#define EI_CLASS	4	/* File class */
#define EI_DATA		5	/* Data encoding */
#define EI_VERSION	6	/* File version */
#define EI_OSABI	7	/* Operating system/ABI identification */
#define EI_ABIVERSION	8	/* ABI version */
#define EI_PAD		9	/* Start of padding bytes */
#define EI_NIDENT	16	/* Size of e_ident[] */

/* e_ident[EI_MAG0] to e_ident[EI_MAG3] */
#define ELFMAG0		0x7f	/* e_ident[EI_MAG0] */
#define ELFMAG1		'E'	/* e_ident[EI_MAG1] */
#define ELFMAG2		'L'	/* e_ident[EI_MAG2] */
#define ELFMAG3		'F'	/* e_ident[EI_MAG3] */

/* e_ident[EI_CLASS] */
#define ELFCLASSNONE	0	/* Invalid class */
#define ELFCLASS32	1	/* 32-bit objects */
#define ELFCLASS64	2	/* 64-bit objects */

/* e_ident[EI_DATA] */
#define ELFDATANONE	0	/* Invalid data encoding */
#define ELFDATA2LSB	1	/* little endian */
#define ELFDATA2MSB	2	/* big endian */

/* e_ident[EI_VERSION] */
#define EV_NONE		0	/* Invalid version */
#define EV_CURRENT	1	/* Current version */

/* e_type */
#define ET_NONE		0	/* No file type */
#define ET_REL		1	/* Relocatable file */
#define ET_EXEC		2	/* Executable file */
#define ET_DYN		3	/* Shared object file */
#define ET_CORE		4	/* Core file */

/* e_machine */
#define EM_PPC		20	/* PowerPC 32-bit */
#define EM_PPC64	21	/* PowerPC 64-bit */

typedef struct {
  unsigned char	e_ident[EI_NIDENT];
  Elf32_Half	e_type;
  Elf32_Half	e_machine;
  Elf32_Word	e_version;
  Elf32_Addr	e_entry;
  Elf32_Off	e_phoff;
  Elf32_Off	e_shoff;
  Elf32_Word	e_flags;
  Elf32_Half	e_ehsize;
  Elf32_Half	e_phentsize;
  Elf32_Half	e_phnum;
  Elf32_Half	e_shentsize;
  Elf32_Half	e_shnum;
  Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

typedef struct {
  unsigned char	e_ident[EI_NIDENT];
  Elf64_Half	e_type;
  Elf64_Half	e_machine;
  Elf64_Word	e_version;
  Elf64_Addr	e_entry;
  Elf64_Off	e_phoff;
  Elf64_Off	e_shoff;
  Elf64_Word	e_flags;
  Elf64_Half	e_ehsize;
  Elf64_Half	e_phentsize;
  Elf64_Half	e_phnum;
  Elf64_Half	e_shentsize;
  Elf64_Half	e_shnum;
  Elf64_Half	e_shstrndx;
} Elf64_Ehdr;

/*
 * Program Header
 */

/* Segment Types, p_type */
#define PT_NULL		0
#define PT_LOAD		1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE		4
#define PT_SHLIB	5
#define PT_PHDR		6
#define PT_LOOS		0x60000000
#define PT_HIOS		0x6fffffff
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff

typedef struct {
  Elf32_Word	p_type;
  Elf32_Off	p_offset;
  Elf32_Addr	p_vaddr;
  Elf32_Addr	p_paddr;
  Elf32_Word	p_filesz;
  Elf32_Word	p_memsz;
  Elf32_Word	p_flags;
  Elf32_Word	p_align;
} Elf32_Phdr;

typedef struct {
  Elf64_Word	p_type;
  Elf64_Word	p_flags;
  Elf64_Off	p_offset;
  Elf64_Addr	p_vaddr;
  Elf64_Addr	p_paddr;
  Elf64_Xword	p_filesz;
  Elf64_Xword	p_memsz;
  Elf64_Xword	p_align;
} Elf64_Phdr;

/*
 * Section Header
 */

/* sh_type */
#define SHT_NULL	0	/* Section header table entry unused */
#define SHT_PROGBITS	1	/* Program data */
#define SHT_SYMTAB	2	/* Symbol table */
#define SHT_STRTAB	3	/* String table */
#define SHT_RELA	4	/* Relocation entries with addends */
#define SHT_HASH	5	/* Symbol hash table */
#define SHT_DYNAMIC	6	/* Dynamic linking information */
#define SHT_NOTE	7	/* Notes */
#define SHT_NOBITS	8	/* Program space with no data (bss) */
#define SHT_REL		9	/* Relocation entries, no addends */
#define SHT_SHLIB	10	/* Reserved */
#define SHT_DYNSYM	11	/* Dynamic linker symbol table */

#define SHF_WRITE	     (1 << 0)	/* Writable */
#define SHF_ALLOC	     (1 << 1)	/* Occupies memory during execution */
#define SHF_EXECINSTR	     (1 << 2)	/* Executable */
#define SHF_MERGE	     (1 << 4)	/* Might be merged */
#define SHF_STRINGS	     (1 << 5)	/* Contains nul-terminated strings */
#define SHF_INFO_LINK	     (1 << 6)	/* `sh_info' contains SHT index */
#define SHF_LINK_ORDER	     (1 << 7)	/* Preserve order after combining */
#define SHF_OS_NONCONFORMING (1 << 8)	/* Non-standard OS specific handling
					   required */
#define SHF_MASKOS	     0x0ff00000	/* OS-specific.  */
#define SHF_MASKPROC	     0xf0000000	/* Processor-specific */


typedef struct {
  Elf32_Word	sh_name;
  Elf32_Word	sh_type;
  Elf32_Word	sh_flags;
  Elf32_Addr	sh_addr;
  Elf32_Off	sh_offset;
  Elf32_Word	sh_size;
  Elf32_Word	sh_link;
  Elf32_Word	sh_info;
  Elf32_Word	sh_addralign;
  Elf32_Word	sh_entsize;
} Elf32_Shdr;

/* Special section indices.  */

#define SHN_UNDEF	0		/* Undefined section */
#define SHN_LORESERVE	0xff00		/* Start of reserved indices */
#define SHN_LOPROC	0xff00		/* Start of processor-specific */
#define SHN_HIPROC	0xff1f		/* End of processor-specific */
#define SHN_LOOS	0xff20		/* Start of OS-specific */
#define SHN_HIOS	0xff3f		/* End of OS-specific */
#define SHN_ABS		0xfff1		/* Associated symbol is absolute */
#define SHN_COMMON	0xfff2		/* Associated symbol is common */
#define SHN_XINDEX	0xffff		/* Index is in extra table.  */
#define SHN_HIRESERVE	0xffff		/* End of reserved indices */

/*
 * Symbol Header
 */

typedef struct {
  Elf32_Word	st_name;
  Elf32_Word	st_value;
  Elf32_Word	st_size;
  unsigned char	st_info;
  unsigned char	st_other;
  Elf32_Half	st_shndx;
} Elf32_Sym;


/* Relocation table entry without addend (in section of type SHT_REL).  */

typedef struct
{
  Elf32_Addr	r_offset;		/* Address */
  Elf32_Word	r_info;			/* Relocation type and symbol index */
} Elf32_Rel;

/* I have seen two different definitions of the Elf64_Rel and
   Elf64_Rela structures, so we'll leave them out until Novell (or
   whoever) gets their act together.  */
/* The following, at least, is used on Sparc v9, MIPS, and Alpha.  */

typedef struct
{
  Elf64_Addr	r_offset;		/* Address */
  Elf64_Xword	r_info;			/* Relocation type and symbol index */
} Elf64_Rel;

/* Relocation table entry with addend (in section of type SHT_RELA).  */

typedef struct
{
  Elf32_Addr	r_offset;		/* Address */
  Elf32_Word	r_info;			/* Relocation type and symbol index */
  Elf32_Sword	r_addend;		/* Addend */
} Elf32_Rela;

typedef struct
{
  Elf64_Addr	r_offset;		/* Address */
  Elf64_Xword	r_info;			/* Relocation type and symbol index */
  Elf64_Sxword	r_addend;		/* Addend */
} Elf64_Rela;

/* How to extract and insert information held in the r_info field.  */

#define ELF32_R_SYM(val)		((val) >> 8)
#define ELF32_R_TYPE(val)		((val) & 0xff)
#define ELF32_R_INFO(sym, type)		(((sym) << 8) + ((type) & 0xff))

#define ELF64_R_SYM(i)			((i) >> 32)
#define ELF64_R_TYPE(i)			((i) & 0xffffffff)
#define ELF64_R_INFO(sym,type)		((((Elf64_Xword) (sym)) << 32) + (type))


#endif /* ELF_H */

/*
** Local Variables:
** tab-width: 8
** End:
** vim:ts=8:sw=2:
*/
