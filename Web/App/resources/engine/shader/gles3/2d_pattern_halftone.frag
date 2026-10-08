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
layout(std140) uniform PSConstants0 // PSConstants2D
{
	vec4 g_patternBackgroundColorMul;
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_sdfOutlineColorPMA;
	vec4 g_sdfShadowColorPMA;
};

layout(std140) uniform PSConstants1 // PSEffectConstants2D
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
	float normalizedMinRadius = g_patternUVTransform[1].z; // 2 * minRadius / pitch
	float normalizedMaxRadius = g_patternUVTransform[1].w; // 2 * maxRadius / pitch
	// Linear radius field expressed in pattern UV coordinates.
	vec2 radiusFieldGradient = g_patternExtraParams.xy;
	float radiusFieldBias = g_patternExtraParams.z;

	vec2 drawingPosition = UV;
	vec2 patternUV = Pattern_UVTransform(drawingPosition);
	vec2 cellCenter = (floor(patternUV) + 0.5);
	vec2 repeat = (2.0 * (patternUV - cellCenter));
	float radiusBlend = clamp((dot(cellCenter, radiusFieldGradient) + radiusFieldBias), 0.0, 1.0);
	float normalizedRadius = mix(normalizedMinRadius, normalizedMaxRadius,
		(radiusBlend * radiusBlend * (3.0 - 2.0 * radiusBlend)));
	// Differentiate continuous UVs, not the radius that changes between cells.
	float fw = length(fwidth(patternUV));
	float coverage = ((1.0 - smoothstep(normalizedRadius - fw, normalizedRadius + fw, length(repeat)))
		* clamp((normalizedRadius / fw), 0.0, 1.0));
	vec4 primary = ColorPMA;
	vec4 background = Pattern_BackgroundColorPMA();

	FragColor = s3d_applyColorAdd(mix(background, primary, coverage), g_colorAdd);
}
