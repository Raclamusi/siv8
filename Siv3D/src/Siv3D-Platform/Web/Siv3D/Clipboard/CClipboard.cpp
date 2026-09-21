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

# include "CClipboard.hpp"
# include <Siv3D/EngineLog.hpp>
# include <Siv3D/Image.hpp>
# include <Siv3D/MemoryViewReader.hpp>

namespace s3d
{
	extern "C"
	{
		size_t siv3dGetClipboardSequenceNumber();
		void siv3dSetClipboardText(const char32* ptr, size_t length);
		char32* siv3dGetClipboardText(size_t* length);
		bool siv3dHasClipboardText();
		void siv3dSetClipboardImage(const void* pngPtr, size_t pngSize);
		void* siv3dGetClipboardImage(size_t* pngSize);
		bool siv3dHasClipboardImage();
		void siv3dSetClipboardHTML(const char32* htmlPtr, size_t htmlLength, const char32* plainTextPtr, size_t plainTextLength);
		void siv3dSetClipboardData(const char32* mimeTypePtr, size_t mimeTypeLength, const void* dataPtr, size_t dataSize, const char32* plainTextPtr, size_t plainTextLength);
		void* siv3dGetClipboardData(const char32* mimeTypePtr, size_t mimeTypeLength, size_t* dataSizePtr);
		const char32** siv3dGetClipboardAvailableMimeTypes();
	}

	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	CClipboard::~CClipboard()
	{
		LOG_SCOPED_DEBUG("CClipboard::~CClipboard()");
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::init()
	{
		LOG_SCOPED_DEBUG("CClipboard::init()");
	}

	////////////////////////////////////////////////////////////////
	//
	//	hasChanged
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::hasChanged()
	{
		const uint64 currentSequenceNumber = getSequenceNumber();
		return (std::exchange(m_sequenceNumber, currentSequenceNumber) != currentSequenceNumber);
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSequenceNumber
	//
	////////////////////////////////////////////////////////////////

	uint64 CClipboard::getSequenceNumber()
	{
		// Not supported on some browsers
		return siv3dGetClipboardSequenceNumber();
	}

	////////////////////////////////////////////////////////////////
	//
	//	clear
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::clear()
	{
		setText(U"");
	}

	////////////////////////////////////////////////////////////////
	//
	//	setText
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::setText(const StringView text)
	{
		siv3dSetClipboardText(text.data(), text.size());
	}

	////////////////////////////////////////////////////////////////
	//
	//	getText
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::getText(String& text)
	{
		text.clear();

		size_t length;
		if (const auto s = siv3dGetClipboardText(&length))
		{
			text = String{ s, length };
			::free(s);
		}

		return (not text.empty());
	}

	////////////////////////////////////////////////////////////////
	//
	//	hasText
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::hasText()
	{
		return siv3dHasClipboardText();
	}

	////////////////////////////////////////////////////////////////
	//
	//	setImage
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::setImage(const Image& image)
	{
		if (image.isEmpty())
		{
			return;
		}

		const auto png = image.encodePNG();
		siv3dSetClipboardImage(png.data(), png.size());
	}

	////////////////////////////////////////////////////////////////
	//
	//	getImage
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::getImage(Image& image, const PremultiplyAlpha premultiplyAlpha)
	{
		image.clear();

		size_t pngSize;
		if (const auto pngPtr = siv3dGetClipboardImage(&pngSize))
		{
			image = Image{ MemoryViewReader{ pngPtr, pngSize }, premultiplyAlpha, ImageFormat::PNG };
			::free(pngPtr);
		}

		return (not image.isEmpty());
	}

	////////////////////////////////////////////////////////////////
	//
	//	hasImage
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::hasImage()
	{
		return siv3dHasClipboardImage();
	}

	////////////////////////////////////////////////////////////////
	//
	//	setFilePaths
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::setFilePaths(const Array<FilePath>& paths)
	{
		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	getFilePaths
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::getFilePaths(Array<FilePath>& paths)
	{
		paths.clear();

		// Not supported

		return false;
	}

	////////////////////////////////////////////////////////////////
	//
	//	hasFilePaths
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::hasFilePaths()
	{
		return false;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setRichText
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::setRichText(const StringView richText, const Optional<StringView>& plainTextFallback)
	{
		// Not supported

		if (plainTextFallback)
		{
			setText(*plainTextFallback);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	setHTML
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::setHTML(const StringView html, const Optional<StringView>& plainTextFallback)
	{
		const auto plainText = plainTextFallback.value_or({});
		siv3dSetClipboardHTML(html.data(), html.size(), plainText.data(), plainText.size());
	}

	////////////////////////////////////////////////////////////////
	//
	//	setData
	//
	////////////////////////////////////////////////////////////////

	void CClipboard::setData(const StringView mimeType, const void* data, const size_t size, const Optional<StringView>& plainTextFallback)
	{
		const auto plainText = plainTextFallback.value_or({});
		siv3dSetClipboardData(mimeType.data(), mimeType.size(), data, size, plainText.data(), plainText.size());
	}

	////////////////////////////////////////////////////////////////
	//
	//	getData
	//
	////////////////////////////////////////////////////////////////

	bool CClipboard::getData(const StringView mimeType, Blob& data)
	{
		data.clear();

		if (not mimeType)
		{
			return false;
		}

		size_t dataSize;
		if (const auto dataPtr = siv3dGetClipboardData(mimeType.data(), mimeType.size(), &dataSize))
		{
			data.assign(dataPtr, dataSize);
			::free(dataPtr);
		}

		return (not data.isEmpty());
	}

	////////////////////////////////////////////////////////////////
	//
	//	getAvailableMimeTypes
	//
	////////////////////////////////////////////////////////////////

	Array<String> CClipboard::getAvailableMimeTypes()
	{
		Array<String> result;

		if (const auto mimeTypes = siv3dGetClipboardAvailableMimeTypes())
		{
			for (auto p = mimeTypes; *p; ++p)
			{
				result.emplace_back(*p);
			}
			::free(mimeTypes);
		}

		return result;
	}
}
