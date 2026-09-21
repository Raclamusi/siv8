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
	struct BCnData;
	struct Rect;

	class GLES3Texture
	{
	public:

		struct Dynamic {};

		struct NoMipmap {};

		struct GenerateMipmap {};

		[[nodiscard]]
		GLES3Texture(const Image& image, const Array<Image>& mipmaps, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(NoMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(GenerateMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(const BCnData& bcnData);

		[[nodiscard]]
		GLES3Texture(Dynamic, NoMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		[[nodiscard]]
		GLES3Texture(Dynamic, GenerateMipmap, const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc);

		~GLES3Texture();

		[[nodiscard]]
		bool isInitialized() const noexcept;

		[[nodiscard]]
		const GLES3Texture2DDesc& getDesc() const noexcept;

		[[nodiscard]]
		bool hasDepth() const noexcept;

		bool fill(const ColorF& color, bool wait);

		bool fill(std::span<const Byte> data, uint32 srcBytesPerRow, bool wait);

		bool fillRegion(const ColorF& color, const Rect& rect);

		bool fillRegion(std::span<const Byte> data, uint32 srcBytesPerRow, const Rect& rect, bool wait);

		void generateMipmaps();

		[[nodiscard]]
		GLuint getTexture() const noexcept;

	private:

		GLES3Texture2DDesc m_desc;

		bool m_initialized = false;

		// [メインテクスチャ]
		GLuint m_texture = 0;
	};
}
