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
layout(std140) uniform PSConstants0 // PSConstants2D
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

vec4 s3d_textureColor(vec4 vertexColorPMA, const vec4 textureColorPMA)
{
	vertexColorPMA *= textureColorPMA;
	return s3d_applyColorAdd(vertexColorPMA, g_colorAdd);
}

//
//	Functions
//
void main()
{
	vec4 texColor = texture(Texture0, UV);

	vec4 colorPMA = s3d_textureColor(ColorPMA, texColor);

	// Quantize straight RGB, then restore PMA. Alpha itself is not quantized.
	// Four intervals give levels 0, 0.25, 0.5, 0.75, and 1 for RGB in [0, 1].
	const float steps = 4.0;
	if (colorPMA.a == 0.0)
	{
		FragColor = colorPMA;
		return;
	}
	vec3 color = (colorPMA.rgb / colorPMA.a);
	vec3 quantized = (floor((color * steps) + 0.5) / steps);
	FragColor = vec4((quantized * colorPMA.a), colorPMA.a);
}
