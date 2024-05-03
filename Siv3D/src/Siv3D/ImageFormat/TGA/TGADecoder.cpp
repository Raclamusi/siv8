﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2024 Ryo Suzuki
//	Copyright (c) 2016-2024 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/ImageFormat/TGADecoder.hpp>
# include <Siv3D/IReader.hpp>
# include <Siv3D/EngineLog.hpp>
# include "TGAHeader.hpp"

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	name
	//
	////////////////////////////////////////////////////////////////

	StringView TGADecoder::name() const noexcept
	{
		return U"TGA"_sv;
	}

	////////////////////////////////////////////////////////////////
	//
	//	imageFormat
	//
	////////////////////////////////////////////////////////////////

	ImageFormat TGADecoder::imageFormat() const noexcept
	{
		return ImageFormat::TGA;
	}

	////////////////////////////////////////////////////////////////
	//
	//	isHeader
	//
	////////////////////////////////////////////////////////////////

	bool TGADecoder::isHeader(const uint8(&)[16]) const noexcept
	{
		// [Siv3D ToDo]
		return false;
	}

	////////////////////////////////////////////////////////////////
	//
	//	possibleExtensions
	//
	////////////////////////////////////////////////////////////////

	const Array<String>& TGADecoder::possibleExtensions() const
	{
		static const Array<String> extensions{ U"tga" };

		return extensions;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getImageInfo
	//
	////////////////////////////////////////////////////////////////

	Optional<ImageInfo> TGADecoder::getImageInfo(const FilePathView path) const
	{
		return IImageDecoder::getImageInfo(path);
	}

	Optional<ImageInfo> TGADecoder::getImageInfo(IReader& reader, const FilePathView) const
	{
		TGAHeader header;

		if (not reader.lookahead(header))
		{
			LOG_FAIL("❌ TGADecoder::getImageInfo(): Failed to load the header");
			return{};
		}

		const Size size{ header.width, header.height };
		
		ImagePixelFormat pixelFormat = ImagePixelFormat::Unknown;

		if (header.imageType == 2)
		{
			if (header.bpp == 24)
			{
				pixelFormat = ImagePixelFormat::R8G8B8X8;
			}
			else if (header.bpp == 32)
			{
				pixelFormat = ImagePixelFormat::R8G8B8A8;
			}
		}

		return ImageInfo{ size, ImageFormat::TGA, pixelFormat, false };
	}

	////////////////////////////////////////////////////////////////
	//
	//	decode
	//
	////////////////////////////////////////////////////////////////

	Image TGADecoder::decode(const FilePathView path, const PremultiplyAlpha premultiplyAlpha) const
	{
		return IImageDecoder::decode(path, premultiplyAlpha);
	}

	Image TGADecoder::decode(IReader& reader, const FilePathView, const PremultiplyAlpha) const
	{
		LOG_SCOPED_DEBUG("TGADecoder::decode()");

		TGAHeader header;
			
		if (not reader.read(header))
		{
			return{};
		}

		if ((header.bpp != 24)
			&& (header.bpp != 32))
		{
			return{};
		}

		if (header.descLen)
		{
			reader.setPos(reader.getPos() + header.descLen);
		}

		const int32 width		= header.width;
		const int32 height		= header.height;
		const int32 pixelSize	= (header.bpp / 8);
		const int32 data_bytes	= width * height * pixelSize;
		const bool reversed		= !(header.attrib & 0x20);

		std::unique_ptr<uint8[]> readPixels;
		try
		{
			readPixels = std::make_unique<uint8[]>(data_bytes);
		}
		catch (const std::bad_alloc&)
		{
			return{};
		}

		reader.read(readPixels.get(), data_bytes);

		Image image{ Size{ width, height }, Color{ 255 } };

		switch (header.bpp)
		{
		case 24:
			{
				Color* pLine = image[reversed ? height - 1 : 0];
				const uint8* pSrc = &readPixels[0];
				int32 step = reversed ? -width : width;

				for (int32 y = 0; y < height; ++y)
				{
					Color* pDst = pLine;

					for (int32 x = 0; x < width; ++x)
					{
						pDst->set(pSrc[2], pSrc[1], pSrc[0]);
						pSrc += 3;
						++pDst;
					}

					pLine += step;
				}

				break;
			}
		case 32:
			{
				Color* pLine = image[reversed ? height - 1 : 0];
				const uint8* pSrc = &readPixels[0];
				int32 step = reversed ? -width : width;

				for (int32 y = 0; y < height; ++y)
				{
					Color* pDst = pLine;

					for (int32 x = 0; x < width; ++x)
					{
						pDst->set(pSrc[2], pSrc[1], pSrc[0], pSrc[3]);
						pSrc += 4;
						++pDst;
					}

					pLine += step;
				}

				break;
			}
		}

		LOG_TRACE(fmt::format("Image ({}x{}) decoded", width, height));

		return image;
	}
}
