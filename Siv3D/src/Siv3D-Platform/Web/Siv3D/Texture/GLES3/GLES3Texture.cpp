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

# include "GLES3Texture.hpp"
# include <Siv3D/Texture/TextureUtility.hpp>
# include <Siv3D/ImageProcessing.hpp>
# include <Siv3D/Blob.hpp>
# include <Siv3D/EngineLog.hpp>
# include <Siv3D/Anchor.hpp>
# include <Siv3D/Rect.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3Texture::GLES3Texture(const Image& image, const std::span<const Image> mipmaps, const TextureDesc desc)
		: m_desc{ desc,
			TextureType::Default,
			image.size(),
			static_cast<uint8>(mipmaps.size() + 1),
			1,
			(desc.sRGB ? TextureFormat::R8G8B8A8_Unorm_SRGB : TextureFormat::R8G8B8A8_Unorm),
			false
		}
	{
		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(mipmaps.size()));

			::glTexImage2D(GL_TEXTURE_2D, 0,
				m_desc.GLInternalFormat(), image.width(), image.height(), 0,
				m_desc.GLFormat(), m_desc.GLType(), image.data());

			for (uint32 i = 0; i < mipmaps.size(); ++i)
			{
				const Image& mipmap = mipmaps[i];
				::glTexImage2D(GL_TEXTURE_2D, (i + 1),
					m_desc.GLInternalFormat(), mipmap.width(), mipmap.height(), 0,
					m_desc.GLFormat(), m_desc.GLType(), mipmap.data());
			}
		}

		m_initialized = true;
	}

	GLES3Texture::GLES3Texture(NoMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, const TextureDesc desc)
		: m_desc{ desc,
			TextureType::Default,
			size,
			1,
			1,
			format,
			false
		}
	{
		if (const uint64 expectedSize = GetTextureUploadSize(format.bytesPerRow(size.x), size.y);
			data.size_bytes() != expectedSize)
		{
			LOG_FAIL(fmt::format("❌ GLES3Texture::GLES3Texture(): size_bytes[{}] != expectedSize[{}]", data.size_bytes(), expectedSize));
			return;
		}

		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

			::glTexImage2D(GL_TEXTURE_2D, 0,
				m_desc.GLInternalFormat(), size.x, size.y, 0,
				m_desc.GLFormat(), m_desc.GLType(), data.data());
		}

		m_initialized = true;
	}

	GLES3Texture::GLES3Texture(GenerateMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, const TextureDesc desc)
		: m_desc{ desc,
			TextureType::Default,
			size,
			ImageProcessing::CalculateMipmapLevel(size.x, size.y),
			1,
			format,
			false
		}
	{
		if (const uint64 expectedSize = GetTextureUploadSize(format.bytesPerRow(size.x), size.y);
			data.size_bytes() != expectedSize)
		{
			LOG_FAIL(fmt::format("❌ GLES3Texture::GLES3Texture(): size_bytes[{}] != expectedSize[{}]", data.size_bytes(), expectedSize));
			return;
		}

		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (m_desc.mipLevels - 1));

			::glTexImage2D(GL_TEXTURE_2D, 0,
				m_desc.GLInternalFormat(), size.x, size.y, 0,
				m_desc.GLFormat(), m_desc.GLType(), data.data());
		}

		m_initialized = true;

		generateMipmaps();
	}

	GLES3Texture::GLES3Texture(const Size& size, const TextureFormat& format, const std::span<const Blob> mipmaps)
		: m_desc{ ((1 < mipmaps.size()) ? TextureDesc::Mipmap : TextureDesc::NoMipmap),
			TextureType::Default,
			size,
			static_cast<uint8>(mipmaps.size()),
			1,
			format,
			false
		}
	{
		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (m_desc.mipLevels - 1));

			for (uint32 i = 0; i < mipmaps.size(); ++i)
			{
				const Size levelSize = ImageProcessing::CalculateMipmapSize(size, i);

				::glTexImage2D(GL_TEXTURE_2D, (i + 1),
					m_desc.GLInternalFormat(), levelSize.x, levelSize.y, 0,
					m_desc.GLFormat(), m_desc.GLType(), mipmaps[i].data());
			}
		}

		m_initialized = true;
	}

	GLES3Texture::GLES3Texture(Dynamic, NoMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, const TextureDesc desc)
		: m_desc{ desc,
			TextureType::Dynamic,
			size,
			1,
			1,
			format,
			false
		}
	{
		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

			::glTexImage2D(GL_TEXTURE_2D, 0,
				m_desc.GLInternalFormat(), size.x, size.y, 0,
				m_desc.GLFormat(), m_desc.GLType(), (data.empty() ? nullptr : data.data()));
		}

		m_initialized = true;
	}

	GLES3Texture::GLES3Texture(Dynamic, GenerateMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, const TextureDesc desc)
		: m_desc{ desc,
			TextureType::Dynamic,
			size,
			ImageProcessing::CalculateMipmapLevel(size.x, size.y),
			1,
			format,
			false
		}
	{
		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (m_desc.mipLevels - 1));

			::glTexImage2D(GL_TEXTURE_2D, 0,
				m_desc.GLInternalFormat(), size.x, size.y, 0,
				m_desc.GLFormat(), m_desc.GLType(), (data.empty() ? nullptr : data.data()));
		}

		m_initialized = true;

		if (not data.empty())
		{
			generateMipmaps();
		}
	}

	GLES3Texture::GLES3Texture(Render, const Size& size, const std::span<const Byte> data, const TextureFormat& format, const TextureDesc desc, const ColorF& color, const uint32 sampleCount)
		: m_desc{ desc,
			((sampleCount == 1) ? TextureType::Render : TextureType::MSRender),
			size,
			(desc.hasMipmap ? ImageProcessing::CalculateMipmapLevel(size.x, size.y) : uint8{ 1 }),
			static_cast<uint8>(sampleCount),
			format,
			false
		}
	{
		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (m_desc.mipLevels - 1));

			::glTexImage2D(GL_TEXTURE_2D, 0,
				m_desc.GLInternalFormat(), size.x, size.y, 0,
				m_desc.GLFormat(), m_desc.GLType(), (data.empty() ? nullptr : data.data()));

			::glGenFramebuffers(1, &m_frameBuffer);
			::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
			::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
			if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				LOG_FAIL(fmt::format("❌ GLES3Texture::GLES3Texture(): Failed to create framebuffer for format {}", format.name().toUTF8()));
				return;
			}
			::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// [マルチサンプル・テクスチャ] を作成
		if (sampleCount > 1)
		{
			::glGenRenderbuffers(1, &m_multiSampleTexture);
			::glBindRenderbuffer(GL_RENDERBUFFER, m_multiSampleTexture);
			::glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, format.GLInternalFormat(), size.x, size.y);

			::glGenFramebuffers(1, &m_multiSampleFrameBuffer);
			::glBindFramebuffer(GL_FRAMEBUFFER, m_multiSampleFrameBuffer);
			::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_multiSampleTexture);
			if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				LOG_FAIL(fmt::format("❌ GLES3Texture::GLES3Texture(): Failed to create multisampled framebuffer for format {}", format.name().toUTF8()));
				return;
			}
			::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		m_initialized = true;

		generateMipmaps();
	}

	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3Texture::~GLES3Texture()
	{
		// [マルチサンプルテクスチャ] を破棄
		if (m_multiSampleFrameBuffer)
		{
			::glDeleteFramebuffers(1, &m_multiSampleFrameBuffer);
			m_multiSampleFrameBuffer = 0;
		}
		if (m_multiSampleTexture)
		{
			::glDeleteRenderbuffers(1, &m_multiSampleTexture);
			m_multiSampleTexture = 0;
		}

		// [メインテクスチャ] を破棄
		if (m_frameBuffer)
		{
			::glDeleteFramebuffers(1, &m_frameBuffer);
			m_frameBuffer = 0;
		}
		if (m_texture)
		{
			::glDeleteTextures(1, &m_texture);
			m_texture = 0;
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	isInitialized
	//
	////////////////////////////////////////////////////////////////

	bool GLES3Texture::isInitialized() const noexcept
	{
		return m_initialized;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getDesc
	//
	////////////////////////////////////////////////////////////////

	const GLES3Texture2DDesc& GLES3Texture::getDesc() const noexcept
	{
		return m_desc;
	}

	////////////////////////////////////////////////////////////////
	//
	//	hasDepth
	//
	////////////////////////////////////////////////////////////////

	bool GLES3Texture::hasDepth() const noexcept
	{
		return m_desc.hasDepth;
	}

	////////////////////////////////////////////////////////////////
	//
	//	fill
	//
	////////////////////////////////////////////////////////////////

	bool GLES3Texture::fill(const ColorF& color, const bool)
	{
		if (m_desc.type != TextureType::Dynamic)
		{
			return false;
		}

		const Array<Byte> data = GenerateInitialColorBuffer(m_desc.size, color, m_desc.format);

		if (not data)
		{
			return false;
		}

		::glBindTexture(GL_TEXTURE_2D, m_texture);

		::glTexSubImage2D(GL_TEXTURE_2D, 0,
			0, 0, m_desc.size.x, m_desc.size.y,
			m_desc.GLFormat(), m_desc.GLType(), data.data());

		return true;
	}

	bool GLES3Texture::fill(const std::span<const Byte> data, const uint32, const bool)
	{
		if (m_desc.type != TextureType::Dynamic)
		{
			return false;
		}

		::glBindTexture(GL_TEXTURE_2D, m_texture);

		::glTexSubImage2D(GL_TEXTURE_2D, 0,
			0, 0, m_desc.size.x, m_desc.size.y,
			m_desc.GLFormat(), m_desc.GLType(), data.data());

		return true;
	}

	////////////////////////////////////////////////////////////////
	//
	//	fillRegion
	//
	////////////////////////////////////////////////////////////////

	bool GLES3Texture::fillRegion(const ColorF& color, const Rect& rect, const bool)
	{
		if (m_desc.type != TextureType::Dynamic)
		{
			return false;
		}

		const Array<Byte> data = GenerateInitialColorBuffer(rect.size, color, m_desc.format);

		if (not data)
		{
			return false;
		}

		::glBindTexture(GL_TEXTURE_2D, m_texture);

		::glTexSubImage2D(GL_TEXTURE_2D, 0,
			rect.x, rect.y, rect.w, rect.h,
			m_desc.GLFormat(), m_desc.GLType(), data.data());

		return true;
	}

	bool GLES3Texture::fillRegion(const std::span<const Byte> data, const uint32, const Rect& rect, const bool)
	{
		if (m_desc.type != TextureType::Dynamic)
		{
			return false;
		}

		::glBindTexture(GL_TEXTURE_2D, m_texture);

		::glTexSubImage2D(GL_TEXTURE_2D, 0,
			rect.x, rect.y, rect.w, rect.h,
			m_desc.GLFormat(), m_desc.GLType(), data.data());

		return true;
	}

	////////////////////////////////////////////////////////////////
	//
	//	clearRT
	//
	////////////////////////////////////////////////////////////////

	void GLES3Texture::clearRT(const ColorF& color)
	{
		if ((m_desc.type != TextureType::Render) && (m_desc.type != TextureType::MSRender))
		{
			return;
		}

		const auto [r, g, b, a] = color.toFloat4();

		::glBindFramebuffer(GL_FRAMEBUFFER, getFrameBuffer());
		::glClearColor(r, g, b, a);
		::glClear(GL_COLOR_BUFFER_BIT);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	////////////////////////////////////////////////////////////////
	//
	//	resolveMSRT
	//
	////////////////////////////////////////////////////////////////

	void GLES3Texture::resolveMSRT()
	{
		if (m_desc.type != TextureType::MSRender)
		{
			return;
		}

		const auto [w, h] = m_desc.size;

		::glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multiSampleFrameBuffer);
		::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
		::glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	////////////////////////////////////////////////////////////////
	//
	//	readRT
	//
	////////////////////////////////////////////////////////////////

	bool GLES3Texture::readRT(const std::span<Byte> destination, const uint32 mipLevel)
	{
		if (((m_desc.type != TextureType::Render) && (m_desc.type != TextureType::MSRender)) || (m_desc.mipLevels <= mipLevel))
		{
			return false;
		}

		const Size levelSize = ImageProcessing::CalculateMipmapSize(m_desc.size, mipLevel);
		const uint32 width = levelSize.x;
		const uint32 height = levelSize.y;
		const uint32 rowBytes = static_cast<uint32>(m_desc.format.packedRowBytes(width));
		if (destination.size_bytes() != (static_cast<size_t>(rowBytes) * height))
		{
			return false;
		}

		::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, static_cast<GLint>(mipLevel));
		::glReadPixels(0, 0, width, height, m_desc.GLFormat(), m_desc.GLType(), destination.data());
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	////////////////////////////////////////////////////////////////
	//
	//	generateMipmaps
	//
	////////////////////////////////////////////////////////////////

	void GLES3Texture::generateMipmaps()
	{
		if (m_desc.mipLevels <= 1)
		{
			return;
		}

		::glBindTexture(GL_TEXTURE_2D, m_texture);
		::glGenerateMipmap(GL_TEXTURE_2D);
	}

	////////////////////////////////////////////////////////////////
	//
	//	getTexture
	//
	////////////////////////////////////////////////////////////////

	GLuint GLES3Texture::getTexture() const noexcept
	{
		return m_texture;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getMultiSampleTexture
	//
	////////////////////////////////////////////////////////////////

	GLuint GLES3Texture::getMultiSampleTexture() const noexcept
	{
		return m_multiSampleTexture;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getFrameBuffer
	//
	////////////////////////////////////////////////////////////////

	GLuint GLES3Texture::getFrameBuffer() const noexcept
	{
		return (m_desc.isMSAA() ? m_multiSampleFrameBuffer : m_frameBuffer);
	}
}
