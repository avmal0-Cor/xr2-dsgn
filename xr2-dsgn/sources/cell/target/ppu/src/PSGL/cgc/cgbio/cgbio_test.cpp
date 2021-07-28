/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

// cgbiotest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cstdio>

#include "cgbio.hpp"

using std::vector;

using cgc::bio::bin_io;
using cgc::bio::elf_writer;
using cgc::bio::nvb_reader;
using cgc::bio::osection;
using cgc::bio::ostring_table;
using cgc::bio::CGBIO_ERROR;
using cgc::bio::oparam_table;

int
main( int argc, char* argv[] )
{
    nvb_reader* nvbr = 0;
    bin_io::instance()->new_nvb_reader( &nvbr );
    CGBIO_ERROR err = nvbr->load( argv[1] );
    printf( "%s: %s\n", argv[0], bin_io::instance()->error_string( err ).c_str());
    printf( "Profile %d, Revision %d, Size %d, Ucode %d\n",
	    nvbr->profile(), nvbr->revision(), nvbr->size(), nvbr->ucode_size() );

    elf_writer* cgbw = 0;
    bin_io::instance()->new_elf_writer( &cgbw );

    printf( "addr %d, half %d cgp %d\n", sizeof(Elf32_Addr), sizeof(Elf32_Half), sizeof(Elf32_cgParameter) );

    cgbw->set_attr( ELFCLASS32, ELFDATA2LSB, EV_CURRENT, ELFOSABI_CGRUNTIME, 1, ET_REL, EM_RSX, 1, 0x12345678 );

    osection* sec = cgbw->add_section( ".text", SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR, 0, 0x10, 0 );
    const char* ucode = nvbr->ucode();
    sec->set_data( ucode, nvbr->ucode_size() );

    osection* strtab = cgbw->add_section( ".strtab", SHT_STRTAB, 0, 0, 1, 0 );
    ostring_table* strpro = 0;
    cgbw->new_section_out( elf_writer::CGBO_STR, strtab, (void**)&strpro );
    osection* ptabsec = cgbw->add_section( ".param", SHT_PROGBITS, 0, 0, 4, sizeof(Elf32_cgParameter) );
    oparam_table* paramtab = 0;
    cgbw->new_section_out( elf_writer::CGBO_PARAM, ptabsec, (void**)&paramtab );

    for (unsigned int ii = 0; ii < nvbr->number_of_params(); ++ii)
    {
		      CGtype type;
		  CGresource res;
		      CGenum var;
			 int rin;
		      string name;
	       vector<float> dv;
	vector<unsigned int> ec;
		      string sem;
		      CGenum dir;
			 int no;
			bool is_referenced;
			bool is_shared;

	nvbr->get_param( ii, type, res, var, rin, name, dv, ec, sem, dir, no, is_referenced, is_shared );
	printf( "%2d: t:%4d, r:%4d, v:%4d, ri:%2d, n:%34s, s:%10s, di:%4d, p:%4d, re:%1d, sh:%1d",
	       ii, type, res, var, rin, name.c_str(), sem.c_str(), dir, no, is_referenced, is_shared );
	Elf32_Word name_index = strpro->add( name.c_str() );
	Elf32_Word sem_index = strpro->add( sem.c_str() );
	for ( int jj = 0; jj < (int)dv.size(); ++jj )
	{
	    printf( ", dv[%2d] %g", jj, dv[jj] );
	}
	for ( int jj = 0; jj < (int)ec.size(); ++jj )
	{
	    printf( ", ec[%2d] %4d", jj, ec[jj] );
	}
	printf( "\n" );
	Elf32_cgParameter param;
	//param.cgp_next = 0;
	//param.cgp_nextleaf = 0;
	//param.cgp_child = 0;
	param.cgp_name = name_index;
	param.cgp_type = type;
	//param.cgp_usertype = 0;
	//param.cgp_arrindex = -1;
	//param.cgp_intrinsic.cgp_semantic = sem_index;
	//param.cgp_intrinsic.cgp_ordinal = no;
	//param.cgp_intrinsic.cgp_resource = rin;
	paramtab->add_entry( param );
    }
    strpro->release();
    strtab->release();
    sec->release();

    osection* fragp = cgbw->add_section( ".info", SHT_PROGBITS, SHF_ALLOC, 0, 0x10, 0 );
    fragp->set_data( (const char*)(nvbr->fragment_program()), sizeof(CgBinaryFragmentProgram) );
    fragp->release();
    cgbw->save( argv[2] );
    nvbr->release();
    cgbw->release();

    return 0;
}
