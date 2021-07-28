/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */
 
#if !defined(CGC_CGBIO_CGBIIMPL_HPP)
#define CGC_CGBIO_CGBIIMPL_HPP 1

#include <iostream>
#include <string>
#include <vector>
#include <cstddef>

// #include <Cg/cg.h>
// #include <Cg/cgBinary.h>

#include "cgbio.hpp"

using std::istream;

namespace cgc {
namespace bio {

class elf_reader_impl : public elf_reader
{
    public:
	elf_reader_impl();
	virtual ~elf_reader_impl();
	virtual CGBIO_ERROR load( const char* filename );
	virtual CGBIO_ERROR load( istream* stream, int start );
	virtual bool	  is_initialized() const;
	virtual ptrdiff_t reference()      const;
	virtual ptrdiff_t release()	   const;

//	virtual int	  get_revision()   const;

    private:
	mutable ptrdiff_t ref_count_;
	istream*	stream_;
	bool		initialized_;
	Elf32_Ehdr	header_;
	STL_NAMESPACE vector<const isection*> sections_;
//	CgBinaryProgram binary_header_;
}; // CGBIImpl
    
} // bio namespace
} // cgc namespace

#endif // CGC_CGBIO_CGBIIMPL_HPP
