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
//	Textures
//
uniform sampler2D Texture0;

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

//
//	Siv3D Functions
//
vec4 s3d_applyColorAdd(const vec4 colorPMA, const vec4 colorAdd)
{
	return (colorPMA + (colorAdd * colorPMA.a));
}

//
//	MSDF
//
const float MSDF_PixelRange = 16.0;
const float MSDF_TextThreshold = 0.5;

struct MSDFState
{
	vec2 invTextureSize;
	float coverageScale;
};

MSDFState MSDF_Init(const vec2 uv)
{
	vec2 textureSize = vec2(textureSize(Texture0, 0));

	MSDFState state;
	state.invTextureSize = (1.0 / textureSize);

	// Combine atlas dimensions and UV derivatives before taking the reciprocal.
	vec2 screenPixelRange = ((0.5 * MSDF_PixelRange) / (textureSize * fwidth(uv)));
	state.coverageScale = (screenPixelRange.x + screenPixelRange.y);

	return state;
}

float MSDF_Median(const float r, const float g, const float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float MSDF_Median(const vec3 rgb)
{
	return MSDF_Median(rgb.r, rgb.g, rgb.b);
}

float MSDF_SampleDistance(const vec2 uv)
{
	return MSDF_Median(texture(Texture0, uv).rgb);
}

float MSDF_Coverage(const float sampledDistance, const float threshold, const float coverageScale)
{
	return clamp(((sampledDistance - threshold) * coverageScale + 0.5), 0.0, 1.0);
}

float MSDF_AlphaAt(const vec2 uv, const float threshold, const MSDFState state)
{
	return MSDF_Coverage(MSDF_SampleDistance(uv), threshold, state.coverageScale);
}

//
//	Functions
//
void main()
{
	MSDFState state = MSDF_Init(UV);
	float sampledDistance = MSDF_SampleDistance(UV);

	const float outlineDistanceOffset = 0.04;
	const float outlineThreshold = (MSDF_TextThreshold - outlineDistanceOffset);

	float textAlpha = sqrt(clamp(((sampledDistance - 0.5) * state.coverageScale + 0.5), 0.0, 1.0));
	float outlineAlpha = sqrt(clamp(((sampledDistance - outlineThreshold) * state.coverageScale + 0.5), 0.0, 1.0));

	const vec2 shadowOffsetTexels = vec2(0.625, 0.625);
	vec2 shadowOffsetUV = (shadowOffsetTexels * state.invTextureSize);
	float shadowDistance = MSDF_SampleDistance(UV - shadowOffsetUV);
	float shadowAlpha = sqrt(clamp(((shadowDistance - outlineThreshold) * state.coverageScale + 0.5), 0.0, 1.0));

	vec3 color = mix(vec3(0.0), vec3(1.0), textAlpha);
	float hollowShadowAlpha = clamp((shadowAlpha * (1.0 - outlineAlpha)), 0.0, 1.0);
	color = mix(color, vec3(0.0), hollowShadowAlpha);

	float finalAlpha = clamp((outlineAlpha + hollowShadowAlpha), 0.0, 1.0);

	FragColor = vec4(color, finalAlpha);
}
