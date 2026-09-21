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

# pragma once
# include <Siv3D/Common.hpp>
# include <Siv3D/Utility.hpp>
# include <Siv3D/Graphics.hpp>
# include <Siv3D/ShaderStage.hpp>
# include <Siv3D/BlendFactor.hpp>
# include <Siv3D/BlendOperation.hpp>
# include <Siv3D/CompareFunction.hpp>
# include <Siv3D/TextureAddressMode.hpp>
# include <Siv3D/TextureFilter.hpp>
# include <GL/glew.h>
# include <GLFW/glfw3.h>

namespace s3d
{
	void CheckOpenGLError();

	constexpr uint32 MakeSamplerSlot(ShaderStage stage, uint32 index) noexcept
	{
		if (stage == ShaderStage::Vertex)
		{
			// unit 8-15 are used for VS
			return (index + Graphics::TextureSlotCount);
		}
		else
		{
			// unit 0-7 are used for PS
			return index;
		}
	}

	inline constexpr uint32 ConstantBufferSlotCount = 12;

	constexpr uint32 MakeUniformBlockBinding(ShaderStage stage, uint32 index) noexcept
	{
		if (stage == ShaderStage::Vertex)
		{
			// index 0-11 are used for VS
			return index;
		}
		else
		{
			// index 12-23 are used for PS
			return (index + ConstantBufferSlotCount);
		}
	}

	[[nodiscard]]
	constexpr GLenum ToGLenum(BlendFactor x) noexcept
	{
		static constexpr GLenum Table[]
		{
			0,
			GL_ZERO,
			GL_ONE,
			GL_SRC_COLOR,
			GL_ONE_MINUS_SRC_COLOR,
			GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,
			GL_DST_ALPHA,
			GL_ONE_MINUS_DST_ALPHA,
			GL_DST_COLOR,
			GL_ONE_MINUS_DST_COLOR,
			GL_SRC_ALPHA_SATURATE,
			0,
			0,
			GL_CONSTANT_COLOR,
			GL_ONE_MINUS_CONSTANT_COLOR,
			GL_SRC1_COLOR,
			GL_ONE_MINUS_SRC1_COLOR,
			GL_SRC1_ALPHA,
			GL_ONE_MINUS_SRC1_ALPHA,
		};

		return Table[FromEnum(x)];
	}

	[[nodiscard]]
	constexpr GLenum ToGLenum(BlendOperation x) noexcept
	{
		static constexpr GLenum Table[]
		{
			0,
			GL_FUNC_ADD,
			GL_FUNC_SUBTRACT,
			GL_FUNC_REVERSE_SUBTRACT,
			GL_MIN,
			GL_MAX,
		};

		return Table[FromEnum(x)];
	}

	[[nodiscard]]
	constexpr GLenum ToGLenum(CompareFunction x) noexcept
	{
		static constexpr GLenum Table[]
		{
			0,
			GL_NEVER,
			GL_LESS,
			GL_EQUAL,
			GL_LEQUAL,
			GL_GREATER,
			GL_NOTEQUAL,
			GL_GEQUAL,
			GL_ALWAYS,
		};

		return Table[FromEnum(x)];
	}

	[[nodiscard]]
	constexpr GLenum ToGLenum(TextureAddressMode x) noexcept
	{
		static constexpr GLenum Table[]
		{
			0,
			GL_REPEAT,
			GL_MIRRORED_REPEAT,
			GL_MIRROR_CLAMP_TO_EDGE,
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_BORDER,
		};

		return Table[FromEnum(x)];
	}

	[[nodiscard]]
	constexpr GLenum ToGLenum(TextureFilter x) noexcept
	{
		static constexpr GLenum Table[]
		{
			GL_NEAREST,
			GL_LINEAR,
		};

		return Table[FromEnum(x)];
	}
}
