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
layout(std140) uniform PSConstants0 // PSConstants2D
{
	vec4 g_patternBackgroundColorMul;
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_sdfOutlineColorPMA;
	vec4 g_sdfShadowColorPMA;
};

layout(std140) uniform PSConstants2 // PSTint
{
	vec4 g_tint;
};

//
//	Siv3D Functions
//
vec4 s3d_premultiplyAlpha(const vec4 color)
{
	return vec4((color.rgb * color.a), color.a);
}

vec4 s3d_applyColorAdd(const vec4 colorPMA, const vec4 colorAdd)
{
	return (colorPMA + (colorAdd * colorPMA.a));
}

//
//	Functions
//
void main()
{
	FragColor = (s3d_applyColorAdd(ColorPMA, g_colorAdd) * s3d_premultiplyAlpha(g_tint));
}
