/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#include "cgbutils.hpp"
#include "nvbiimpl.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include <Cg/cg.h>

namespace cgc {
namespace bio {

nvb_reader_impl::nvb_reader_impl()
{
    ref_count_ = 1;
    offset_ = 0;
    image_ = 0;
    owner_ = false;
    loaded_ = false;
    endianness_ = host_endianness();
    std::fill_n( reinterpret_cast<char*>( &header_ ), sizeof( header_ ), '\0' );
}

nvb_reader_impl::~nvb_reader_impl()
{
    if ( 0 != image_ )
    {
		delete [] image_;
    }
}

ptrdiff_t
nvb_reader_impl::reference() const
{
    return ++ref_count_;
}

ptrdiff_t
nvb_reader_impl::release() const
{
    ptrdiff_t ret = --ref_count_;
    if ( 0 == ref_count_ )
    {
        delete this;
    }
    return ret;
}

CGBIO_ERROR
nvb_reader_impl::loadFromString( const char* source, size_t length)
{
	if ( loaded_ )
	{
		return CGBIO_ERROR_LOADED;
	}
#if defined(JS_USE_STD_STRING) && !(defined(JS_PLATFORM_CELL))
	std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary;
	std::istringstream* iss = new std::istringstream(mode);
	if ( 0 == iss )
	{
		return CGBIO_ERROR_MEMORY;
	}

	std::string sourceString((const char*)source,length);
	iss->str(sourceString);
	if ( !*iss )
	{
		return CGBIO_ERROR_FILEIO;
	}
	strStream_ = true;
	owner_ = true;
	CGBIO_ERROR ret = load( iss, 0 );
#else
	//don't use the stream function, do it directly here
	CGBIO_ERROR ret = CGBIO_ERROR_NO_ERROR;
	while (1)
	{
		if (length < sizeof( header_ ))
		{
			ret = CGBIO_ERROR_FORMAT;
			break;
		}
		memcpy(&header_ ,source,sizeof( header_ ));
		if ( CG_BINARY_FORMAT_REVISION != header_.binaryFormatRevision )
		{
			endianness_ = ( CGBIODATALSB == endianness_ ) ? CGBIODATAMSB : CGBIODATALSB;
			int binaryRevision = convert_endianness( header_.binaryFormatRevision, endianness_ );
			if ( CG_BINARY_FORMAT_REVISION != binaryRevision )
			{
				ret = CGBIO_ERROR_FORMAT;
				break;
			}
		}
		// map the file image into memory.
		size_t sz = length;
		image_ = new char[sz];
		memcpy(image_,source,sz);
		loaded_ = true;
		ret = CGBIO_ERROR_NO_ERROR;
		break;
	}
#endif
	return ret;
}

#ifndef JS_PLATFORM_CELL

CGBIO_ERROR
nvb_reader_impl::load( const char* filename )
{
	if ( loaded_ )
	{
		return CGBIO_ERROR_LOADED;
	}
	std::ifstream* ifs = new std::ifstream;
	if ( 0 == ifs )
	{
		return CGBIO_ERROR_MEMORY;
	}
	ifs->open( filename, std::ios::in | std::ios::binary );
	if ( !*ifs )
	{
		return CGBIO_ERROR_FILEIO;
	}
	CGBIO_ERROR ret = load( ifs, 0 );
	strStream_ = false;
	owner_ = true;
	return ret;
}

CGBIO_ERROR
nvb_reader_impl::load( std::istream* stream, int start, bool owner )
{
    if ( loaded_ )
    {
		return CGBIO_ERROR_LOADED;
    }
    owner_ = owner;
    offset_ = start;
    stream->seekg( offset_ );
    stream->read( reinterpret_cast<char*>( &header_ ), sizeof( header_ ) );
    if ( stream->gcount() != sizeof( header_ ) )
    {
		return CGBIO_ERROR_FORMAT;
    }
    if ( CG_BINARY_FORMAT_REVISION != header_.binaryFormatRevision )
    {
		endianness_ = ( CGBIODATALSB == endianness_ ) ? CGBIODATAMSB : CGBIODATALSB;
		
		int binaryRevision = convert_endianness( header_.binaryFormatRevision, endianness_ );
 		if ( CG_BINARY_FORMAT_REVISION != binaryRevision )
		{
		    return CGBIO_ERROR_FORMAT;
		}
    }
	// map the file image into memory.
	unsigned int sz = size();
	image_ = new char[sz];
	stream->seekg( offset_ );
	stream->read( image_, sz );
	loaded_ = true;

	if (owner_)
	{
		((std::ifstream*) stream)->close();
		delete stream;
	}
	return CGBIO_ERROR_NO_ERROR;
}

#endif

bool
nvb_reader_impl::is_loaded() const
{
    return loaded_;
}

unsigned char
nvb_reader_impl::endianness() const
{
    return endianness_;
}

CGprofile
nvb_reader_impl::profile() const
{
    return (CGprofile) convert_endianness( (unsigned int) header_.profile, endianness() );
}

unsigned int
nvb_reader_impl::revision() const
{
    return convert_endianness( header_.binaryFormatRevision, endianness() );
}

unsigned int
nvb_reader_impl::size() const
{
    return convert_endianness( header_.totalSize, endianness() );
}

unsigned int
nvb_reader_impl::number_of_params() const
{
    return convert_endianness( header_.parameterCount, endianness() );
}

unsigned int
nvb_reader_impl::ucode_size() const
{
    return convert_endianness( header_.ucodeSize, endianness() );
}

const char*
nvb_reader_impl::ucode() const
{
    if ( 0 == image_ || 0 == ucode_size() )
    {
	return 0;
    }
    return ( image_ + convert_endianness( header_.ucode, endianness() ) );
}

const CgBinaryFragmentProgram*
nvb_reader_impl::fragment_program() const
{
    if ( 0 == image_ )
    {
	return 0;
    }
    return reinterpret_cast<CgBinaryFragmentProgram*>( &image_[convert_endianness( header_.program, endianness_ )] );
}

const CgBinaryVertexProgram*
nvb_reader_impl::vertex_program() const
{
    if ( 0 == image_ )
    {
	return 0;
    }
    return reinterpret_cast<CgBinaryVertexProgram*>( &image_[convert_endianness( header_.program, endianness_ )] );
}

CGBIO_ERROR
nvb_reader_impl::get_param_name( unsigned int index, const char **name, bool& is_referenced ) const
{
	if ( index >= number_of_params() )
	{
		return CGBIO_ERROR_INDEX;
	}
	if ( 0 == image_ )
	{
		return CGBIO_ERROR_NO_ERROR;
	}
	const CgBinaryParameter* params = reinterpret_cast<CgBinaryParameter*>( &image_[convert_endianness( header_.parameterArray, endianness_ )] );
	const CgBinaryParameter& pp = params[index];
    is_referenced = convert_endianness( pp.isReferenced, endianness() ) != 0;
	CgBinaryStringOffset nm_offset = convert_endianness( pp.name,endianness() );
	if ( 0 != nm_offset )
	{
		*name = &image_[nm_offset];
	}
	else
		*name = "";
	return CGBIO_ERROR_NO_ERROR;
}

CGBIO_ERROR
nvb_reader_impl::get_param( unsigned int index,
			    	 CGtype& type,
			     CGresource& resource,
				 CGenum& variability,
				    int& resource_index,
				 const char ** name,
		     STL_NAMESPACE vector<float>& default_value,
	      STL_NAMESPACE vector<unsigned int>& embedded_constants,
				 const char ** semantic,
				 CGenum& direction,
				    int& paramno,
				   bool& is_referenced,
				   bool& is_shared ) const
{
    if ( index >= number_of_params() )
    {
	return CGBIO_ERROR_INDEX;
    }
    if ( 0 == image_ )
    {
	return CGBIO_ERROR_NO_ERROR;
    }
    const CgBinaryParameter* params = reinterpret_cast<CgBinaryParameter*>( &image_[convert_endianness( header_.parameterArray, endianness_ )] );
    const CgBinaryParameter& pp = params[index];
    type		= static_cast<CGtype>(		convert_endianness( static_cast<unsigned int>( pp.type ),	endianness() ) );
    resource		= static_cast<CGresource>(	convert_endianness( static_cast<unsigned int>( pp.res ),	endianness() ) );
    variability		= static_cast<CGenum>(		convert_endianness( static_cast<unsigned int>( pp.var ),	endianness() ) );
    resource_index	=				convert_endianness( pp.resIndex,				endianness() );
    direction		= static_cast<CGenum>(		convert_endianness( static_cast<unsigned int>( pp.direction ),	endianness() ) );
    paramno		=				convert_endianness( pp.paramno,					endianness() );
    is_referenced	=				convert_endianness( pp.isReferenced,				endianness() ) != 0;
    is_shared		=				convert_endianness( pp.isShared,				endianness() ) != 0;
    CgBinaryStringOffset		nm_offset =	convert_endianness( pp.name,					endianness() );
    CgBinaryFloatOffset			dv_offset =	convert_endianness( pp.defaultValue,				endianness() );
    CgBinaryEmbeddedConstantOffset	ec_offset =	convert_endianness( pp.embeddedConst,				endianness() );
    CgBinaryStringOffset		sm_offset =	convert_endianness( pp.semantic,				endianness() );
    if ( 0 != nm_offset )
    {
		*name = &image_[nm_offset];
    }
	else
		*name = "";

    if ( 0 != sm_offset )
    {
		*semantic = &image_[sm_offset];
    }
	else
		*semantic = "";
    if ( 0 != dv_offset )
    {
	char *vp = &image_[dv_offset];
	// is this always correct, the defaults come in four?
	for (int ii = 0; ii < 4; ++ii)
	{
/*#ifndef JS_PLATFORM_CELL
		float *fv = static_cast<float*>( vp );
		float f = fv[ii];
		unsigned int tmp = *(unsigned int*)&f;
		tmp = convert_endianness(tmp,endianness());
		default_value.push_back( *(float*)&tmp );
#else*/
		int tmp;
		memcpy(&tmp,vp+4*ii,4);
		tmp = convert_endianness(tmp,endianness());
		float tmp2;
		memcpy(&tmp2,&tmp,4);
		default_value.push_back( tmp2 );
//#endif
	}
    }
    if ( 0 != ec_offset )
    {
	void *vp = &image_[ec_offset];
	CgBinaryEmbeddedConstant& ec = *(static_cast<CgBinaryEmbeddedConstant*>( vp ));
	for (unsigned int ii = 0; ii < convert_endianness( ec.ucodeCount, endianness() ); ++ii)
	{
	    unsigned int offset = convert_endianness( ec.ucodeOffset[ii], endianness() );
	    embedded_constants.push_back( offset );
	}
    }
    return CGBIO_ERROR_NO_ERROR;
}


} // bio namespace
} // cgc namespace
