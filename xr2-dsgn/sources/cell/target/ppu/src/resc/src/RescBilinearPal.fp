/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

uniform float tValue;// = 0.47;

void main
(
	in float2 texcoord0 : TEXCOORD0,
	in float2 texcoord1 : TEXCOORD1,
	
	uniform sampler2D samplerPalNow,
	uniform sampler2D samplerPalPrev,

	out float4 color0 : COLOR0,
	out float4 color1 : COLOR1
)
{
	float4 colorNow   = tex2D(samplerPalNow,  texcoord0);
	float4 colorPrev  = tex2D(samplerPalPrev, texcoord1);

	// out to render target 1.
	color1 = colorNow;

	// merging now and prev by given ratio, then out to render target 0.
	color0 = lerp(colorNow, colorPrev, tValue);
}
