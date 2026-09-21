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
# include <string>
# include <Siv3D/Common.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>

namespace s3d
{
	class GLES3PixelShader
	{
	public:

		struct Null {};

		[[nodiscard]]
		GLES3PixelShader() = default;

		[[nodiscard]]
		GLES3PixelShader(Null);

		[[nodiscard]]
		GLES3PixelShader(const std::string& source);

		~GLES3PixelShader();

		[[nodiscard]]
		bool isInitialized() const noexcept;

		[[nodiscard]]
		GLuint getShader() const noexcept;

	private:

		GLuint m_shader = 0;

		bool m_initialized = false;
	};
}
