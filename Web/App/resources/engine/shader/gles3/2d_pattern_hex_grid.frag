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

float Pattern_Hex(const vec2 p)
{
	const vec2 HEX = vec2(1, 1.73205081);
	vec4 t = (floor(vec4(p, p - vec2(0.5, 1)) / HEX.xyxy) + vec4(0.5, 0.5, 0.5, 0.5));
	vec4 h = vec4((p - t.xy * HEX), (p - (t.zw + vec2(0.5, 0.5)) * HEX));
	vec2 hex = abs((dot(h.xy, h.xy) < dot(h.zw, h.zw)) ? h.xy : h.zw);
	return max(dot(hex, (HEX * 0.5)), hex.x);
}

void main()
{
	vec2 uv = Pattern_UVTransform(gl_FragCoord.xy);
	vec2 fw = fwidth(uv);
	float w = (max(fw.x, fw.y) * 0.5);

	float thicknessScale = (g_patternUVTransform[1].z * (1.0 + 2.0 * w));
	float h = Pattern_Hex(uv);
	float c = smoothstep((thicknessScale - w), (thicknessScale + w), h);

	vec4 primary = s3d_shapeColor(ColorPMA);
	vec4 background = Pattern_BackgroundColor();

	FragColor = mix(primary, background, c);
}
