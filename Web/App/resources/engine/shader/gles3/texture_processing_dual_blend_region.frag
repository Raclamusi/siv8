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
uniform sampler2D Texture1;

//
// PSInput
//
in vec2 UV;
in vec2 SecondaryUV;

//
//	PSOutput
//
layout(location = 0) out vec4 FragColor;

// Shared constant layout: Siv3D/RenderTextureOps/TextureProcessing.hpp
struct ProcessingConstants
{
	vec4 sourceUV;
	vec4 sourceBounds;
	vec4 secondaryUV;
	vec4 secondaryBounds;
	vec4 parameters;
};

//
//	Constant Buffer
//
layout(std140) uniform PSConstants0 // TextureProcessingConstants
{
	ProcessingConstants g_processingConstants;
};

layout(std140) uniform PSConstants1 // TextureProcessingGaussianSamples
{
	vec4 g_gaussianSamples[49];
};

//
//	Siv3D Functions
//
vec4 s3d_processingSample(const vec2 uv, const bool bounded)
{
	return textureLod(Texture0,
		bounded ? clamp(uv, g_processingConstants.sourceBounds.xy, g_processingConstants.sourceBounds.zw) : uv,
		0.0);
}

vec4 s3d_processingResample(const vec2 uv, const bool bounded)
{
	return s3d_processingSample(uv, bounded);
}

vec4 s3d_processingDownsample(const vec2 uv, const bool bounded)
{
	vec2 offset = g_processingConstants.parameters.xy;
	return (s3d_processingSample(uv + vec2(-offset.x, -offset.y), bounded)
		+ s3d_processingSample(uv + vec2(offset.x, -offset.y), bounded)
		+ s3d_processingSample(uv + vec2(-offset.x, offset.y), bounded)
		+ s3d_processingSample(uv + offset, bounded)) * 0.25;
}

vec4 s3d_processingGaussian(const vec2 uv, const bool bounded)
{
	vec4 result = s3d_processingSample(uv, bounded) * g_gaussianSamples[0].x;
	for (uint i = 1u; i < uint(g_processingConstants.parameters.w); ++i)
	{
		vec2 offset = g_processingConstants.parameters.xy * g_gaussianSamples[i].y;
		result += (s3d_processingSample(uv - offset, bounded)
			+ s3d_processingSample(uv + offset, bounded)) * g_gaussianSamples[i].x;
	}
	return result;
}

vec4 s3d_processingDualUp(const vec2 uv, const bool bounded)
{
	vec2 offset = g_processingConstants.parameters.xy;
	return (s3d_processingSample(uv + vec2(-2.0 * offset.x, 0.0), bounded)
		+ s3d_processingSample(uv + vec2(2.0 * offset.x, 0.0), bounded)
		+ s3d_processingSample(uv + vec2(0.0, -2.0 * offset.y), bounded)
		+ s3d_processingSample(uv + vec2(0.0, 2.0 * offset.y), bounded)
		+ 2.0 * (s3d_processingSample(uv + vec2(-offset.x, -offset.y), bounded)
			+ s3d_processingSample(uv + vec2(offset.x, -offset.y), bounded)
			+ s3d_processingSample(uv + vec2(-offset.x, offset.y), bounded)
			+ s3d_processingSample(uv + offset, bounded))) / 12.0;
}

//
//	Functions
//
void main()
{
	FragColor = mix(
		textureLod(Texture1,
			clamp(SecondaryUV, g_processingConstants.sourceBounds.xy, g_processingConstants.sourceBounds.zw),
			0.0),
		s3d_processingDualUp(UV, true),
		g_processingConstants.parameters.z);
}
