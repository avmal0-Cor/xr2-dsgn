////////////////////////////////////////////////////////////////////////////
//	Created		: 20.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef CAPSULE_LIGHT_EFFECT_H_INCLUDED
#define CAPSULE_LIGHT_EFFECT_H_INCLUDED

#include <xray/render/core/effect_descriptor.h>

namespace xray {
namespace render {

class capsule_light_effect : public effect_descriptor {
private:
	enum
	{
		front_side_pass,
		back_side_pass,
	};
	virtual	void compile	( effect_compiler& compiler, custom_config_value const& config )
	{
		XRAY_UNREFERENCED_PARAMETER	( config);

		compiler.begin_technique(); // front side of capsule
			compiler.begin_pass	( "light", NULL, "capsule_light_accumulator", shader_configuration(), NULL);
				compiler.set_depth			( true, false, D3D_COMPARISON_LESS_EQUAL);
				//compiler.set_stencil		( true, 0x81, 0x81, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_DECR, D3D_STENCIL_OP_DECR);
				compiler.set_stencil		( true, 0xff, check_all_geometry_type, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_INVERT);
				compiler.set_alpha_blend	( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD);
				compiler.set_cull_mode		( D3D_CULL_BACK);
				compiler.set_texture		( "t_position",    r2_rt_p);
				compiler.set_texture		( "t_normal",      r2_rt_n);
				compiler.set_texture		( "t_material",    r2_material);
				compiler.set_texture		( "t_target_ex",   r2_rt_p_ex);
				compiler.set_texture		( "t_emissive",    r2_rt_emissive);
				compiler.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse);
				compiler.set_texture		( "t_decals_normal", r2_rt_decals_normal);

				compiler.set_texture		( "t_tangents", r2_rt_tangents);
			compiler.end_pass();
		compiler.end_technique();
		
		compiler.begin_technique(); // back side of capsule
			compiler.begin_pass	( "light", NULL, "capsule_light_accumulator", shader_configuration(), NULL);
				compiler.set_depth			( false, false, D3D_COMPARISON_LESS_EQUAL);
				//compiler.set_stencil		( true, 0x81, 0x81, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_DECR, D3D_STENCIL_OP_KEEP);
				compiler.set_stencil		( true, 0xff, check_all_geometry_type, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_KEEP);
				compiler.set_alpha_blend	( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD);
				compiler.set_cull_mode		( D3D_CULL_FRONT);
				compiler.set_texture		( "t_position",    r2_rt_p);
				compiler.set_texture		( "t_normal",      r2_rt_n);
				compiler.set_texture		( "t_material",    r2_material);
				compiler.set_texture		( "t_target_ex",   r2_rt_p_ex);
				compiler.set_texture		( "t_emissive",    r2_rt_emissive);
				compiler.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse);
				compiler.set_texture		( "t_decals_normal", r2_rt_decals_normal);

				compiler.set_texture		( "t_tangents", r2_rt_tangents);
			compiler.end_pass();
		compiler.end_technique();
	}
}; // class capsule_light_effect

} // namespace render
} // namespace xray

#endif // #ifndef CAPSULE_LIGHT_EFFECT_H_INCLUDED