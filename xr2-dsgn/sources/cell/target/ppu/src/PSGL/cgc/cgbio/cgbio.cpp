/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include "cgbio.hpp"
#include "cgbiimpl.hpp"
#include "cgboimpl.hpp"
#include "nvbiimpl.hpp"
#include "nvboimpl.hpp"

namespace cgc {
namespace bio {

bin_io* bin_io::instance_ = 0;

bin_io::bin_io()
{
}

bin_io::bin_io( const bin_io& )
{
}

const bin_io*
bin_io::instance()
{
    if ( 0 == instance_ )
    {
	instance_ = new bin_io;
    }
    return instance_;
}

void
bin_io::delete_instance()
{
	if ( 0 != instance_ )
	{
		delete instance_;
		instance_ = 0;
	}
}

CGBIO_ERROR
bin_io::new_elf_reader( elf_reader** obj ) const
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    *obj = new elf_reader_impl;
    if ( 0 == *obj )
    {
	ret = CGBIO_ERROR_MEMORY;
    }
    return ret;
}

CGBIO_ERROR
bin_io::new_elf_writer( elf_writer** obj ) const
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    *obj = new elf_writer_impl;
    if ( 0 == *obj )
    {
	ret = CGBIO_ERROR_MEMORY;
    }
    return ret;
}

const char*
bin_io::error_string( CGBIO_ERROR error ) const
{
    switch ( error )
    {
	case CGBIO_ERROR_NO_ERROR:
	    return "No error";
	case CGBIO_ERROR_LOADED:
	    return "Binary file has been loaded earlier";
	case CGBIO_ERROR_FILEIO:
	    return "File input output error";
	case CGBIO_ERROR_FORMAT:
	    return "File format error";
	case CGBIO_ERROR_INDEX:
	    return "Index is out of range";
	case CGBIO_ERROR_MEMORY:
	    return "Can't allocate memory";
	case CGBIO_ERROR_RELOC:
	    return "Relocation error";
	case CGBIO_ERROR_SYMBOL:
	    return "Symbol error";
	case CGBIO_ERROR_UNKNOWN_TYPE:
	    return "Uknown type";
	default:
	    return "Unknown error";
    }
    return "Unknown error";
}

CGBIO_ERROR
bin_io::new_nvb_reader( nvb_reader** obj ) const
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    *obj = new nvb_reader_impl;
    if ( 0 == *obj )
    {
	ret = CGBIO_ERROR_MEMORY;
    }
    return ret;
}

CGBIO_ERROR
bin_io::new_nvb_writer( nvb_writer** obj ) const
{
    CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
    *obj = new nvb_writer_impl;
    if ( 0 == *obj )
    {
	ret = CGBIO_ERROR_MEMORY;
    }
    return ret;
}


} // bio namespace
} // cgc namespace

#if 0
// cgbio.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    return TRUE;
}
#endif
