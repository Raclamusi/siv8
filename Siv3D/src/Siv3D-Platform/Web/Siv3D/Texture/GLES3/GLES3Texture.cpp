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
# include <Siv3D/BCnData.hpp>
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

	GLES3Texture::GLES3Texture(const Image& image, const Array<Image>& mipmaps, const TextureDesc desc)
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
		if (const size_t expectedSize = (format.bytesPerRow(size.x) * size.y);
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
		if (const size_t expectedSize = (format.bytesPerRow(size.x) * size.y);
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

	GLES3Texture::GLES3Texture(const BCnData& bcnData)
		: m_desc{ ((1 < bcnData.textures.size()) ? TextureDesc::Mipmap : TextureDesc::NoMipmap),
			TextureType::Default,
			bcnData.size,
			static_cast<uint8>(bcnData.textures.size()),
			1,
			bcnData.format,
			false
		}
	{
		// [メインテクスチャ] を作成
		{
			::glGenTextures(1, &m_texture);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, (m_desc.mipLevels - 1));

			for (uint32 i = 0; i < bcnData.textures.size(); ++i)
			{
				const Blob& texture = bcnData.textures[i];
				const uint32 width  = Max<uint32>((bcnData.size.x >> i), 1);
				const uint32 height = Max<uint32>((bcnData.size.y >> i), 1);

				::glCompressedTexImage2D(GL_TEXTURE_2D, i,
					m_desc.GLInternalFormat(), width, height, 0,
					static_cast<GLsizei>(texture.size_bytes()), texture.data());
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

	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3Texture::~GLES3Texture()
	{
		// [メインテクスチャ] を破棄
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

	bool GLES3Texture::fillRegion(const ColorF& color, const Rect& rect)
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
}
