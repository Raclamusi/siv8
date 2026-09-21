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
vec4 s3d_textureColor(vec4 vertexColorPMA, const vec4 textureColorPMA)
{
	vertexColorPMA *= textureColorPMA;
	return (vertexColorPMA + (g_colorAdd * vertexColorPMA.a));
}

//
//	Functions
//
void main()
{
	vec4 texColor = texture(Texture0, UV);

	FragColor = s3d_textureColor(ColorPMA, texColor);
}
