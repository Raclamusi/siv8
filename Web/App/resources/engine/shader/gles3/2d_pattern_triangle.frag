#version 300 es

//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2026 Ryo Suzuki
//	Copyright (c) 2016-2026 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

precision mediump float;

//
// PSInput
//
in vec4 ColorPMA;

//
//	PSOutput
//
layout(location = 0) out vec4 FragColor;

//
//	Constant Buffer
//
layout(std140) uniform PSConstants2D
{
	vec4 g_patternBackgroundColorMul;
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_sdfOutlineColorPMA;
	vec4 g_sdfShadowColorPMA;
};

layout(std140) uniform PSEffectConstants2D
{
	vec4 g_patternUVTransform[2];
	vec4 g_patternBackgroundColor;
	mat3x3 g_quadWarpInvHomography;
	vec4 g_quadWarpUVTransform;
};

//
//	Siv3D Functions
//
vec4 s3d_shapeColor(const vec4 vertexColorPMA)
{
	return (vertexColorPMA + (g_colorAdd * vertexColorPMA.a));
}

vec4 s3d_premultiplyAlpha(const vec4 color)
{
	return vec4((color.rgb * color.a), color.a);
}

//
//	Functions
//
vec4 Pattern_BackgroundColor()
{
	vec4 colorPMA = s3d_premultiplyAlpha(g_patternBackgroundColor * g_patternBackgroundColorMul);
	return (colorPMA + (g_colorAdd * colorPMA.a));
}

vec2 Pattern_UVTransform(const vec2 uv)
{
	return (g_patternUVTransform[0].zw + (uv.x * g_patternUVTransform[0].xy) + (uv.y * g_patternUVTransform[1].xy));
}

vec2 Pattern_Skew(const vec2 v)
{
	mat2x2 transform = mat2x2(1.0, (1.0 / tan(3.1415926535 / 3.0)), 0.0, (1.0 / sin(3.1415926535 / 3.0)));
	return (v * transform);
}

void main()
{
	vec2 uv = Pattern_UVTransform(gl_FragCoord.xy);
	vec2 fw = (fwidth(uv) * 0.25);

	vec2 s1 = Pattern_Skew(uv + vec2(-fw.x, -fw.y));
	vec2 s2 = Pattern_Skew(uv + vec2(fw.x, fw.y));
	vec2 s3 = Pattern_Skew(uv + vec2(-fw.x, fw.y));
	vec2 s4 = Pattern_Skew(uv + vec2(fw.x, -fw.y));

	vec4 f1 = fract(vec4(s1, s2));
	vec4 f2 = fract(vec4(s3, s4));
	vec4 ss = vec4(step(f1.x, f1.y), step(f1.z, f1.w), step(f2.x, f2.y), step(f2.z, f2.w));
	float c = dot(ss, vec4(0.25));

	vec4 primary = s3d_shapeColor(ColorPMA);
	vec4 background = Pattern_BackgroundColor();

	FragColor = mix(primary, background, c);
}
