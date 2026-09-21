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
# include <Siv3D/IConstantBuffer.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>

namespace s3d
{
	class ConstantBuffer_GLES3 final : public IConstantBuffer
	{
	public:

		explicit ConstantBuffer_GLES3(size_t size);

		~ConstantBuffer_GLES3() override;

		bool _internal_init() override;

		bool _internal_update(const void* data, size_t size) override;

		[[nodiscard]]
		GLuint getBuffer() const noexcept
		{
			return m_uniformBuffer;
		}

	private:

		GLuint m_uniformBuffer = 0;

		uint32 m_bufferSize = 0;
	};
}
