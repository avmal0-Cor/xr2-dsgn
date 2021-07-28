/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include <string>

#include "cgbiimpl.hpp"

using std::fill_n;

namespace cgc {
namespace bio {

elf_reader_impl::elf_reader_impl()
{
    ref_count_ = 1;
    initialized_ = false;
    fill_n( reinterpret_cast<char*>( &header_ ), sizeof( header_ ), '\0' );
}

elf_reader_impl::~elf_reader_impl()
{
    if (initialized_)
    {
	// NYI
    }
}

CGBIO_ERROR
elf_reader_impl::load( const char* filename )
{
    // NYI
    return CGBIO_ERROR_NO_ERROR;
}

CGBIO_ERROR
elf_reader_impl::load( std::istream* stream, int start )
{
    // NYI
    return CGBIO_ERROR_NO_ERROR;
}

bool
elf_reader_impl::is_initialized() const
{
    return initialized_;
}

ptrdiff_t
elf_reader_impl::reference() const
{
    return ++ref_count_;
}

ptrdiff_t
elf_reader_impl::release() const
{
	ptrdiff_t ret = --ref_count_;
    if ( 0 == ref_count_ )
	{
        delete this;
    }

    return ret;
}

} // bio namespace
} // cgc namespace
