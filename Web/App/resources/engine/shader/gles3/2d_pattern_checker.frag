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

vec2 Pattern_Integral(vec2 v)
{
	v /= 2.0;
	return (floor(v) + max((2.0 * fract(v) - 1.0), 0.0));
}

float Pattern_CheckersFiltered(const vec2 p, const vec2 hv)
{
	vec2 fw = fwidth(p);
	float w = max(fw.x, fw.y);
	vec2 i = (Pattern_Integral(p + 0.5 * w) - Pattern_Integral(p - 0.5 * w));
	i *= hv;
	i /= w;
	return (i.x + i.y - 2.0 * i.x * i.y);
}

void main()
{
	vec2 uv = Pattern_UVTransform(gl_FragCoord.xy);
	float c = Pattern_CheckersFiltered(uv, g_patternUVTransform[1].zw);

	vec4 primary = s3d_shapeColor(ColorPMA);
	vec4 background = Pattern_BackgroundColor();

	FragColor = mix(primary, background, c);
}
