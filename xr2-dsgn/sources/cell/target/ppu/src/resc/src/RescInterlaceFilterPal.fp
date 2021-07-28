/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

// uniform variables
uniform float tValue;

uniform sampler2D samplerPalNow;
uniform sampler2D samplerPalPrev;
uniform sampler1D samplerInterlace;

void main
(
	in float2 texcoord0 : TEXCOORD0,
	in float2 texcoord1 : TEXCOORD1,

	out float4 color0 : COLOR0,
	out float4 color1 : COLOR1
)
{
	float y_floor, y_fraction;

	y_fraction = modf((float)texcoord0.y, y_floor);

	const float4 y_diff = {-1.f, 0.f, 1.f, 2.f};

	float4 texel_y = float4(y_floor) + y_diff;

	// table (windowed and normilized) look up.
	float4 g_coeffs = tex1D(samplerInterlace, y_fraction);

	float4x4 color_matrix;
	color_matrix[0] = tex2D(samplerPalNow, float2(texcoord0.x, texel_y[0]));
	color_matrix[1] = tex2D(samplerPalNow, float2(texcoord0.x, texel_y[1]));
	color_matrix[2] = tex2D(samplerPalNow, float2(texcoord0.x, texel_y[2]));
	color_matrix[3] = tex2D(samplerPalNow, float2(texcoord0.x, texel_y[3]));

	float4 colorNow  = mul(g_coeffs, color_matrix);

	float4 colorPrev = tex2D(samplerPalPrev, texcoord1);

	// out to render target 1.
	color1 = colorNow;

	// merging now and prev by given ratio, then out to render target 0.
	color0 = lerp(colorNow, colorPrev, tValue);

}// end of main.

