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
in vec2 UV;

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
	mat2x4 g_patternUVTransform;
	vec4 g_patternBackgroundColor;
	vec4 g_patternExtraParams;
};

//
//	Siv3D Functions
//
vec2 s3d_transformPoint2D(const vec2 position, const mat2x4 transform)
{
	vec2 translation = transform[0].zw;
	vec2 basisX = transform[0].xy;
	vec2 basisY = transform[1].xy;
	return (translation + (position.x * basisX) + (position.y * basisY));
}

vec4 s3d_applyColorAdd(const vec4 colorPMA, const vec4 colorAdd)
{
	return (colorPMA + (colorAdd * colorPMA.a));
}

vec4 s3d_premultiplyAlpha(const vec4 color)
{
	return vec4((color.rgb * color.a), color.a);
}

//
//	Functions
//
vec4 Pattern_BackgroundColorPMA()
{
	return s3d_premultiplyAlpha(g_patternBackgroundColor * g_patternBackgroundColorMul);
}

vec2 Pattern_UVTransform(const vec2 drawingPosition)
{
	return s3d_transformPoint2D(drawingPosition, g_patternUVTransform);
}

void main()
{
	vec2 drawingPosition = UV;
	vec2 patternUV = Pattern_UVTransform(drawingPosition);
	vec2 repeat = (2.0 * fract(patternUV) - 1.0);
	float value = length(repeat);
	float fw = (length(vec2(dFdx(value), dFdy(value))) * 0.70710678118);

	float normalizedRadius = g_patternUVTransform[1].z;
	float c = smoothstep((normalizedRadius - fw), (normalizedRadius + fw), value);

	vec4 primary = ColorPMA;
	vec4 background = Pattern_BackgroundColorPMA();

	FragColor = s3d_applyColorAdd(mix(primary, background, c), g_colorAdd);
}
