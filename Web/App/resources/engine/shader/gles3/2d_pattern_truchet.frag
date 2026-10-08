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

// Matches Pattern::Truchet::Layout.
const uint Pattern_TruchetLayoutRandom = 0u;
const uint Pattern_TruchetLayoutAlternating = 2u;

uint Pattern_DecodeTruchetSeed(const vec2 seedHalves)
{
	// The CPU stores finite numeric 16-bit halves, not bit-cast float payloads.
	uint seedLow = uint(seedHalves.x);
	uint seedHigh = uint(seedHalves.y);
	return (seedLow | (seedHigh << 16));
}

uint Pattern_TruchetHash(const vec2 cell, const uint seed)
{
	// Hash exact IEEE-754 bits of half-integer cell centers. This avoids signed
	// float-to-int conversion limits and gives +0/-0 the same key at the origin.
	uvec2 key = floatBitsToUint(cell + 0.5);
	uint h = ((key.x * 0x9E3779B9u) ^ (key.y * 0x85EBCA6Bu) ^ seed);
	h ^= (h >> 16);
	h *= 0x7FEB352Du;
	h ^= (h >> 15);
	h *= 0x846CA68Bu;
	h ^= (h >> 16);
	return h;
}

void main()
{
	float normalizedThickness = g_patternUVTransform[1].z; // thickness / pitch

	vec2 drawingPosition = UV;
	vec2 patternUV = Pattern_UVTransform(drawingPosition);
	vec2 cell = floor(patternUV);
	vec2 tilePosition = (patternUV - cell);
	uint truchetLayout = uint(g_patternExtraParams.z);
	bool flip = false; // Uniform (1) keeps the original orientation.
	if (truchetLayout == Pattern_TruchetLayoutRandom)
	{
		uint seed = Pattern_DecodeTruchetSeed(g_patternExtraParams.xy);
		flip = ((Pattern_TruchetHash(cell, seed) & 1u) != 0u);
	}
	else if (truchetLayout == Pattern_TruchetLayoutAlternating)
	{
		flip = (fract(dot(cell, vec2(0.5, 0.5))) > 0.25);
	}
	tilePosition.x = (flip ? (1.0 - tilePosition.x) : tilePosition.x);
	// These equal-radius circles are disjoint, so the nearer center gives the nearer arc.
	vec2 oppositeCornerOffset = (tilePosition - 1.0);
	float distance = abs(sqrt(min(dot(tilePosition, tilePosition),
		dot(oppositeCornerOffset, oppositeCornerOffset))) - 0.5);
	// Differentiate the continuous coordinates, not tile-dependent arc distances.
	float fw = length(fwidth(patternUV));
	float width = (normalizedThickness * (1.0 + 2.0 * fw) - fw);
	float coverage = (1.0 - smoothstep(width - fw, width + fw, 2.0 * distance));
	vec4 primary = ColorPMA;
	vec4 background = Pattern_BackgroundColorPMA();

	FragColor = s3d_applyColorAdd(mix(background, primary, coverage), g_colorAdd);
}
