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

//
//	Siv3D Functions
//
vec4 s3d_shapeColor(const vec4 vertexColorPMA)
{
	return (vertexColorPMA + (g_colorAdd * vertexColorPMA.a));
}

//
//	Functions
//
void main()
{
	float u = (0.1 * (UV.x - 1.0));
	float u2 = (u + 0.5);
	float w = fwidth(u);
	float distance = abs(2.0 * fract(u) - 1.0);
	float distance2 = abs(2.0 * fract(u2) - 1.0);
	float alpha1 = smoothstep((0.4 - w), (0.4 + w), distance);
	float alpha2 = smoothstep((0.9 - w), (0.9 + w), distance2);
	float alpha = max(alpha1, alpha2);

	FragColor = s3d_shapeColor(ColorPMA * alpha);
}
