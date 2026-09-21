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
	vec2 uv = ((UV + vec2(0.5, 0.0)) * vec2(0.5, 1.0));
	float w = fwidth(uv.y);
	float distance = length(vec2(4.0, 2.0) * fract(uv) - vec2(2.0, 1.0));
	float alpha = (1.0 - smoothstep((1.0 - w), (1.0 + w), distance));

	FragColor = s3d_shapeColor(ColorPMA * alpha);
}
