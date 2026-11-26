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

//
// VSOutput
//
out vec2 UV;
out vec2 SecondaryUV;

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
layout(std140) uniform VSConstants0 // TextureProcessingConstants
{
	ProcessingConstants g_processingConstants;
};

void main()
{
	gl_Position = vec4(
		(gl_VertexID == 2 ? 3.0 : -1.0),
		(gl_VertexID == 1 ? 3.0 : -1.0),
		0.0,
		1.0);

	vec2 uv = vec2(
		(gl_VertexID == 2 ? 2.0 : 0.0),
		(gl_VertexID == 1 ? -1.0 : 1.0));
	UV = g_processingConstants.sourceUV.xy + uv * g_processingConstants.sourceUV.zw;
	SecondaryUV = g_processingConstants.secondaryUV.xy + uv * g_processingConstants.secondaryUV.zw;
}
