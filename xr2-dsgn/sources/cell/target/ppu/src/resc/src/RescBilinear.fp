/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

void main
(
	in float2 texcoord0 : TEXCOORD0,

	uniform sampler2D samplerSrc0,

	out float4 color : COLOR
)
{
	color = tex2D(samplerSrc0, texcoord0);
}

