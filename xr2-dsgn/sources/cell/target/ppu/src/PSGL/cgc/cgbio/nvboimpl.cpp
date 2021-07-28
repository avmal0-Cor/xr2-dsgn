/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */
 
#include <algorithm>
#include <iostream>
#include <string>

#include "cgbutils.hpp"
#include "nvboimpl.hpp"

using std::copy;
using std::fill_n;

namespace cgc {
namespace bio {

nvb_writer_impl::nvb_writer_impl()
{
    ref_count_ = 1;
    fill_n( reinterpret_cast<char*>( &header_ ), sizeof( header_ ), '\0' );
}

nvb_writer_impl::~nvb_writer_impl()
{
#if 0
    std::vector<osection_impl*>::iterator it;
    for ( it = sections_.begin(); it != sections_.end(); ++it)
    {
	delete (*it);
    }
#endif
}

ptrdiff_t
nvb_writer_impl::reference()
{
    return ++ref_count_;
}

ptrdiff_t
nvb_writer_impl::release()
{
    ptrdiff_t ret = --ref_count_;
    if ( 0 == ref_count_ )
    {
	delete this;
    }
    return ret;
}

CGBIO_ERROR
nvb_writer_impl::save( ofstream& ofs )
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    if ( !ofs )
    {
	return CGBIO_ERROR_FILEIO;
    }
    header_.e_shoff = convert_endianness( (Elf32_Off)sizeof( Elf32_Ehdr ), endianness() );
    header_.e_phnum = convert_endianness( 0, endianness() );
#if 0
    streampos header  = convert_endianness( header_.e_shoff, endianness() );
    for ( unsigned int ii = 0; ii < sections_.size(); ++ii )
    {
	sections_[ii]->save( ofs, header, section_offset( ii ) );
	header += sizeof( Elf32_Shdr );
    }
#endif
    ofs.seekp( 0 );
    ofs.write( reinterpret_cast<const char*>( &header_ ), sizeof( Elf32_Ehdr ) );
    ofs.close();

    return ret;
}

CGBIO_ERROR
nvb_writer_impl::save( const char* filename )
{
    ofstream ofs( filename, std::ios::out | std::ios::binary );
    return save( ofs );
}

CGBIO_ERROR
nvb_writer_impl::set_attr( const unsigned char file_class,
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

    header_.e_ehsize    = convert_endianness( (Elf32_Half)sizeof( header_ ),    endianness );
    header_.e_phentsize = convert_endianness( (Elf32_Half)sizeof( Elf32_Phdr ), endianness );
    header_.e_shentsize = convert_endianness( (Elf32_Half)sizeof( Elf32_Shdr ), endianness );
    header_.e_shstrndx  = convert_endianness( 1, endianness );
#if 0
    // Create empty and section header string table sections
    osection_impl* sec0 = new osection_impl( 0, this, "", 0, 0, 0, 0, 0 );
    sections_.push_back( sec0 );
    sec0->set_name_index( 0 );
    osection_impl* shstrtab = new osection_impl( 1, this, ".shstrtab", SHT_STRTAB, 0, 0, 0, 0 );
    sections_.push_back( shstrtab );

    // Add the name to the section header string table
    ostring_table* strtbl = 0;
    if ( CGBIO_ERROR_NO_ERROR == new_section_out( CGBO_STR, shstrtab, reinterpret_cast<void**>( &strtbl ) ) )
    {
	const char* sec_name = shstrtab->name().c_str();
        Elf32_Word index = strtbl->add( sec_name );
        shstrtab->set_name_index( index );
        strtbl->release();
    }
#endif
    return CGBIO_ERROR_NO_ERROR;
}

Elf32_Addr
nvb_writer_impl::get_entry() const
{
    return convert_endianness( header_.e_entry, endianness() );
}

CGBIO_ERROR
nvb_writer_impl::set_entry( Elf32_Addr entry )
{
    header_.e_entry = convert_endianness( entry, endianness() );
    return CGBIO_ERROR_NO_ERROR;
}

unsigned char
nvb_writer_impl::endianness() const
{
    return header_.e_ident[EI_DATA];
}

oparam_array_impl::oparam_array_impl( nvb_writer* cgbo ) :
    ref_count_( 1 ), cgbo_( cgbo )
{
    cgbo_->reference();
}

oparam_array_impl::~oparam_array_impl()
{
}

ptrdiff_t
oparam_array_impl::reference()
{
    cgbo_->reference();
    return ++ref_count_;
}

ptrdiff_t
oparam_array_impl::release()
{
    ptrdiff_t ret = --ref_count_;
    nvb_writer* cgbo = cgbo_;
    if ( 0 == ref_count_ )
    {
	delete this;
    }
    cgbo->release();
    return ret;
}

CGBIO_ERROR
oparam_array_impl::add_entry()
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    return ret;
}


} // bio namespace
} // cgc namespace
