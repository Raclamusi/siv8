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
# include <Siv3D/Image.hpp>
# include "GLES3Texture2DDesc.hpp"

namespace s3d
{
	class Blob;
	struct Rect;

	class GLES3Texture
	{
	public:

		struct Dynamic {};

		struct Render {};

		struct NoMipmap {};

		struct GenerateMipmap {};

		[[nodiscard]]
		GLES3Texture(const Image& image, std::span<const Image> mipmaps, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(NoMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(GenerateMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(const Size& size, const TextureFormat& format, std::span<const Blob> mipmaps);

		[[nodiscard]]
		GLES3Texture(Dynamic, NoMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(Dynamic, GenerateMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(Render, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc, const ColorF& color, uint32 sampleCount = 1);

		~GLES3Texture();

		[[nodiscard]]
		bool isInitialized() const noexcept;

		[[nodiscard]]
		const GLES3Texture2DDesc& getDesc() const noexcept;

		[[nodiscard]]
		bool hasDepth() const noexcept;

		bool fill(const ColorF& color, bool wait);

		bool fill(std::span<const Byte> data, uint32 srcBytesPerRow, bool wait);

		bool fillRegion(const ColorF& color, const Rect& rect, bool wait);

		bool fillRegion(std::span<const Byte> data, uint32 srcBytesPerRow, const Rect& rect, bool wait);

		void clearRT(const ColorF& color);

		void resolveMSRT();

		bool readRT(std::span<Byte> destination, uint32 mipLevel);

		void generateMipmaps();

		[[nodiscard]]
		GLuint getTexture() const noexcept;

		[[nodiscard]]
		GLuint getMultiSampleTexture() const noexcept;

		[[nodiscard]]
		GLuint getFrameBuffer() const noexcept;

	private:

		GLES3Texture2DDesc m_desc;

		bool m_initialized = false;

		// [メインテクスチャ]
		GLuint m_texture = 0;
		GLuint m_frameBuffer = 0;

		// [マルチサンプル・テクスチャ]
		GLuint m_multiSampleTexture = 0;
		GLuint m_multiSampleFrameBuffer = 0;
	};
}
