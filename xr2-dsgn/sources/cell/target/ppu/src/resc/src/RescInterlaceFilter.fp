/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 310.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */

#define SRC_HEIGHT     720.f
#define DST_HEIGHT     480.f

#if 0
# define NUM_TAPS         4
# define EXTRA_WIN_TAPS   1
# define WIN_TAPS         (NUM_TAPS + 2*EXTRA_WIN_TAPS)
# define M_PI             3.1415926535f
# define SEVIRITY         30.f            // 0~100
#endif

// uniform variables
uniform float4 g_coeffs = {0.1f, 0.4f, 0.4f, 0.1f};
uniform float4 g_window = {1.f, 1.f, 1.f, 1.f};
uniform float  g_bandwidth = 0.5f / (SRC_HEIGHT / DST_HEIGHT);   // -> 0.3333

uniform sampler2D samplerSrc0;
uniform sampler1D samplerInterlace;

void main
(
	in float2 texcoord0 : TEXCOORD0,

	out float4 color : COLOR
)
{
	float y_floor, y_fraction;

	y_fraction = modf((float)texcoord0.y, y_floor);

	const float4 y_diff = {-1.f, 0.f, 1.f, 2.f};

	float4 texel_y = float4(y_floor) + y_diff;

// create blackman window
{
#if 0
#if 0
	// in Cg&C (portable) 
	const float x0 = ((1.f * 2.f*M_PI)/5.f) - M_PI;
	const float x1 = ((2.f * 2.f*M_PI)/5.f) - M_PI;
	const float x2 = ((3.f * 2.f*M_PI)/5.f) - M_PI;
	const float x3 = ((4.f * 2.f*M_PI)/5.f) - M_PI;

	const float a0 = 0.42f + (0.50f * cos(x0)) + (0.08f * cos(2.f*x0));
	const float a1 = 0.42f + (0.50f * cos(x1)) + (0.08f * cos(2.f*x1));
	const float a2 = 0.42f + (0.50f * cos(x2)) + (0.08f * cos(2.f*x2));
	const float a3 = 0.42f + (0.50f * cos(x3)) + (0.08f * cos(2.f*x3));

	g_window[0]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.0*a0);
	g_window[1]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.0*a1);
	g_window[2]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.0*a2);
	g_window[3]    = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.0*a3);
#else
	// in Cg
	const float4 x = (float4(1.f, 2.f, 3.f, 4.f) * 2.f*M_PI)/5.f -M_PI;
	const float4 a = 0.42f + (0.50f * cos(x)) + (0.08f * cos(2.f*x));

	g_window       = ((100.f-SEVIRITY)/100.f + SEVIRITY/100.0*a);
#endif
#endif
}

//genCoeff (g_bandwidth, y_fraction);
{
#if 0
	float4 n   = float4(-1.5f, -0.5f, 0.5f, 1.5f) - float4(y_fraction);
	float4 phi = n * 2.f * M_PI * float4(g_bandwidth);

	// this is heavy calculation, should use table lookup.
	g_coeffs = sin(phi) / phi;

	g_coeffs *= g_window;

	// add 4 elements in a vector.
	float coeffs_total = dot(g_coeffs, float4(1.f));
	// the sum of g_coeffs's elements sholud be 1.0f, normalizing.
	g_coeffs /= coeffs_total;
#else
	// table (windowed and normilized) look up.
	g_coeffs = tex1D(samplerInterlace, y_fraction);
#endif
}

//void samplingAndWeightedSum(out float4 color, in float2 texcoord0, in float4 texel_y)
{
#if 1
	float4 c0 = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[0]));
	float4 c1 = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[1]));
	float4 c2 = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[2]));
	float4 c3 = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[3]));
	c0 *= g_coeffs[0];
	c1 *= g_coeffs[1];
	c2 *= g_coeffs[2];
	c3 *= g_coeffs[3];

	color = c0 + c1 + c2 + c3;
#elif 0
	color = 0.f;
	color =  tex2D(samplerSrc0, float2(texcoord0.x, texel_y[0])) * g_coeffs[0];
	color += tex2D(samplerSrc0, float2(texcoord0.x, texel_y[1])) * g_coeffs[1];
	color += tex2D(samplerSrc0, float2(texcoord0.x, texel_y[2])) * g_coeffs[2];
	color += tex2D(samplerSrc0, float2(texcoord0.x, texel_y[3])) * g_coeffs[3];
#else 
	float4x4 color_matrix;
	color_matrix[0] = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[0]));
	color_matrix[1] = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[1]));
	color_matrix[2] = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[2]));
	color_matrix[3] = tex2D(samplerSrc0, float2(texcoord0.x, texel_y[3]));

	color = mul(g_coeffs, color_matrix);
#endif
}

}// end of main.

