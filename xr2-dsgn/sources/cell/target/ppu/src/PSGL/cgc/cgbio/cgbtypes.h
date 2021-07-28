/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */

#ifndef CGBTYPES_HEADER
#define CGBTYPES_HEADER

/*//cgp_info defines
//variability
#define CGPV_MASK 0x03
#define CGPV_VARYING 0x00
#define CGPV_UNIFORM 0x01
#define CGPV_CONSTANT 0x02
#define CGPV_MIXED 0x03
//direction
#define CGPD_MASK 0x0C
#define CGPD_IN 0x00
#define CGPD_OUT 0x04
#define CGPD_INOUT 0x08
//is_referenced
#define CGPF_REFERENCED 0x10
#define CGPF_SHARED 0x20

#define CGPF_GLOBAL 0x40

//aggregated type
#define CGP_AGGREGATED 0x80

//internal parameter
#define CGP_INTERNAL 0x100
*/

// parameter structure
typedef struct _Elf32_cgParameter {
	Elf32_Word cgp_name; // index of name in strtab
	Elf32_Word cgp_semantic; // index of semantic string in strtab
	Elf32_Half cgp_default;		// index of default data in const //Reduced to half
	Elf32_Half cgp_reloc;		// index of reloc indices in rel 
	Elf32_Half cgp_resource;		// index of hardware resource assigned
	Elf32_Half cgp_resource_index;		// index of hardware resource assigned
	unsigned char cgp_type;
	Elf32_Half cgp_info;
	unsigned char unused;
} Elf32_cgParameter; //20 bytes

#define CGF_OUTPUTFROMH0 0x01
#define CGF_DEPTHREPLACE 0x02
#define CGF_PIXELKILL 0x04

/*typedef struct _Elf32_cgShader {
	Elf32_Half cgs_profile; // Vertex / Fragment
	Elf32_Half compilerVersion;
	Elf32_Half cgs_param_count;
	Elf32_Word cgs_instruction_count;
	Elf32_Word cgs_attrib_input_mask;
	union {
		struct { //16 bytes
			Elf32_Word cgs_instruction_slot;
			Elf32_Word cgs_register_count;
			Elf32_Word cgs_attrib_output_mask;
			Elf32_Word cgs_user_clip_mask;
		} cgs_vertex_program;
		struct { //12 bytes
			Elf32_Word cgs_partial_tex_type;					
			Elf32_Half cgs_texcoord_input_mask;
			Elf32_Half cgs_texcoord_2d;
			Elf32_Half cgs_texcoord_centroid;
			unsigned char cgs_register_count;
			unsigned char cgs_flags; //combinaison of CGF_OUTPUTFROMH0,CGF_DEPTHREPLACE,CGF_PIXELKILL
		} cgs_fragment_program;
	};
} Elf32_cgShader; //32 bytes*/


#endif
