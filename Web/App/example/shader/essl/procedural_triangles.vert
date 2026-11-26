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
//	VSInput
//
layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec4 VertexColor;

//
//	VSOutput
//
out vec4 ColorPMA;
out vec2 UV;

//
//	Constant Buffer
//
layout(std140) uniform VSConstants0 // VSConstants2D
{
	mat3x4 g_transform;
	vec4 g_colorMul;
};

layout(std140) uniform VSConstants2 // Triangles
{
	vec4 g_originSize;
	vec4 g_stepRotation;
	vec4 g_color;
};

//
//	Functions
//
void main()
{
	vec2 corners[3] = vec2[3](vec2(0, 0), vec2(1, 0), vec2(0, 1));
	vec2 corner = corners[gl_VertexID % 3] * g_originSize.zw;
	float cosine = g_stepRotation.z;
	float sine = g_stepRotation.w;
	vec2 rotated = vec2(corner.x * cosine - corner.y * sine, corner.x * sine + corner.y * cosine);
	vec2 position = g_originSize.xy + rotated + float(gl_VertexID / 3) * g_stepRotation.xy;
	vec4 clip = g_transform * vec3(position, 1);
	vec4 color = g_color * g_colorMul;

	gl_Position = vec4(clip.xy, 0, clip.w);
	ColorPMA = vec4(color.rgb * color.a, color.a);
	UV = corners[gl_VertexID % 3];
}
