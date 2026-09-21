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

void main()
{
	vec2 uv = Pattern_UVTransform(gl_FragCoord.xy);
	vec2 repeat = (2.0 * fract(uv) - 1.0);
	float value = length(repeat);
	float fw = (length(vec2(dFdx(value), dFdy(value))) * 0.70710678118);

	float radiusScale = g_patternUVTransform[1].z;
	float c = smoothstep((radiusScale - fw), (radiusScale + fw), value);

	vec4 primary = s3d_shapeColor(ColorPMA);
	vec4 background = Pattern_BackgroundColor();

	FragColor = mix(primary, background, c);
}
