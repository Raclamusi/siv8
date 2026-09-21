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

# include <Siv3D/MessageBox.hpp>

namespace s3d
{
	extern "C"
	{
		void siv3dShowMessageBoxOK(const char32* ptr, size_t length);
		bool siv3dShowMessageBoxOKCancel(const char32* ptr, size_t length);
	}

	namespace System
	{
		MessageBoxResult MessageBoxOK(const StringView, const StringView text, const MessageBoxStyle)
		{
			siv3dShowMessageBoxOK(text.data(), text.size());
			return MessageBoxResult::OK;
		}

		MessageBoxResult MessageBoxOKCancel(const StringView, const StringView text, const MessageBoxStyle)
		{
			const bool ok = siv3dShowMessageBoxOKCancel(text.data(), text.size());
			return (ok ? MessageBoxResult::OK : MessageBoxResult::Cancel);
		}

		MessageBoxResult MessageBoxYesNo(const StringView, const StringView text, const MessageBoxStyle)
		{
			// JavaScript does not have Yes/No dialog
			const bool ok = siv3dShowMessageBoxOKCancel(text.data(), text.size());
			return (ok ? MessageBoxResult::Yes : MessageBoxResult::No);
		}
	}
}
