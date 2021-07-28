/* Script for -z combreloc: combine and sort reloc sections */
OUTPUT_FORMAT("elf64-powerpc-celloslv2", "elf64-powerpc-celloslv2",
	      "elf64-powerpc-celloslv2")
OUTPUT_ARCH(powerpc:common64)
ENTRY(_start)
SEARCH_DIR("=/usr/local/lib"); SEARCH_DIR("=/lib"); SEARCH_DIR("=/usr/lib");
/* Do we need any of these for elf?
   __DYNAMIC = 0;    */
PROVIDE (__stack = 0); PROVIDE (___stack = 0);
PHDRS
{
  /*
   * Lv-2 defines the following attributes in PF_MASKOS filed.
   *
   *  PF_SPU_X     0x00100000    SPU execute (unused)
   *  PF_SPU_W     0x00200000    SPU write
   *  PF_SPU_R     0x00400000    SPU read
   *
   *  PF_RSX_X     0x01000000    RSX execute (unused)
   *  PF_RSX_W     0x02000000    RSX write
   *  PF_RSX_R     0x04000000    RSX read
   *
   */
  /* PPU text and PPU/SPU shared RO data */
  ph_text_srodata	PT_LOAD FILEHDR PHDRS	FLAGS(0x1 | 0x4 | 0x00400000);
  /* PPU/SPU shared RW data */
  ph_sdata		PT_LOAD		FLAGS(0x2 | 0x4 | 0x00200000 | 0x00400000);
  /* PPU local RO data */
  ph_rodata		PT_LOAD		FLAGS(0x4);
  /* PPU local RW data */
  ph_data		PT_LOAD		FLAGS(0x2 | 0x4);
  /* Segment for RSX */
  ph_rsx      PT_LOAD               FLAGS(0x2 | 0x4 | 0x00200000 | 0x00400000 |
                                          0x02000000 | 0x04000000);
  /* Thread local storage */
  ph_tls      PT_TLS                FLAGS(0x4);
  /* Process parameters */
  ph_proc_param    0x60000001       FLAGS(0x0); /* PT_PROC_PARAM (Cell OS Lv-2 Specific segment) */
  ph_proc_prx_param 0x60000002      FLAGS(0x0); /* PT_PROC_PARAM (Cell OS Lv-2 Specific segment) *//* PRX */
}
SECTIONS
{
  /* Read-only sections, merged into text segment: */
  PROVIDE (__executable_start = 0x10000); . = 0x10000 + SIZEOF_HEADERS;
  .interp         : { *(.interp) }: ph_text_srodata
  .hash           : { *(.hash) }
  .dynsym         : { *(.dynsym) }
  .dynstr         : { *(.dynstr) }
  .gnu.version    : { *(.gnu.version) }
  .gnu.version_d  : { *(.gnu.version_d) }
  .gnu.version_r  : { *(.gnu.version_r) }
  .rel.dyn        :
    {
      *(.rel.init)
      *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*)
      *(.rel.fini)
      *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*)
      *(.rel.data.rel.ro*)
      *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*)
      *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*)
      *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*)
      *(.rel.ctors)
      *(.rel.dtors)
      *(.rel.got)
      *(.rel.sdata .rel.sdata.* .rel.gnu.linkonce.s.*)
      *(.rel.sbss .rel.sbss.* .rel.gnu.linkonce.sb.*)
      *(.rel.sdata2 .rel.sdata2.* .rel.gnu.linkonce.s2.*)
      *(.rel.sbss2 .rel.sbss2.* .rel.gnu.linkonce.sb2.*)
      *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*)
    }
  .rela.dyn       :
    {
      *(.rela.init)
      *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
      *(.rela.fini)
      *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
      *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
      *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
      *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*)
      *(.rela.ctors)
      *(.rela.dtors)
      *(.rela.got)
      *(.rela.toc)
      *(.rela.sdata .rela.sdata.* .rela.gnu.linkonce.s.*)
      *(.rela.sbss .rela.sbss.* .rela.gnu.linkonce.sb.*)
      *(.rela.sdata2 .rela.sdata2.* .rela.gnu.linkonce.s2.*)
      *(.rela.sbss2 .rela.sbss2.* .rela.gnu.linkonce.sb2.*)
      *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
    }
  .rel.plt        : { *(.rel.plt) }
  .rela.plt       : { *(.rela.plt) }
  .rela.tocbss	  : { *(.rela.tocbss) }
  .init           :
  {
    KEEP (*(.init))
  } =0x60000000
  .text           :
  {
    *(.sfpr)
    *(.text .stub .text.* .gnu.linkonce.t.*)
    KEEP (*(.text.*personality*))
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.glink)
  } =0x60000000
  .fini           :
  {
    KEEP (*(.fini))
  } =0x60000000
  .sceStub.text   : { *(.sceStub.text) *(SORT(.sceStub.text.*)) }	/* PRX */
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .eh_frame_hdr : { *(.eh_frame_hdr) }
  .eh_frame       : ONLY_IF_RO 
  {
    KEEP (*crtbegin.o(.eh_frame))
    KEEP (*(EXCLUDE_FILE (*crtend.o) .eh_frame))
    KEEP (*(.eh_frame))
  }

  .gcc_except_table   : ONLY_IF_RO { KEEP (*(.gcc_except_table)) *(.gcc_except_table.*) }

  .rodata.sceResident   : { KEEP (*(.rodata.sceResident)) }		 		/* PRX */
  .rodata.sceFNID       : { KEEP (*(.rodata.sceFNID .rodata.sceFNID.*)) }		/* PRX */
  .rodata.sceVNID       : { KEEP (*(.rodata.sceVNID .rodata.sceVNID.*)) }		/* PRX */
  .rodata.sceTNID       : { KEEP (*(.rodata.sceTNID .rodata.sceTNID.*)) }		/* PRX */
  .rodata.sceVStub      : { KEEP (*(.rodata.sceVStub)) KEEP (*(SORT(.rodata.sceVStub.*))) }	/* PRX */
  .lib.ent.top    : { KEEP (*(.lib.ent.top)) }					    	/* PRX */
  .lib.ent        : { KEEP (*(.lib.ent)) }						/* PRX */
  .lib.ent.btm    : { KEEP (*(.lib.ent.btm)) }						/* PRX */
  .lib.stub.top   : { KEEP (*(.lib.stub.top)) }						/* PRX */
  .lib.stub       : { KEEP (*(.lib.stub)) }						/* PRX */
  .lib.stub.btm   : { KEEP (*(.lib.stub.btm)) }						/* PRX */

  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) } 
  .PpuGUID        : { KEEP (*(.PpuGUID)) }
  .rodata1        : { *(.rodata1) }
  .spu_image      : { *(.spu_image .spu_image.*) }

  .sys_proc_param : { KEEP (*(.sys_proc_param)) } :ph_text_srodata :ph_proc_param
  .sys_proc_prx_param : { KEEP (*(.sys_proc_prx_param)) } :ph_text_srodata :ph_proc_prx_param	/* PRX */
  /* Here is a boundary of page to separate the PPU/SPU shared RW region
     from the PPU RO region */
  . = ALIGN(64K);
  .sdata2         : { *(.sdata2 .sdata2.* .gnu.linkonce.s2.*) } : ph_sdata
  .sbss2          : { *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*) }
  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) }
  .gcc_except_table   : ONLY_IF_RW { KEEP (*(.gcc_except_table)) *(.gcc_except_table.*) }
  /* Ensure the __preinit_array_start label is properly aligned.  We
     could instead move the label definition inside the section, but
     the linker would then create the section even if it turns out to
     be empty, which isn't pretty.  */
  . = ALIGN(64 / 8);
  PROVIDE (__preinit_array_start = .);
  .preinit_array     : { KEEP (*(.preinit_array)) }
  PROVIDE (__preinit_array_end = .);
  PROVIDE (__init_array_start = .);
  .init_array     : { KEEP (*(.init_array)) }
  PROVIDE (__init_array_end = .);
  PROVIDE (__fini_array_start = .);
  .fini_array     : { KEEP (*(.fini_array)) }
  PROVIDE (__fini_array_end = .);
  .ctors          :
  {
    PROVIDE (__CTOR_LIST__ = .); PROVIDE (___CTOR_LIST__ = .);
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin*.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    PROVIDE (__CTOR_END__ = .); PROVIDE (___CTOR_END__ = .);
  }
  .dtors          :
  {
    PROVIDE (__DTOR_LIST__ = .); PROVIDE (___DTOR_LIST__ = .);
    KEEP (*crtbegin*.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    PROVIDE (__DTOR_END__ = .); PROVIDE (___DTOR_END__ = .);
  }
  .jcr            : { KEEP (*(.jcr)) }
  .data.rel.ro : { *(.data.rel.ro.local) *(.data.rel.ro*) }
  .dynamic        : { *(.dynamic) }
  .data.sceFStub  : { KEEP (*(.data.sceFStub)) KEEP (*(SORT(.data.sceFStub.*))) }	/* PRX */
 /* .data.sceFStub must before .data */
  .toc1		 ALIGN(8) : { *(.toc1) }
  .opd		 ALIGN(8) : { KEEP (*(.opd)) }
  .got		ALIGN(8) : { *(.got .toc) }
  .toc.sceStub	  : { KEEP (*(.toc.sceStub)) KEEP (*(SORT(.toc.sceStub.*))) }		/* PRX */
  /* Thread Local Storage sections  */
  .tdata	  : { *(.tdata .tdata.* .gnu.linkonce.td.*) } :ph_sdata :ph_tls
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } :ph_sdata :ph_tls

  .data           :
  {
    *(.data .data.* .gnu.linkonce.d.*)
    KEEP (*(.gnu.linkonce.d.*personality*))
    SORT(CONSTRUCTORS)

    /* Add the SNC static contructors. __cplinit is the address of the start
     * of a null terminated array. 
     */
    PROVIDE (__cplinit = .);
    *(__cplinit);
    LONG(0);
    LONG(0);
  } :ph_sdata
  .data1          : { *(.data1) }

  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
  .sdata          :
  {
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }
  _edata = .;
  PROVIDE (edata = .);
  __bss_start = .;
  .tocbss	 ALIGN(8) : { *(.tocbss)}
  .sbss           :
  {
    PROVIDE (__sbss_start = .);
    PROVIDE (___sbss_start = .);
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    PROVIDE (__sbss_end = .);
    PROVIDE (___sbss_end = .);
  }
  .plt            : { *(.plt) }
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.  */
   . = ALIGN(64 / 8);
  }

  /* Here is a boundary of PPU segment to separate PPU local region
     from the PPU/SPU shared region */
  . = ALIGN(256M);
  .ppu_rodata         : { *(.ppu_rodata .ppu_rodata.*) } :ph_rodata

  /* Here is a boundary of page to separate the PPU local RW region
     from the PPU local RO region */
  . = ALIGN(64K);
  .ppu_data       : { *(.ppu_data .ppu_data.*) } :ph_data

  /* Here is a boundary of PPU segment to separate RSX region
     from the PPU local region */
  . = ALIGN(256M);
  .rsx_image      : { *(.rsx_*) } :ph_rsx
  _end = .;
  __end = .;
  PROVIDE (end = .);
  . = ALIGN(8);
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) } :NONE
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* TRANSMETA PGO Begin */
  /* This section is used by simulator-based pgo data gathering.  */
  .pgo_info 0 : { *(.pgo_info) }
  /* TRANSMETA PGO End */
  /DISCARD/ : { *(.lto*) }
  .psp_libgen_markfunc : { KEEP (*(.psp_libgen_markfunc)) }
  .psp_libgen_markvar : { KEEP (*(.psp_libgen_markvar)) }
  .psp_libgen_marktls : { KEEP (*(.psp_libgen_marktls)) }
  /DISCARD/ : { *(.note.GNU-stack) }
  /DISCARD/ : { *(.psp_lib_mark*) *(.rel.psp_lib_mark*) }
}

