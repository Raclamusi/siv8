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
# include <Siv3D/PointVector.hpp>
# include <Siv3D/ColorHSV.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>

namespace s3d
{
	class GLES3InternalTexture2D
	{
	public:

		void clear(const ColorF& color);

		void reset();

		[[nodiscard]]
		bool isEmpty() const noexcept
		{
			return m_size.isZero();
		}

		[[nodiscard]]
		operator bool() const noexcept
		{
			return (not isEmpty());
		}

		[[nodiscard]]
		const Size& size() const noexcept
		{
			return m_size;
		}

		[[nodiscard]]
		GLuint getFrameBuffer() const noexcept
		{
			return m_frameBuffer;
		}

		[[nodiscard]]
		GLuint getTexture() const noexcept
		{
			return m_texture;
		}

		[[nodiscard]]
		static std::unique_ptr<GLES3InternalTexture2D> CreateRenderTexture(const Size& size);

		[[nodiscard]]
		static std::unique_ptr<GLES3InternalTexture2D> CreateMSRenderTexture(const Size& size, uint32 sampleCount);

	private:

		GLuint m_frameBuffer = 0;

		GLuint m_texture = 0;

		Size m_size{ 0, 0 };

		uint32 m_sampleCount = 1;
	};
}
