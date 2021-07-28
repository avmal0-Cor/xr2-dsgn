/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

 
/*
 * Spu simulator library - ELF reader.
 * AUTHORS: Antoine Labour, Robin Green
 * DATE: 2003-Oct-16
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>			/* malloc, free */
#include <string.h>			/* memset */
#include <errno.h>
#include <PSGL/readelf.h>
#include <PSGL/platform.h>
#include <PSGL/psgl.h>
#include <PSGL/Utils.h>
#include <PSGL/private.h>

#include <PSGL/Debug.h>

#define REPORT_UNRESOLVED_SYMBOLS	1

#ifdef __FUNCTION__
#define PWHERE		printf("%s:%d:%s: ",__FILE__,__LINE__,__FUNCTION__)
#else /* !__FUNCTION__ */
#define PWHERE		printf("%s:%d:%s: ",__FILE__,__LINE__,__func__)
#endif /* __FUNCTION__ */
#define PRINTF(...)	(PWHERE,printf(__VA_ARGS__))
#define PERROR(s)	(PWHERE,perror(s))

#if JS_ENDIAN == JS_BIG_ENDIAN
# define IS_OPPOSITE_ELF_ENDIAN(s)	((s) == ELFDATA2LSB)
#else
# define IS_OPPOSITE_ELF_ENDIAN(s)	((s) == ELFDATA2MSB)
#endif


Elf32_Half endian_half(int endian, Elf32_Half v)
{
	if (IS_OPPOSITE_ELF_ENDIAN(endian)) {
		v = (v >> 8 & 0x00ff) | (v << 8 & 0xff00);
	}
	return v;
}

Elf32_Word endian_word(int endian, Elf32_Word v)
{
	if (IS_OPPOSITE_ELF_ENDIAN(endian)) {
		v = (v >> 16 & 0x0000ffffUL) | (v << 16);
		v = (v >>  8 & 0x00ff00ffUL) | (v <<  8 & 0xff00ff00UL);
	}
	return v;
}

Elf32_Sword endian_sword(int endian, Elf32_Sword v)
{
	return (Elf32_Sword)endian_word(endian,(Elf32_Word)v);
}

inline void endianSwapEhdr(int endian,Elf32_Ehdr *ehdr)
{
	ehdr->e_type      = endian_half(endian, ehdr->e_type);
	ehdr->e_machine   = endian_half(endian, ehdr->e_machine);
	ehdr->e_version   = endian_word(endian, ehdr->e_version);
	ehdr->e_entry     = endian_addr(endian, ehdr->e_entry);
	ehdr->e_phoff     = endian_off(endian, ehdr->e_phoff);
	ehdr->e_shoff     = endian_off(endian, ehdr->e_shoff);
	ehdr->e_flags     = endian_word(endian, ehdr->e_flags);
	ehdr->e_ehsize    = endian_half(endian, ehdr->e_ehsize);
	ehdr->e_phentsize = endian_half(endian, ehdr->e_phentsize);
	ehdr->e_phnum     = endian_half(endian, ehdr->e_phnum);
	ehdr->e_shentsize = endian_half(endian, ehdr->e_shentsize);
	ehdr->e_shnum     = endian_half(endian, ehdr->e_shnum);
	ehdr->e_shstrndx  = endian_half(endian, ehdr->e_shstrndx);
}

inline void endianSwapShdr(int endian,Elf32_Shdr *shdr)
{
	shdr->sh_name      = endian_word(endian, shdr->sh_name);
	shdr->sh_type      = endian_word(endian, shdr->sh_type);
	shdr->sh_flags     = endian_word(endian, shdr->sh_flags);
	shdr->sh_addr      = endian_addr(endian, shdr->sh_addr);
	shdr->sh_offset    = endian_off(endian, shdr->sh_offset);
	shdr->sh_size      = endian_word(endian, shdr->sh_size);
	shdr->sh_link      = endian_word(endian, shdr->sh_link);
	shdr->sh_info      = endian_word(endian, shdr->sh_info);
	shdr->sh_addralign = endian_word(endian, shdr->sh_addralign);
	shdr->sh_entsize   = endian_word(endian, shdr->sh_entsize);
}

inline void endianSwapPhdr(int endian,Elf32_Phdr *phdr)
{
	phdr->p_type   = endian_word(endian, phdr->p_type);
	phdr->p_offset = endian_off(endian, phdr->p_offset);
	phdr->p_vaddr  = endian_addr(endian, phdr->p_vaddr);
	phdr->p_paddr  = endian_addr(endian, phdr->p_paddr);
	phdr->p_filesz = endian_word(endian, phdr->p_filesz);
	phdr->p_memsz  = endian_word(endian, phdr->p_memsz);
	phdr->p_flags  = endian_word(endian, phdr->p_flags);
	phdr->p_align  = endian_word(endian, phdr->p_align);
}

inline void endianSwapSym(int endian,Elf32_Sym *sym)
{
	sym->st_name  = endian_word(endian, sym->st_name);
	sym->st_value = endian_word(endian, sym->st_value);
	sym->st_size  = endian_word(endian, sym->st_size);
	sym->st_shndx = endian_half(endian, sym->st_shndx);
}

inline void endianSwapRel(int endian,Elf32_Rel *rel)
{
	rel->r_offset = endian_addr(endian, rel->r_offset);
	rel->r_info   = endian_word(endian, rel->r_info);
}

inline void endianSwapRela(int endian,Elf32_Rela *rel)
{
	endianSwapRel(endian,(Elf32_Rel *)rel);
	rel->r_addend = endian_sword(endian, rel->r_addend);
}

struct _Streamer_t;

typedef int (* readFunction)(void* buffer, int count, struct _Streamer_t* streamer);
typedef int (* seekFunction)(int count, struct _Streamer_t* streamer);

typedef struct _Streamer_t
{
	readFunction fread;
	seekFunction fseek;
	const void *handle;
	unsigned int pointer;
	unsigned int size;
} Streamer_t;



int file_fread(void *buffer,int count,Streamer_t *streamer)
{
	FILE *fp=(FILE *)streamer->handle;
	return fread(buffer,count,1,fp);
}

int file_fseek(int count,Streamer_t *streamer)
{
	FILE *fp=(FILE *)streamer->handle;
	return fseek(fp,count,SEEK_SET);
}

int memory_fread(void *buffer,int count,Streamer_t *streamer)
{
	const char *memory=(const char *)streamer->handle;
	// temporarily using 0 as a value to handle elfs transparently.  to be removed after sdk 0.8.5
	if (streamer->pointer+count>streamer->size && streamer->size!=0) return 0;
	if (streamer->pointer<0) return 0;
	memcpy(buffer,memory+streamer->pointer,count);
	streamer->pointer+=count;
	return 1;
}

int memory_fseek(int count,Streamer_t *streamer)
{
	// temporarily using 0 as a value to handle elfs transparently.  to be removed after sdk 0.8.5
	if (count>(int)streamer->size && streamer->size!=0) count=streamer->size;
	if (count<0) count=0;
	streamer->pointer=count;
	return 0;
}

int readSection(ELF_t *elf,Elf32_Off secoff,GLuint sectionIdx,Streamer_t *streamer)
{
	// given a section header index, read in the section header and
	// the data, setting section->data with a pointer to the allocated memory

	// TODO: *** make sure loading a section respects the alignment specified
	// TODO: *** in "sh_addralign", which is currently ignored

	unsigned int endian = elf->endian;
	
	// if we're requesting an out-of-bounds section header, give up.
	if (sectionIdx >= elf->sectionCount)
	{
		return -1;
	}

	// get a pointer to the section header (this assumes the section storage has been allocated)
	ELF_section_t *section = elf->sections + sectionIdx;

	// attempt to read the section header from the section header table
	// first checking that the section header is empty (no shdr loaded)

	// TODO: *** assumes uninitialised data is zero filled and that a section of
	// TODO: *** type SHT_NULL will never occur or has no use in a valid file

	if (section->header.sh_type == SHT_NULL)
	{
		// first, attempt to seek the location of the section header in the file
		if (streamer->fseek(secoff + sectionIdx*sizeof(Elf32_Shdr), streamer) < 0)
		{
			// seek failed, return failure
			return -1;
		}

		// if the seek worked, read an Elf32_Shdr structure form the file
		if (streamer->fread(&section->header, sizeof (Elf32_Shdr), streamer) < 1)
		{
			// read failed, return failure
			return -1;
		}

		// make sure the structure just read is endian-swapped for this platform.
		endianSwapShdr(endian,&section->header);
	}

	// find out the size of the section
	unsigned int size = section->header.sh_size;
	if (size <= 0 || section->data != 0)
	{
		// no data to read or data already read, return success.
		return 0;
	}
	
	// now we have the header, allocate storage for the section data
	section->data = (char *)jsMalloc(size);
	if (!section->data)
	{
		// memory allocation problem, return failure.
		return -1;
	}

	// attempt to seek the location of the section data in the ELF file.
	if (streamer->fseek(section->header.sh_offset,streamer) < 0)
	{
		// seek failed, release memory and return failure
		jsFree(section->data);
		section->data = NULL;
		return -1;
	}

	// if the seek succeeded, read the data block into memory
	if (streamer->fread(section->data,size,streamer) < 1)
	{
		// read failed, release memory and return failure
		jsFree(section->data);
		section->data = NULL;
		return -1;
	}

	// return success
	return 0;
}

int readSymbolSection(ELF_t* elf, Elf32_Off shoff, int sectionIdx, Streamer_t* streamer)
{
	// first, read the section header
	if (readSection(elf, shoff, sectionIdx, streamer)<0)
	{
		return -1;
	}

	// get a direct pointer to the section record
	ELF_section_t* section = elf->sections + sectionIdx;

	// if the section contains data that has been loaded...
	if (section->data)
	{
		// get the size of a single entry in the section (never assume about padding)
		int esize = section->header.sh_entsize;

		// calculate the number of elements in this data block
		elf->symbolCount = section->header.sh_size / esize;

		// if there are any symbols in this data block, expand the symbols into the elf record
		if (elf->symbolCount > 0)
		{
			// allocate memory for all the the symbol records as a block
			elf->symbols = (ELF_symbol_t*)jsMalloc(elf->symbolCount * sizeof(ELF_symbol_t));
			if (!elf->symbols)
			{
				// allocation for symbol records failed
				return -1;
			}
			
			// walk through the symbols expanding the symtab entry into a symbol record
			ELF_symbol_t* symbol = elf->symbols;
			char* data = section->data;
			for (GLuint i = 0; i<elf->symbolCount; ++i)
			{
				// get a pointer to the symbol
				Elf32_Sym* elf_sym = (Elf32_Sym*)data;

				// swap the data to match the endinness of this machine
				endianSwapSym(elf->endian, elf_sym);

				// expand the information into a symbol record
				// NOTE: the address of the string table (strtab) is not necessarily known yet
				
				// offset of symbol name in the strtab
				symbol->name = (char*)(0)+elf_sym->st_name;

				// is this the definition of a symbol relative to a section?
				symbol->resolved = (elf_sym->st_shndx != SHN_UNDEF);

				// size of the symbol (non-zero if the symbol is a function)
				symbol->size = elf_sym->st_size;

				// address within the section where the symbol is defined
				// TODO: *** this seems to assume the symbol is always an STT_OBJECT
				symbol->vma = elf_sym->st_value;

				// note the section where the symbol is defined, if known
				symbol->section = symbol->resolved ? elf_sym->st_shndx : 0;

				// is the symbol defined in the current elf or defined externally?
				// TODO: this assumes only two contexts for a symbol to be defined from.
				symbol->foreign = 0;
				
				// TODO: *** the symbol record makes no note of the type or binding of the symbol

				// advance the pointers for the next iteration
				data += esize;
				++symbol;
			}
		}
	}

	// return success
	return 0;
}

void retrieveSymbolNames(ELF_t* elf)
{
	// by now we have loaded the symtab and it's associated strtab
	// run through each symbol and patch up the name pointer to the strtab

	// convert the address of the strtab into an integer offset from zero
	unsigned int offset = elf->sections[elf->symbolNamesSection].data - (char *)NULL;

	// add that offset to the strtab offset already in each symbol record.
	ELF_symbol_t* symbol = elf->symbols;
	for (GLuint i=0; i<elf->symbolCount; ++i)
	{
		symbol->name += offset;
		++symbol;
	}
}

int readRelSection(ELF_t* elf, Elf32_Off secoff, int sectionIdx, Streamer_t* streamer)
{
	// read and record the relocation records associated with a section

	// first, read in the data into the section record
	if (readSection(elf, secoff, sectionIdx, streamer) < 0)
	{
		// read failed, return failure
		return -1;
	}

	// get a direct pointer to the section record
	ELF_section_t* section = elf->sections + sectionIdx;

	// is the section a REL+ADDEND or REL type?
	int rela = (section->header.sh_type == SHT_RELA) ? 1 : 0;

	// if the section record has data...
	if (section->data)
	{
		char* data = section->data;

		// find out how many relocation records there are in this data section
		int esize = section->header.sh_entsize;
		int num = section->header.sh_size / esize;

		// loop over all relocation records
		for (int i=0; i<num; ++i)
		{
			// swap the endianness of the data to match this machine
			if (rela)
			{
				endianSwapRela(elf->endian,(Elf32_Rela*)data);
			}
			else 
			{
				endianSwapRel(elf->endian,(Elf32_Rel*)data);
			}

			// update the pointer to the next relocation record
			data += esize;
		}
	}

	// return success
	return 0;
}

ELF_t *readElf(Streamer_t *streamer)
{
	ELF_t *elf;
	Elf32_Ehdr ehdr;

	// read just the ELF header
	if (streamer->fread(&ehdr, sizeof(Elf32_Ehdr), streamer) < 1)
	{
		// read failed.
		return NULL;
	}
	if (!(ehdr.e_ident[EI_MAG0] == ELFMAG0 &&
			ehdr.e_ident[EI_MAG1] == ELFMAG1 &&
			ehdr.e_ident[EI_MAG2] == ELFMAG2 &&
			ehdr.e_ident[EI_MAG3] == ELFMAG3))
	{
		// file is not an ELF file, header does not match the magic token "\0x7fELF"
		return NULL;
	}

	// allocate memory and zero it
	elf = (ELF_t *)jsMalloc(sizeof(ELF_t));
	if (!elf)
	{
		// memory allocation failed.
		return NULL;
	}

	elf->paramSection = 0;
	// get the endian flag
	elf->endian = ehdr.e_ident[EI_DATA];

	// shuffle the Ehdr structure to match the machine endianness
	endianSwapEhdr(elf->endian, &ehdr);

	// check to see if the file type is a relocatable file (object file).
	// *** NOTE: unclear, we could equally call it a shared object or a dynamic library.
	elf->relocatable = (ehdr.e_type == ET_REL);

	// record the file offset of the section header table
	Elf32_Off shoff = ehdr.e_shoff;

	// record the number of sections in the section header table.
	elf->sectionCount = ehdr.e_shnum;

	// allocate memory for the section header table
	// TODO: *** ELF_section_t should be using e_shentsize instead of sizeof()
	elf->sections = (ELF_section_t*)jsMalloc(elf->sectionCount * sizeof(ELF_section_t));
	if (!elf->sections)
	{
		// section header memory allocation failed, clean up and exit.
		freeElf(elf);
		return NULL;
	}

	// clear the section header table (readSection requires this)
	memset(elf->sections, 0, elf->sectionCount * sizeof(ELF_section_t) );

	// record the number of segments in this file (and there should be none for a shader)
	elf->segmentCount = ehdr.e_phnum;
	
	if (elf->segmentCount)
	{
	// allocate space for the segment header table
	elf->segments = (ELF_segment_t *)jsMalloc(elf->segmentCount * sizeof(ELF_segment_t));
	if (!elf->segments)
	{
		// segment header memory allocation failed, clean up and exit.
		freeElf(elf);
		return NULL;
	}
	}
	else
		elf->segments = NULL;

	// initialise the symbol table as empty
	elf->symbols=NULL;
	elf->symbolCount=0;

	// record which section header points to the section header string table (.shstrtab)
	int stringSectionIndex = ehdr.e_shstrndx;

	// read in the section header string table from the section header table
	if (readSection(elf, shoff, stringSectionIndex, streamer)<0)
	{
		// if reading the section header string table fails, give up.
		freeElf(elf);
		return NULL;
	}
	
	// get a pointer to the section header string table
	char* shstrtab = elf->sections[stringSectionIndex].data;
	
	// seek the beginning of the section header table
	// TODO *** which we already did while loading the section header string table
	if (streamer->fseek(shoff, streamer) < 0)
	{
		freeElf(elf);
		return NULL;
	}

	// read the section header table entries one by one
	for (GLuint i=0; i<elf->sectionCount; ++i)
	{
		ELF_section_t* section = elf->sections + i;
		if (streamer->fread(&section->header, sizeof(Elf32_Shdr), streamer) < 1)
		{
			freeElf(elf);
			return NULL;
		}

		// convert the structure read from the file to the endianness of this machine
		endianSwapShdr(elf->endian, &section->header);

		// look up the section name in the shstrtab and record it as a char*
		section->name = shstrtab + section->header.sh_name;
	}
	
	// go through each section and load the associated data if there is any.
	for (GLuint i = 0; i<elf->sectionCount; i++)
	{
		ELF_section_t* section = elf->sections + i;

		// check the section type
		switch(section->header.sh_type)
		{
 		case SHT_SYMTAB:
			// the section is a symbol table
			if (readSymbolSection(elf, shoff, i, streamer)<0)
			{
				// reading the symbol section failed, clean up and return failure
				freeElf(elf);
				return NULL;
			}
			// record which section contains the symbol table
			// TODO: this assumes only one symbol table per elf (which is the ELF 1.1 standard)
			elf->symbolsSection = i;
			break;

		case SHT_REL:
		case SHT_RELA:
			// the section is a set of relocation records
			if (readRelSection(elf, shoff, i, streamer)<0)
			{
				// reading the relocation records failed, clean up and return failure
				freeElf(elf);
				return NULL;
			}
			break;

		default:
			// check to see whether the section allocates space in an executable
			// TODO: *** this covers not only SHT_PROGBITS and SHT_NOBITS sections
			// TODO: *** but also SHT_STRTAB and SHT_HASH. Do we want this?
			if ((elf->segmentCount==0) && (section->header.sh_flags&SHF_ALLOC))
			{
				// TODO: *** why must segmentCount be zero? So we have only one executable
				// TODO: *** section in the executable image? 
				
				// read in a block of executable instructions
				if (readSection(elf, shoff, i, streamer)<0)
				{
					// read failed, clean up and return failure.
					freeElf(elf);
					return NULL;
				}
			}
			break;
				
		}
	}

	// if we have read in a symbol table and it contains symbols...
	if ((elf->symbolsSection>0) && (elf->symbolCount>0))
	{
		// get a direct pointer to the symbol table record
		ELF_section_t* section = elf->sections + elf->symbolsSection;
		
		// for a symbol table the sh_link field tells us which section the strtab is in
		elf->symbolNamesSection = section->header.sh_link;
		
		// if there is no string table (strtab)...
		if (elf->symbolNamesSection <= 0)
		{
			// clean up and return failure
			freeElf(elf);
			return NULL;
		}

		// otherwise read in the strtab
		if (readSection(elf, shoff, elf->symbolNamesSection, streamer)<0)
		{
			// read failed, clean up and return failure
			freeElf(elf);
			return NULL;
		}

		// process the symbols to resolve pointers to their names
		retrieveSymbolNames(elf);
	}

	// next we turn our attention to program segments...

// ----------------

	// get the size of a segment header record (program header)
	Elf32_Half e_phentsize = ehdr.e_phentsize;

	// TODO: *** aren't we forgetting that segments and sections may well reference the same data?
	for (GLuint i = 0; i<elf->segmentCount; ++i)
	{
		// assuming that the ELF32_Phdr structure is embedded at the start of a
		// e_phentsize block (the padding occurs after the structure), seek the
		// beginning of the phdr record
		if (streamer->fseek(ehdr.e_phoff + i * e_phentsize, streamer) < 0)
		{
			// seek failed, clean up and return failure
			freeElf(elf);
			return NULL;
		}

		// read the segment header
		if (streamer->fread(&elf->segments[i].header, sizeof(Elf32_Phdr), streamer) < 1)
		{
			// seek failed, clean up and return failure
			freeElf(elf);
			return NULL;
		}
		
		// endian swap the phdr structure to match this machine
		endianSwapPhdr(elf->endian, &elf->segments[i].header);

		// get the number of bytes of filespace this segment occupies
		Elf32_Word p_filesz = elf->segments[i].header.p_filesz;

		// if the segment has no bytes, keep on looking for segments
		if (p_filesz == 0) continue;

		// allocate space for the executable data plus extra padding for 128-byte alignment
		elf->segments[i].pointer = (unsigned char *)jsMalloc(p_filesz + 128);
		if (!elf->segments[i].pointer)
		{
			// allocation failed, clean up and return failure
			freeElf(elf);
			return NULL;
		}

		// make sure the data pointer is 128-byte aligned
		elf->segments[i].data = (unsigned char *)( ((long)(elf->segments[i].pointer + 127)) & ~127);
		
		// get the file offset of the executable data
		Elf32_Off p_offset = elf->segments[i].header.p_offset;

		// attempt to seek the data
		if (streamer->fseek(p_offset, streamer) < 0)
		{
			// seek failed, clean up and return failure
			freeElf(elf);
			return NULL;
		}

		// if the seek succeeded, read the data into the aligned storage
		if (streamer->fread(elf->segments[i].data, p_filesz, streamer) < 1)
		{
			// read failed, clean up and return failure
			freeElf(elf);
			return NULL;
		}
	}

// ----------------

	// finally record the entry point (which may be zero) in target memory space
	elf->entrypoint = ehdr.e_entry;

	// succeeded
	return elf;
}

ELF_t *readElfFromFile(const char *filename)
{
	FILE *fp;
	Streamer_t streamer;

	if ((fp = fopen(filename, "rb")) == NULL) return NULL;
	
	streamer.fread=file_fread;
	streamer.fseek=file_fseek;
	streamer.handle=fp;
	ELF_t *elf=readElf(&streamer);
	
	fclose(fp);
	
	return elf;
}

ELF_t *readElfFromMemory(const char *memory,unsigned int size)
{
	Streamer_t streamer;

	streamer.fread=memory_fread;
	streamer.fseek=memory_fseek;
	streamer.handle=memory;
	streamer.pointer=0;
	streamer.size=size;

	ELF_t *elf=readElf(&streamer);
	
	return elf;
}

ELF_section_t *findSection(const ELF_t *elf,const char *name)
{
	GLuint i;
	for (i=0; i<elf->sectionCount; i++)
	{
		if (!strcmp(elf->sections[i].name, name)) return elf->sections+i;
	}
	return NULL;
}

const Elf32_Sym *getSymbolByIndex(const ELF_t *elf,int idx)
{
	if (!elf) return NULL;
	if (elf->symbolsSection<=0) return NULL;
	ELF_section_t *section=elf->sections+elf->symbolsSection;
	if (!section->data) return NULL;
	if ((idx<0) || (idx>=(int)elf->symbolCount)) return NULL;
	return (Elf32_Sym *)(section->data+section->header.sh_entsize*idx);
}

int lookupSymbol(const ELF_t *elf,const char *name)
{
	GLuint i;
	if (!elf||!name) return -1;
	ELF_symbol_t *symbol=elf->symbols;
	if (!symbol) return -1;

	for (i=0;i<elf->symbolCount;++i,++symbol)
	{
		if (strcmp(name,symbol->name)==0) 
		    {
			return i;
		    }
	}
	return -1;
}

int lookupResolvedSymbol(const ELF_t *elf,const char *name)
{
	// given a symbol name, find out if it is defined (resolved) in
	// some section inside this ELF, and return it's index if it is found
	// or -1 if failed.

	if ( NULL == elf || NULL == name)
	{
		// bad parameters, return failure
		return -1;
	}

	// if the elf has no symbols defined, return failure.
	ELF_symbol_t* symbol = elf->symbols;
	if (!symbol)
	{
		return -1;
	}

	// loop through each symbol looking for a resolved symbol with the correct name
	for (GLuint i = 0; i<elf->symbolCount; ++i)
	{
		if (symbol->resolved)
		{
			if (strcmp(name,symbol->name)==0)
			{
				return i;
			}
		}
		++symbol;
	}
	return -1;
}

int resolveElf(ELF_t *main_elf,ELF_t *elf)
{
	int unresolved=0;

	// for each symbol in the secondary elf...
	for (GLuint i = 0; i<elf->symbolCount; ++i)
	{
		ELF_symbol_t* symbol = elf->symbols + i;

		// if the symbol does not yet have a section it is defined in...
		if (!symbol->resolved)
		{
			// see if the unresolved symbol is defined by name in the main_elf first
			int symbolIndex = lookupResolvedSymbol(main_elf,symbol->name);
			if (symbolIndex >= 0)
			{
				// symbol was found by name, patch the elf symbol table to point to
				// to the found symbol (this does not create a memory leak, the symbol table 
				// is allcated as a whole).
				*symbol = main_elf->symbols[symbolIndex]; // this symbol uses the main elf's name.
				symbol->foreign = 1;
			}
			else 
			{
				symbolIndex = lookupResolvedSymbol(elf,symbol->name);
				if (symbolIndex >= 0)
				{
					*symbol=elf->symbols[symbolIndex];
				}
				else
				{
					// unresolved symbol
#if REPORT_UNRESOLVED_SYMBOLS
					printf("Unresolved : %s\n",symbol->name);
#endif
					++unresolved;
					continue; 
				}
			}
		}
	}
	return unresolved;
}

int relocateSymbols(ELF_t *elf,unsigned int origin)
{
	// for each symbol in the relocation section, apply a base offset.

	// size is also used as the next free byte
	int size = 0;

	// for each section...
	for (GLuint i = 0; i<elf->sectionCount; ++i)
	{
		ELF_section_t* section = elf->sections + i;

		// if the section is allocated in the executable image
		if (section->header.sh_flags & SHF_ALLOC)
		{
			// find out the byte alignment for this section
			unsigned int align = section->header.sh_addralign;
			
			// make sure the alignment is a power of two
			JS_ASSERT((align&(align-1))==0);

			// work out where, in the final image, the first byte of this section will reside
			unsigned int addr = (origin + size + align-1) & ~(align-1);

			// record this address in the ELF header
			section->header.sh_addr = addr;

			// update the size of the section (works for SHT_NOBITS sections too)
			size = addr - origin + section->header.sh_size;
		}
	}

	// for each symbol...
	for (GLuint i = 0; i<elf->symbolCount; ++i)
	{
		ELF_symbol_t* symbol = elf->symbols + i;

		// if the symbol is resolved (has a section) and is not foreign (?)
		if (symbol->resolved && !symbol->foreign) 
		{
			// find out which section the symbol is defined in
			int idx = symbol->section;

			// check that this is a valid section (section 0 is always null)
			JS_ASSERT(idx>=0);

			if (idx < (int)elf->sectionCount)
			{
				// add the section base address to the symbol offset
				symbol->vma += elf->sections[idx].header.sh_addr;
			}
			else if (idx == SHN_COMMON)
			{
				// if the symbol lives in a common block
				// TODO: *** this should never happen.
				//JS_ASSERT(0);

				// align size+origin on symbol->vma which is the alignment requirement
				unsigned int align = symbol->vma;
				JS_ASSERT((align&(align-1))==0);
				unsigned int addr=(size + origin + align-1) &~ (align-1);
				symbol->vma = addr;
				size = addr - origin + symbol->size;
			}
			else
			{
				// section has an absolute address that cannot be changed.
				// TODO: *** this should never happen.
				JS_ASSERT(idx == SHN_ABS);
			}
		}
	}
	return size;
}

int loadSectionsToMemory(ELF_t* elf, char* memory, int memorySize, unsigned int origin)
{
	// copy out each section from the elf, in order, into the buffer at "memory"
	// not exceeding the memorySize.

	int offset = 0;
	for (GLuint i = 0; i<elf->sectionCount; ++i)
	{
		// get a pointer to the section
		ELF_section_t* section = elf->sections + i;
		// if the section should be allocated in the executable image...
		if (section->header.sh_flags & SHF_ALLOC)
		{
			// get it's size
			int size = section->header.sh_size;

			// work out where the section is to be put
			// NOTE: *** this resets the offset to the precalculated address for the section
			// NOTE: *** this also respects the alignment calculated earlier.
			offset = section->header.sh_addr - origin;

			// do not overflow the memory size
			if (offset + size > memorySize)
			{
				// memory overflowed, return failure
				return -1;
			}
			
			// copy the section data into the executable image at the location 
			memcpy(memory + offset, section->data, size);

			// update the offset.
			offset += size;
		}
	}

	// initialize the bss by zeroing out all the remaining space
	// NOTE: *** this only works if the final section allocated is the last one
	// NOTE: *** allocated in the memory buffer, so the offset will be correct
	memset(memory + offset, 0, memorySize - offset);

	// return success
	return 0;
}

void doRelocations(ELF_t* elf, char* memory, int memorySize, int origin,
				   const ELF_rel_type_t* rel_types, unsigned int rel_types_count)
{
	// apply the relocation records to the executable image

	// for each section...
	for (GLuint i = 0; i<elf->sectionCount; ++i)
	{
		ELF_section_t* section = elf->sections + i;

		// if the section is not a REL or a RELA section, keep looking
		if ((section->header.sh_type != SHT_RELA) && (section->header.sh_type != SHT_REL)) continue;

		// is this a REL or a RELA section?
		int is_rela = (section->header.sh_type == SHT_RELA);

		// which section do these relocations apply to?
		int relocating_idx = section->header.sh_info;

		// get a pointer to that section
		ELF_section_t* relocating_section = elf->sections + relocating_idx;

		// if the section does not take space in the executable image, keep looking
		if (!(relocating_section->header.sh_flags & SHF_ALLOC)) continue;

		// the size of a single entity in the REL or RELA section
		int esize = section->header.sh_entsize;
		
		// work out how many records there are in this section
		int num = section->header.sh_size / esize;

		char* secdata = section->data;
		JS_ASSERT(secdata);

		// for each relocation record...
		for (int j = 0; j<num; ++j, secdata += esize)
		{
			// get a pointer to the relocation record
			// NOTE: RELA is a superset of REL
			Elf32_Rela* rel = (Elf32_Rela*)secdata;

			// get a pointer to the symbol this relocation refers to
			ELF_symbol_t* sym = elf->symbols + ELF32_R_SYM(rel->r_info);
			// make sure the symbol has a resolved address
			JS_ASSERT(sym->resolved);

			// get the address of the instruction to apply the relocation to
			unsigned int addr = rel->r_offset;
			if (elf->relocatable)
			{
				// if the elf is relocatable, the address is relative to the section start
				addr += relocating_section->header.sh_addr;
			}

			// find out what type of relocation to perform
			unsigned int type = ELF32_R_TYPE(rel->r_info);

			// if the type number is not recognised, skip over this one.
			// TODO: *** maybe report an error here?
			if ((type == 0) || (type >= rel_types_count)) continue;

			// get the offset of the insttuction in the executable image
			int offset = addr - origin;

			// check that this offset is within the executable image
			if (offset>=memorySize)
			{
				// offset overflows the memory image, skip this relocation
				JS_ASSERT(0);
				continue;
			}

			// get a pointer to the instruction to patch
			unsigned int* ptr = (unsigned int*)(memory + offset);
			
			// get the instruction into a uint and make sure it's endian correct for this machine
			unsigned int data = endian_word(elf->endian, *ptr);

			// extract the relocation information from the tables
			unsigned int mask = rel_types[type].mask;
			unsigned int shift = rel_types[type].shift;
			unsigned int position = rel_types[type].position;

			// if this is not a RELA relocation, the data in the instruction is the addend
			unsigned int inplace_addend = ( (data & mask) >> position ) << shift;

			// if this is a RELA relocation, use the addend, else use the data in the instruction
			unsigned int addend = is_rela ? rel->r_addend : inplace_addend;

			// if the relocation is a relative one (and SPU only has one: REL_SPU_REL16)
			// then subtract the PC from the addend making it PC relative.
			if (rel_types[type].relative)
			{
				addend -= addr;
			}

			// reconstruct the final value to patch the instruction with
			unsigned int value = ( (sym->vma + addend) >> shift ) << position;
			
			// slot the value into the instruction masking off all the bits that need to stay
			data = ( data & ~mask ) | (value & mask);

			// write back the patched instruction in the correct endianness
			*ptr = endian_word(elf->endian, data);
		}

	}
}

int loadElfToMemory(unsigned char *memory, unsigned int size, unsigned int load_address, ELF_t *elf)
{
	GLuint i;

	// load
	for (i=0;i<elf->segmentCount;++i)
	{
		Elf32_Addr p_vaddr;
		Elf32_Word p_filesz;
		Elf32_Word p_memsz;
		if (elf->segments[i].header.p_type != PT_LOAD) continue;

		p_vaddr  = elf->segments[i].header.p_vaddr + load_address;
		p_filesz = elf->segments[i].header.p_filesz;
		p_memsz  = elf->segments[i].header.p_memsz;

		if (p_vaddr + p_memsz > size) return -9; // ENOMEM
		if (p_filesz>0)
		{
			if (!elf->segments[i].data) return -11; // EINVAL
			memcpy(memory+p_vaddr,elf->segments[i].data,p_filesz);
		}
		if (p_filesz < p_memsz)
		{
			memset(memory+p_vaddr+p_filesz, 0, p_memsz - p_filesz);
		}
	}

	// GOT relocation
	if (load_address>0)
	{
		ELF_section_t *gotSection=findSection(elf,".got");
		if (gotSection)
		{
			unsigned int seek_offset = gotSection->header.sh_offset;
			unsigned int entry_size = 0x1 << gotSection->header.sh_addralign;
			unsigned int num_entries = (gotSection->header.sh_size / entry_size) - 1;
			unsigned char *start=memory+load_address+seek_offset;

			for (i=0; i<num_entries; i++)
			{
				unsigned int *entry = (unsigned int *)start;
				*entry = endian_word(elf->endian, endian_word(elf->endian,*entry) + load_address);
				start += entry_size;
			}
		}
	}

	return 0;
}

void freeElf(ELF_t *elf)
{
	GLuint i;
	if (!elf) return;
	if (elf->sections)
	{
		for (i=0;i<elf->sectionCount;++i)
		{
			if (elf->sections[i].data) jsFree(elf->sections[i].data);
		}
		jsFree(elf->sections);
	}
	if (elf->segments)
	{
		for (i=0;i<elf->segmentCount;++i)
		{
			if (elf->segments[i].pointer) jsFree(elf->segments[i].pointer);
		}
		jsFree(elf->segments);
	}
	if (elf->symbols)
		jsFree(elf->symbols);
	jsFree(elf);
}

// spu specific relocations.

static const ELF_rel_type_t spu_rel_types[]=
{
	/*relative, shift, size, position, mask */
	{0, 0,   0,  0, 0x00000000}, /* REL_SPU_NONE       0 */
	{0, 4,  10, 14, 0x00ffc000}, /* REL_SPU_ADDR10     1 */
	{0, 2,  16,  7, 0x007fff80}, /* REL_SPU_ADDR16     2 */
	{0, 0,  16,  7, 0x007fff80}, /* REL_SPU_ADDR16_LO  3 */
	{0, 16, 16,  7, 0x007fff80}, /* REL_SPU_ADDR16_HI  4 */
	{0, 0,  18,  7, 0x01ffff80}, /* REL_SPU_ADDR18     5 */
	{0, 0,  32,  0, 0xffffffff}, /* REL_SPU_GLOB_DAT   6 */
	{1, 2,  16,  7, 0x007fff80}, /* REL_SPU_REL16      7 */
	{0, 0,   7, 14, 0x001fc000}, /* REL_SPU_IMM7       8 */
	{0, 0,   8, 14, 0x003fc000}, /* REL_SPU_IMM8       9 */
	{0, 0,  10, 14, 0x00ffc000}, /* REL_SPU_IM10      10 */
};

static const int spu_rel_types_count=sizeof(spu_rel_types)/sizeof(spu_rel_types[0]);

void spuDoRelocations(ELF_t *elf,char *memory,int memorySize,int origin)
{
	doRelocations(elf, memory, memorySize, origin, spu_rel_types, spu_rel_types_count);
}

static const char *_getStringTable(const Elf32_Ehdr *ehdr)
{
	const char *sectionHeaderStart = (const char*)ehdr + ehdr->e_shoff;
	const Elf32_Shdr *shstrtabHeader = (const Elf32_Shdr*)sectionHeaderStart + ehdr->e_shstrndx;
	return (const char*)ehdr + shstrtabHeader->sh_offset;
}

const char *findSectionInPlace(const char* memory,unsigned int /*size*/,const char *name, size_t *sectionSize)
{
	const Elf32_Ehdr *ehdr = (const Elf32_Ehdr*)memory;
	
	//first get the string section header
	const char *shstrtab = _getStringTable(ehdr);
	
	//find the section
	size_t sectionCount = ehdr->e_shnum;
	const char *sectionHeaderStart = (const char*)ehdr + ehdr->e_shoff;
	for (size_t i=0;i<sectionCount;i++)
	{
		const Elf32_Shdr *sectionHeader = (const Elf32_Shdr *)sectionHeaderStart + i;
		const char *sectionName = shstrtab + sectionHeader->sh_name;
		if (!strcmp(name,sectionName))
		{
			*sectionSize = sectionHeader->sh_size;
			return (const char*)ehdr + sectionHeader->sh_offset;
		}
	}
	return NULL;
}

const char *findSymbolSectionInPlace(const char *memory, unsigned int /*size*/, size_t *symbolSize, size_t *symbolCount, const char **symbolstrtab)
{
	const Elf32_Ehdr *ehdr = (const Elf32_Ehdr*)memory;
	
	//find the section
	size_t sectionCount = ehdr->e_shnum;
	const char *sectionHeaderStart = (const char*)ehdr + ehdr->e_shoff;
	for (size_t i=0;i<sectionCount;i++)
	{
		const Elf32_Shdr *sectionHeader = (const Elf32_Shdr *)sectionHeaderStart + i;
		if (sectionHeader->sh_type == SHT_SYMTAB)
		{
			*symbolSize = sectionHeader->sh_entsize;
			*symbolCount = sectionHeader->sh_size / sectionHeader->sh_entsize;

			const Elf32_Shdr *symbolStrHeader = (const Elf32_Shdr *)sectionHeaderStart + sectionHeader->sh_link;
			*symbolstrtab = (const char*)ehdr + symbolStrHeader->sh_offset;
			return (const char*)ehdr + sectionHeader->sh_offset;
		}
	}
	return NULL;
}


int lookupSymbolValueInPlace(const char* symbolSection, size_t symbolSize, size_t symbolCount, const char *symbolstrtab, const char *name)
{
	for (size_t i=0;i<symbolCount;i++)
	{
		Elf32_Sym* elf_sym = (Elf32_Sym*)symbolSection;

		if (!strcmp(symbolstrtab + elf_sym->st_name, name))
		{
			return elf_sym->st_value;
		}
		symbolSection+= symbolSize;
	}
	return -1;
}

const char *getSymbolByIndexInPlace(const char* symbolSection, size_t symbolSize, size_t symbolCount,  const char *symbolstrtab, int index)
{
	JS_ASSERT(index<(int)symbolCount);
	Elf32_Sym* elf_sym = (Elf32_Sym*)symbolSection + index;
	return symbolstrtab + elf_sym->st_name;
}
