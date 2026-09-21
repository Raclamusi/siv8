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
//	MSDF
//
const float MSDF_PixelRange = 16.0;
const float MSDF_TextThreshold = 0.5;

struct MSDFState
{
	vec2 textureSize;
	vec2 invTextureSize;
	float scale;
};

MSDFState MSDF_Init(const vec2 uv)
{
	MSDFState st;
	st.textureSize = vec2(textureSize(Texture0, 0));
	st.invTextureSize = (1.0 / st.textureSize);

	vec2 msdfUnit = (MSDF_PixelRange * st.invTextureSize);
	vec2 screenPixelRange = (0.5 / fwidth(uv));
	st.scale = dot(msdfUnit, screenPixelRange);

	return st;
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

float MSDF_Coverage(const float d, const float threshold, const float scale)
{
	return clamp(((d - threshold) * scale + 0.5), 0.0, 1.0);
}

float MSDF_AlphaAt(const vec2 uv, const float threshold, const MSDFState st)
{
	return MSDF_Coverage(MSDF_SampleDistance(uv), threshold, st.scale);
}

vec4 MSDF_PremulAdd(const vec4 colorPMA)
{
	return (colorPMA + (g_colorAdd * colorPMA.a));
}

//
//	Functions
//
void main()
{
	MSDFState st = MSDF_Init(UV);

	float textAlpha = MSDF_AlphaAt(UV, MSDF_TextThreshold, st);

	vec4 textPMA = (ColorPMA * textAlpha);

	FragColor = MSDF_PremulAdd(textPMA);
}
