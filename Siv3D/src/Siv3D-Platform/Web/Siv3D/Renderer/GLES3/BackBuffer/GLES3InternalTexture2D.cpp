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

# include "GLES3InternalTexture2D.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	clear
	//
	////////////////////////////////////////////////////////////////

	void GLES3InternalTexture2D::clear(const ColorF& color)
	{
		if (isEmpty())
		{
			return;
		}

		const auto [r, g, b] = color.toFloat3();

		::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		::glClearColor(r, g, b, 1.0f);
		::glClear(GL_COLOR_BUFFER_BIT);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	////////////////////////////////////////////////////////////////
	//
	//	reset
	//
	////////////////////////////////////////////////////////////////

	void GLES3InternalTexture2D::reset()
	{
		if (m_texture)
		{
			if (m_sampleCount != 1)
			{
				::glDeleteRenderbuffers(1, &m_texture);
			}
			else
			{
				::glDeleteTextures(1, &m_texture);
			}
			m_texture = 0;
		}

		if (m_frameBuffer)
		{
			::glDeleteFramebuffers(1, &m_frameBuffer);
			m_frameBuffer = 0;
		}

		m_size = Size{ 0, 0 };
		m_sampleCount = 1;
	}

	////////////////////////////////////////////////////////////////
	//
	//	CreateRenderTexture
	//
	////////////////////////////////////////////////////////////////

	std::unique_ptr<GLES3InternalTexture2D> GLES3InternalTexture2D::CreateRenderTexture(const Size& size)
	{
		LOG_SCOPED_DEBUG("GLES3InternalTexture2D::CreateRenderTexture()");

		auto p = std::make_unique<GLES3InternalTexture2D>();

		GLuint frameBuffer = 0;
		GLuint texture = 0;

		::glGenFramebuffers(1, &frameBuffer);
		::glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		::glGenTextures(1, &texture);
		::glBindTexture(GL_TEXTURE_2D, texture);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return nullptr;
		}
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);

		p->m_frameBuffer	= frameBuffer;
		p->m_texture		= texture;
		p->m_size			= size;

		return p;
	}

	////////////////////////////////////////////////////////////////
	//
	//	CreateMSRenderTexture
	//
	////////////////////////////////////////////////////////////////

	std::unique_ptr<GLES3InternalTexture2D> GLES3InternalTexture2D::CreateMSRenderTexture(const Size& size, const uint32 sampleCount)
	{
		LOG_SCOPED_DEBUG(fmt::format("GLES3InternalTexture2D::CreateMSRenderTexture(size = {}, sampleCount= {})", size, sampleCount));

		assert(sampleCount != 0);

		auto p = std::make_unique<GLES3InternalTexture2D>();

		GLuint frameBuffer = 0;
		GLuint texture = 0;

		::glGenFramebuffers(1, &frameBuffer);

		::glGenRenderbuffers(1, &texture);
		::glBindRenderbuffer(GL_RENDERBUFFER, texture);
		::glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_RGBA8, size.x, size.y);

		::glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, texture);
		if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return nullptr;
		}
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);

		p->m_frameBuffer	= frameBuffer;
		p->m_texture		= texture;
		p->m_sampleCount	= sampleCount;
		p->m_size			= size;

		return p;
	}
}
