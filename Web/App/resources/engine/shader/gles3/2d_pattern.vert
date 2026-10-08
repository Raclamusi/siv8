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
//	Siv3D Functions
//
vec4 s3d_positionTransform(const vec2 position, const mat3x4 transform)
{
	vec4 clip = (transform * vec3(position, 1.0));
	return vec4(clip.xy, 0.0, clip.w);
}

vec4 s3d_premultiplyAlpha(const vec4 color)
{
	return vec4((color.rgb * color.a), color.a);
}

//
//	Constant Buffer
//
layout(std140) uniform VSConstants2D
{
	mat3x4 g_transform;
	vec4 g_colorMul;
};

//
//	Functions
//
void main()
{
	gl_Position = s3d_positionTransform(VertexPosition, g_transform);

	ColorPMA = s3d_premultiplyAlpha(VertexColor * g_colorMul);

	UV = VertexPosition;
}
