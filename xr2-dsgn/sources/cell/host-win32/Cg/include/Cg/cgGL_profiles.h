/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 310.001
* Copyright (C) 2006 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

// NVidia style profile defs for RSX, not found in original NV .h files.
CG_PROFILE_MACRO(VertexRSX,	vprsx,		VPRSX,	"sce_vp_rsx",	7003, 1)
CG_PROFILE_MACRO(FragmentRSX,	fprsx,		FPRSX,	"sce_fp_rsx",	7004, 0)

// SCE aliases to NVidia's profile defs.
CG_PROFILE_MACRO(not used,	not used, SCE_VP_TYPEB, "sce_vp_TypeB", 7001, 1)
CG_PROFILE_MACRO(not used,	not used, SCE_FP_TYPEB, "sce_fp_TypeB", 6151, 0)
CG_PROFILE_MACRO(not used,	not used, SCE_VP_RSX,	"sce_vp_rsx",	7003, 1)
CG_PROFILE_MACRO(not used,	not used, SCE_FP_RSX,	"sce_fp_rsx",	7004, 0)

// Deprecated. Not used anywhere in cgc. Needed for compatibility with
// older SDKs.  Must be removed eventually.
CG_PROFILE_MACRO(VertexTypeB,	sce_vp_TypeC,	SCE_VP_TYPEC,	"sce_vp_TypeC", 7005, 1)
CG_PROFILE_MACRO(FragmentTypeB, sce_fp_TypeC,	SCE_FP_TYPEC,	"sce_fp_TypeC", 7006, 1)

#include <Cg/NV/cgGL_profiles.h>
