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

# include "ConstantBuffer_GLES3.hpp"
# include <Siv3D/Renderer/GLES3/CRenderer_GLES3.hpp>
# include <Siv3D/Engine/Siv3DEngine.hpp>
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	ConstantBuffer_GLES3::ConstantBuffer_GLES3(const size_t size)
		: m_bufferSize{ static_cast<uint32>(size) } {}

	ConstantBuffer_GLES3::~ConstantBuffer_GLES3()
	{
		if (m_uniformBuffer)
		{
			::glDeleteBuffers(1, &m_uniformBuffer);
			m_uniformBuffer = 0;
		}
	}

	bool ConstantBuffer_GLES3::_internal_init()
	{
		if (m_uniformBuffer)
		{
			return true;
		}

		::glGenBuffers(1, &m_uniformBuffer);
		::glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		::glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, nullptr, GL_DYNAMIC_DRAW);
		::glBindBuffer(GL_UNIFORM_BUFFER, 0);

		LOG_DEBUG(fmt::format("ConstantBuffer_GLES3 initialized (size: {0})", m_bufferSize));

		return true;
	}

	bool ConstantBuffer_GLES3::_internal_update(const void* data, const size_t size)
	{
		if (not _internal_init())
		{
			return false;
		}

		assert(size <= m_bufferSize);

		::glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		::glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
		::glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return true;
	}
}
