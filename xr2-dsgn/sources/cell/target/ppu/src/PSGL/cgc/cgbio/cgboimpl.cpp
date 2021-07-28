/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */
 
#include <algorithm>
#include <iostream>
#include <string>

#include "cgbutils.hpp"
#include "cgboimpl.hpp"


namespace cgc {
namespace bio {

elf_writer_impl::elf_writer_impl()
{
    ref_count_ = 1;
    std::fill_n( reinterpret_cast<char*>( &header_ ), sizeof( header_ ), '\0' );
}

elf_writer_impl::~elf_writer_impl()
{
	STL_NAMESPACE vector<osection_impl*>::iterator it;
    for ( it = sections_.begin(); it != sections_.end(); ++it)
    {
	delete (*it);
    }
}

ptrdiff_t
elf_writer_impl::reference()
{
    return ++ref_count_;
}

ptrdiff_t
elf_writer_impl::release()
{
    ptrdiff_t ret = --ref_count_;
    if ( 0 == ref_count_ )
    {
	delete this;
    }
    return ret;
}

CGBIO_ERROR
elf_writer_impl::save( ofstream& ofs )
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    if ( !ofs )
    {
	return CGBIO_ERROR_FILEIO;
    }
    header_.e_shoff = convert_endianness( (Elf32_Off) sizeof( Elf32_Ehdr ), endianness() );
    header_.e_phnum = convert_endianness( (Elf32_Half)0, endianness() );
    header_.e_shnum = convert_endianness( (Elf32_Half)num_of_sections(), endianness() );

    streampos header  = convert_endianness( header_.e_shoff, endianness() );
    for ( unsigned int ii = 0; ii < sections_.size(); ++ii )
    {
	sections_[ii]->save( ofs, header, section_offset( ii ) );
	header += sizeof( Elf32_Shdr );
    }

    ofs.seekp( 0 );
    ofs.write( reinterpret_cast<const char*>( &header_ ), sizeof( Elf32_Ehdr ) );
    ofs.close();

    return ret;
}

CGBIO_ERROR
elf_writer_impl::save( const char* filename )
{
    ofstream ofs( filename, std::ios::out | std::ios::binary );
    return save( ofs );
}

CGBIO_ERROR
elf_writer_impl::set_attr( const unsigned char file_class,
			const unsigned char endianness,
			const unsigned char ELFversion,
			const unsigned char abi,
			const unsigned char ABIversion,
			const Elf32_Half type,
			const Elf32_Half machine,
			const Elf32_Word version,
			const Elf32_Word flags )
{
    header_.e_ident[EI_MAG0]    = ELFMAG0;
    header_.e_ident[EI_MAG1]    = ELFMAG1;
    header_.e_ident[EI_MAG2]    = ELFMAG2;
    header_.e_ident[EI_MAG3]    = ELFMAG3;
    header_.e_ident[EI_CLASS]   = file_class;
    header_.e_ident[EI_DATA]    = endianness;
    header_.e_ident[EI_VERSION] = ELFversion;
    header_.e_ident[EI_OSABI]	= abi;
    header_.e_ident[EI_ABIVERSION]	= ABIversion;

    header_.e_type    = convert_endianness( type,    endianness );
    header_.e_machine = convert_endianness( machine, endianness );
    header_.e_version = convert_endianness( version, endianness );
    header_.e_flags   = convert_endianness( flags,   endianness );

    header_.e_ehsize    = convert_endianness( (Elf32_Half)(sizeof( header_ )),    endianness );
    header_.e_phentsize = convert_endianness( (Elf32_Half)(sizeof( Elf32_Phdr )), endianness );
    header_.e_shentsize = convert_endianness( (Elf32_Half)(sizeof( Elf32_Shdr )), endianness );
    header_.e_shstrndx  = convert_endianness( (Elf32_Half)1, endianness );

    // Create empty and section header string table sections
    osection_impl* sec0 = new osection_impl( 0, this, "", 0, 0, 0, 0, 0 );
    sections_.push_back( sec0 );
    sec0->set_name_index( 0 );
    
	
	osection_impl* shstrtab = new osection_impl( 1, this, ".shstrtab", SHT_STRTAB, 0, 0, 0, 0 );
    sections_.push_back( shstrtab );

    // Add the name to the section header string table
    ostring_table* strtbl = 0;
    void* vp = strtbl;
    if ( CGBIO_ERROR_NO_ERROR == new_section_out( CGBO_STR, shstrtab, &vp ) )
    {
		strtbl = static_cast<ostring_table*>( vp );
        Elf32_Word index = strtbl->add(shstrtab->name());
        shstrtab->set_name_index( index );
        strtbl->release();
    }
    return CGBIO_ERROR_NO_ERROR;
}

Elf32_Addr
elf_writer_impl::get_entry() const
{
    return convert_endianness( header_.e_entry, endianness() );
}

CGBIO_ERROR
elf_writer_impl::set_entry( Elf32_Addr entry )
{
    header_.e_entry = convert_endianness( entry, endianness() );
    return CGBIO_ERROR_NO_ERROR;
}

unsigned char
elf_writer_impl::endianness() const
{
    return header_.e_ident[EI_DATA];
}

Elf32_Half
elf_writer_impl::num_of_sections() const
{
    return (Elf32_Half)sections_.size();
}

osection*
elf_writer_impl::get_section( Elf32_Half index ) const
{
    osection* ret = 0;
    if ( index < num_of_sections() )
    {
        ret = sections_[index];
        ret->reference();
    }
    return ret;
}

osection*
elf_writer_impl::get_section( const char* name ) const
{
    osection* ret = 0;

    STL_NAMESPACE vector<osection_impl*>::const_iterator it;
    for ( it = sections_.begin(); it != sections_.end(); ++it )
    {
        if ( (*it)->name() == name )
	{
            ret = *it;
            ret->reference();
            break;
        }
    }
    return ret;
}

osection*
elf_writer_impl::add_section( const char* name,
			   Elf32_Word type,
			   Elf32_Word flags,
			   Elf32_Word info,
			   Elf32_Word align,
			   Elf32_Word esize )
{
    osection_impl* sec = new osection_impl( (Elf32_Half)sections_.size(), this, name, type, flags, info, align, esize );
    if ( 0 == sec )
    {
	return sec;
    }
    sec->reference();
    sections_.push_back( sec );
    osection* shstrtab = get_section( 1 );
    ostring_table* strtbl;
    void* vp;
    if ( CGBIO_ERROR_NO_ERROR == new_section_out( CGBO_STR, shstrtab, &vp ) )
    {
	strtbl = static_cast<ostring_table*>( vp );
	Elf32_Word index = strtbl->add( name );
	sec->set_name_index( index );
	strtbl->release();
    }
    shstrtab->release();
    return sec;
}

streampos
elf_writer_impl::section_offset( Elf32_Half index ) const
{
    streampos ret = sizeof( Elf32_Ehdr ) + sizeof( Elf32_Shdr ) * num_of_sections();
    Elf32_Half size = (Elf32_Half)sections_.size();
    Elf32_Word align = 0;
    for ( Elf32_Half ii = 0; ii < size && ii < index; ++ii )
    {
        if ( sections_[ii]->type() != SHT_NOBITS && sections_[ii]->type() != SHT_NULL )
	{
            align = sections_[ii]->addralign();
            if ( align > 1 && ret % align != 0 )
	    {
                ret += align - ret % align;
            }
            ret += sections_[ii]->size();
        }
    }
    if ( sections_[index]->type() != SHT_NOBITS && sections_[index]->type() != SHT_NULL )
    {
        align = sections_[index]->addralign();
        if ( align > 1 && ret % align != 0 )
	{
            ret += align - ret % align;
        }
    }
    return ret;
}

CGBIO_ERROR
elf_writer_impl::new_section_out( PRODUCER type, osection* sec, void** obj ) const
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;

    switch ( type )
    {
	case CGBO_STR:
	    *obj = new ostring_table_impl( const_cast<elf_writer_impl*>( this ), sec );
	    break;
	case CGBO_SYM:
	    *obj = new osymbol_table_impl( const_cast<elf_writer_impl*>( this ), sec );
	    break;
	case CGBO_REL:
	    *obj = new orelocation_table_impl( const_cast<elf_writer_impl*>( this ), sec );
	    break;
	case CGBO_PARAM:
	    *obj = new oparam_table_impl( const_cast<elf_writer_impl*>( this ), sec );
	    break;
	case CGBO_CONST:
		*obj = new oconst_table_impl( const_cast<elf_writer_impl*>( this ), sec );
		break;
	default:
	    ret  = CGBIO_ERROR_UNKNOWN_TYPE;
	    obj = 0;
    }
    return ret;
}


osection_impl::osection_impl( Elf32_Half index,
			      elf_writer* cgbo,
			      const char* name,
			      Elf32_Word type,
			      Elf32_Word flags,
			      Elf32_Word info,
			      Elf32_Word align,
			      Elf32_Word esize ) :
    index_( index ),
    cgbo_( cgbo ),
    data_( 0 )
{
    strncpy(name_, name,sizeof(name_));
	name_[sizeof(name_)-1] = '\0';
	std::fill_n( reinterpret_cast<char*>( &sh_ ), sizeof( sh_ ), '\0' );
    sh_.sh_type      = convert_endianness( type, cgbo_->endianness() );
    sh_.sh_flags     = convert_endianness( flags, cgbo_->endianness() );
    sh_.sh_info      = convert_endianness( info, cgbo_->endianness() );
    sh_.sh_addralign = convert_endianness( align, cgbo_->endianness() );
    sh_.sh_entsize   = convert_endianness( esize, cgbo_->endianness() );
}

osection_impl::~osection_impl()
{
    delete [] data_;
}

ptrdiff_t
osection_impl::reference()
{
    return cgbo_->reference();
}

ptrdiff_t
osection_impl::release()
{
    return cgbo_->release();
}

CGBIO_ERROR
osection_impl::save(ofstream& of, streampos header, streampos data)
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;

    if ( 0 != index() && SHT_NOBITS != type() )
    {
		sh_.sh_offset = convert_endianness( (Elf32_Off)data, cgbo_->endianness() );
    }
    of.seekp( header );
    of.write( reinterpret_cast<const char*>( &sh_ ), sizeof( Elf32_Shdr ) );
    if ( SHT_NOBITS != type() )
    {
        of.seekp( data );
        of.write( get_data(), size() );
    }
    return ret;
}

Elf32_Half
osection_impl::index() const
{
    return index_;
}

const char *
osection_impl::name() const
{
    return name_;
}

Elf32_Word
osection_impl::type() const
{
    return convert_endianness( sh_.sh_type, cgbo_->endianness() );
}

Elf32_Word
osection_impl::flags() const
{
    return convert_endianness( sh_.sh_flags, cgbo_->endianness() );
}

Elf32_Word
osection_impl::info() const
{
    return convert_endianness( sh_.sh_info, cgbo_->endianness() );
}

Elf32_Word
osection_impl::addralign() const
{
    return convert_endianness( sh_.sh_addralign, cgbo_->endianness() );
}

Elf32_Word
osection_impl::entsize() const
{
    return convert_endianness( sh_.sh_entsize, cgbo_->endianness() );
}

Elf32_Word
osection_impl::size() const
{
    return convert_endianness( sh_.sh_size, cgbo_->endianness() );
}

Elf32_Word
osection_impl::get_name_index() const
{
    return convert_endianness( sh_.sh_name, cgbo_->endianness() );
}

void
osection_impl::set_name_index( Elf32_Word index )
{
    sh_.sh_name = convert_endianness( index, cgbo_->endianness() );
}

Elf32_Addr
osection_impl::get_address() const
{
    return convert_endianness( sh_.sh_addr, cgbo_->endianness() );
}

void
osection_impl::set_address( Elf32_Addr addr )
{
    sh_.sh_addr = convert_endianness( addr, cgbo_->endianness() );
}

Elf32_Word
osection_impl::get_link() const
{
    return convert_endianness( sh_.sh_link, cgbo_->endianness() );
}

void
osection_impl::set_link( Elf32_Word link )
{
    sh_.sh_link = convert_endianness( link, cgbo_->endianness() );
}

char*
osection_impl::get_data() const
{
    return data_;
}

CGBIO_ERROR
osection_impl::set_data( const char* data, const Elf32_Word size )
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    sh_.sh_size = convert_endianness( size, cgbo_->endianness() );

    if ( SHT_NOBITS == type() )
    {
	return ret;
    }
    delete [] data_;
    data_ = new char[size];
    if ( 0 != size )
    {
	if ( 0 == data_ )
	{
	    sh_.sh_size = 0;
	    ret = CGBIO_ERROR_MEMORY;
	}
	else if ( 0 != data )
	{
	    std::copy( data, data + size, data_ );
	}
    }

    return ret;
}

CGBIO_ERROR
osection_impl::add_data( const char* data, const Elf32_Word size )
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;

    char* new_data = new char[ osection_impl::size() + size];
    if ( 0 == new_data )
    {
	ret = CGBIO_ERROR_MEMORY;
    }
    else
    {
	std::copy( data_, data_ + osection_impl::size(), new_data );
	std::copy( data, data + size, new_data + osection_impl::size() );
	delete [] data_;
	data_ = new_data;
	sh_.sh_size = convert_endianness(osection_impl::size()+size, cgbo_->endianness() );
    }

    return ret;
}

ostring_table_impl::ostring_table_impl( elf_writer* cgbo, osection* section )
    : ref_count_( 1 )
    , cgbo_( cgbo )
    , section_( section )
{
    if ( 0 != section->get_data() && 0 != section->size() )
    {
        data_.insert(data_.end(), section->get_data(),section->get_data() + section->size());
		//data_.append( section->get_data(), section->size() );
    }
    cgbo_->reference();
    section_->reference();
}

ostring_table_impl::~ostring_table_impl()
{
}

ptrdiff_t
ostring_table_impl::reference()
{
    cgbo_->reference();
    section_->reference();
    return ++ref_count_;
}

ptrdiff_t
ostring_table_impl::release()
{
    ptrdiff_t ret = --ref_count_;
    elf_writer* cgbo = cgbo_;
    osection* sec = section_;

    if ( 0 == ref_count_ )
    {
        section_->set_data( &data_[0], (Elf32_Word)data_.size() );
        delete this;
    }
    sec->release();
    cgbo->release();
    return ret;
}

const char*
ostring_table_impl::get( Elf32_Word index ) const
{
    if ( index < data_.size() )
    {
        const char* data = &data_[0];
        if ( 0 != data )
	{
            return data + index;
        }
    }
    return 0;
}

//search the string table for the first occurence of str
Elf32_Word
ostring_table_impl::find( const char* str  ) const
{
	const char* data = &data_[0];
	size_t size = data_.size();
	const char *end = data + size;

	size_t length = strlen(str);
	if (length+1 > size)
		return 0;
	data += length;

	const char *p = (char*)memchr(data,'\0',end-data);
	while (p && (end-data)>0)
	{
		if (!memcmp(p - length, str, length))
		{
#ifdef MSVC
#pragma warning( push )
#pragma warning ( disable : 4311 )
#endif
			//found
			return (Elf32_Word)(p - length - &data_[0]);
#ifdef MSVC
#pragma warning ( pop )
#endif
		}
		data = p+1;	
		p = (char*)memchr(data,'\0',end-data);
	}
	return 0;
}

Elf32_Word
ostring_table_impl::addUnique( const char* str )
{
	if ( data_.size() == 0 )
		data_.push_back('\0');
	Elf32_Word ret = find(str);
	if (ret == 0 && str[0] != '\0')
		ret = add(str);
	return ret;
}

// Adds STR string at the end of the string table and returns the
// index of the new string.
Elf32_Word
ostring_table_impl::add( const char* str )
{
    Elf32_Word ret = (Elf32_Word)data_.size();

    if ( data_.size() == 0 )
    {
        data_.push_back('\0');
        ret = 1;
    }

	data_.insert( data_.end(), str ,str + strlen(str) + 1);
	
    return ret;
}

oconst_table_impl::oconst_table_impl( elf_writer* cgbo, osection* section )
: ref_count_( 1 )
, cgbo_( cgbo )
, section_( section )
{
	//if there's any data already set on the section
	if ( 0 != section->get_data() && 0 != section->size() )
	{
		size_t count = section->size()/sizeof(Elf32_Word);
		data_.resize(count);
		memcpy(&data_[0],section->get_data(),count*sizeof(Elf32_Word));
	}
	cgbo_->reference();
	section_->reference();
}

oconst_table_impl::~oconst_table_impl()
{
}

ptrdiff_t
oconst_table_impl::reference()
{
	cgbo_->reference();
	section_->reference();
	return ++ref_count_;
}

ptrdiff_t
oconst_table_impl::release()
{
	ptrdiff_t ret = --ref_count_;
	elf_writer* cgbo = cgbo_;
	osection* sec = section_;

	if ( 0 == ref_count_ )
	{
		//now that we are done with the section implementation, we need to save the data in the section
		//---> it means we are making an unecessary copy here, we could keep the buffer around for a little bit
		//longer instead of recopying the data in the parent section.
		section_->set_data( (const char*)&data_[0], (Elf32_Word)(data_.size()*sizeof(Elf32_Word)) );
		delete this;
	}
	sec->release();
	cgbo->release();
	return ret;
}

const Elf32_Word*
oconst_table_impl::get( Elf32_Word index ) const
{
	if ( index < data_.size() )
	{
		return &data_[index];
	}
	return NULL;
}

// Adds the data at the end of the table and returns the
// index of the beginning of the new data, the user keeps track of the size he passed
//( in the type of the parameter for instance ), count is in words not in bytes
Elf32_Word
oconst_table_impl::add( const Elf32_Word *data, Elf32_Word count)
{
	Elf32_Word ret = (Elf32_Word)data_.size();
	data_.resize(ret+count);
	memcpy(&data_[ret],data,count*sizeof(Elf32_Word));
	return ret;
}

osymbol_table_impl::osymbol_table_impl( elf_writer* cgbo, osection* sec ) :
    ref_count_( 1 ), cgbo_( cgbo ), section_( sec )
{
    cgbo_->reference();
    section_->reference();
    if ( 0 == section_->size() )
    {
        Elf32_Sym entry;
        entry.st_name  = 0;
        entry.st_value = 0;
        entry.st_size  = 0;
        entry.st_info  = 0;
        entry.st_other = 0;
        entry.st_shndx = 0;
	CGBIO_ERROR err = section_->add_data( reinterpret_cast<char*>( &entry ), sizeof( entry ) );
	if ( CGBIO_ERROR_NO_ERROR != err )
	{
	    ;
	}
    }
}

osymbol_table_impl::~osymbol_table_impl()
{
}

ptrdiff_t
osymbol_table_impl::reference()
{
    cgbo_->reference();
    section_->reference();
    return ++ref_count_;
}

ptrdiff_t
osymbol_table_impl::release()
{
    ptrdiff_t ret = --ref_count_;
    elf_writer* cgbo = cgbo_;
    osection* sec = section_;
    if ( 0 == ref_count_ )
    {
        delete this;
    }
    sec->release();
    cgbo->release();
    return ret;
}

Elf32_Word
osymbol_table_impl::add_entry( Elf32_Word	name,
			       Elf32_Addr	value,
			       Elf32_Word	size,
			       unsigned char	info,
			       unsigned char	other,
			       Elf32_Half	shndx )
{
    Elf32_Sym entry;
    entry.st_name  = convert_endianness( name, cgbo_->endianness() );
    entry.st_value = convert_endianness( value, cgbo_->endianness() );
    entry.st_size  = convert_endianness( size, cgbo_->endianness() );;
    entry.st_info  = info;
    entry.st_other = other;
    entry.st_shndx = convert_endianness( shndx, cgbo_->endianness() );
    CGBIO_ERROR err = section_->add_data( reinterpret_cast<char*>( &entry ), sizeof( entry ) );
    if ( CGBIO_ERROR_NO_ERROR != err )
    {
	;
    }
    Elf32_Word ret = section_->size() / sizeof(Elf32_Sym) - 1;
    return ret;
}

Elf32_Word
osymbol_table_impl::add_entry( Elf32_Word	name,
			       Elf32_Addr	value,
			       Elf32_Word	size,
			       unsigned char	bind,
			       unsigned char	type,
			       unsigned char	other,
			       Elf32_Half	shndx )
{
    return 0;
}

Elf32_Word
osymbol_table_impl::add_entry( ostring_table*	strtab,
			       const char*	str,
			       Elf32_Addr	value,
			       Elf32_Word	size,
			       unsigned char	info,
			       unsigned char	other,
			       Elf32_Half	shndx )
{
    return 0;
}

Elf32_Word
osymbol_table_impl::add_entry( ostring_table*	strtab,
			       const char*	str,
			       Elf32_Addr	value,
			       Elf32_Word	size,
			       unsigned char	bind,
			       unsigned char	type,
			       unsigned char	other,
			       Elf32_Half	shndx )
{
    return 0;
}


orelocation_table_impl::orelocation_table_impl( elf_writer* cgbo, osection* sec )
{
}

orelocation_table_impl::~orelocation_table_impl()
{
}

ptrdiff_t
orelocation_table_impl::reference()
{
    return 0;
}

ptrdiff_t
orelocation_table_impl::release()
{
    return 0;
}

CGBIO_ERROR
orelocation_table_impl::add_entry( Elf32_Addr offset,
				   Elf32_Word info )
{
    return CGBIO_ERROR_NO_ERROR;
}

CGBIO_ERROR
orelocation_table_impl::add_entry( Elf32_Addr offset,
				   Elf32_Word symbol,
				   unsigned char type )
{
    return CGBIO_ERROR_NO_ERROR;
}

CGBIO_ERROR
orelocation_table_impl::add_entry( Elf32_Addr offset,
				   Elf32_Word info,
				   Elf32_Sword addend )
{
    return CGBIO_ERROR_NO_ERROR;
}


CGBIO_ERROR
orelocation_table_impl::add_entry( Elf32_Addr offset,
				   Elf32_Word symbol,
				   unsigned char type,
				   Elf32_Sword addend )
{
    return CGBIO_ERROR_NO_ERROR;
}


CGBIO_ERROR
orelocation_table_impl::add_entry( ostring_table* pStrWriter,
				   const char* str,
				   osymbol_table* pSymWriter,
				   Elf32_Addr value,
				   Elf32_Word size,
				   unsigned char symInfo,
				   unsigned char other,
				   Elf32_Half shndx,
				   Elf32_Addr offset,
				   unsigned char type )
{
    return CGBIO_ERROR_NO_ERROR;
}


CGBIO_ERROR
orelocation_table_impl::add_entry( ostring_table* pStrWriter,
				   const char* str,
				   osymbol_table* pSymWriter,
				   Elf32_Addr value,
				   Elf32_Word size,
				   unsigned char symInfo,
				   unsigned char other,
				   Elf32_Half shndx,
				   Elf32_Addr offset,
				   unsigned char type,
				   Elf32_Sword addend )
{
    return CGBIO_ERROR_NO_ERROR;
}


oparam_table_impl::oparam_table_impl( elf_writer* cgbo, osection* sec ) :
    ref_count_( 1 ), cgbo_( cgbo ), section_( sec )
{
    cgbo_->reference();
    section_->reference();
}

oparam_table_impl::~oparam_table_impl()
{
}

ptrdiff_t
oparam_table_impl::reference()
{
    cgbo_->reference();
    section_->reference();
    return ++ref_count_;
}

ptrdiff_t
oparam_table_impl::release()
{
    ptrdiff_t ret = --ref_count_;
    elf_writer* cgbo = cgbo_;
    osection* sec = section_;
    if ( 0 == ref_count_ )
    {
	delete this;
    }
    sec->release();
    cgbo->release();
    return ret;
}

#if 0
CGBIO_ERROR
oparam_table_impl::add_entry( Elf32_Word next,
			      Elf32_Word nextleaf,
			      Elf32_Word child,
			      const char* name,
			      Elf32_Half type,
			      Elf32_Half usertype,
			      Elf32_Shalf arrindex,
			      unsigned char vartype,
			      Elf32_Word ordinal,
			      Elf32_Half resource,
			      ostring_table* strtbl,
			      osymbol_table* symtbl )
{
    Elf32_Word stridx = strtbl->add( name );
    Elf32_Word symidx = symtbl->add_entry( stridx, value, size, sym_info, other, shidx );
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    Elf32_cgparameter param;
    Elf32_Word  index = section_->size() / sizeof( Elf32_cgparameter );
    //printf ( "adding parameter %d\n", index );
    ret = section_->add_data( param, sizeof( param ) );
    return ret;
}
#endif

CGBIO_ERROR
oparam_table_impl::add_entry( Elf32_cgParameter& parameter )
{
    void* vp = &parameter;
    return section_->add_data( static_cast<char*>( vp ), sizeof( parameter ) );
}


} // bio namespace
} // cgc namespace
