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

# include <Siv3D/System.hpp>
# include <Siv3D/FileSystem.hpp>

namespace s3d
{
	namespace System
	{
		extern "C"
		{
			void siv3dLaunchBrowser(const char32* ptr, size_t length);
		}

		////////////////////////////////////////////////////////////////
		//
		//	OpenInBrowser
		//
		////////////////////////////////////////////////////////////////

		bool OpenInBrowser(const URLView url)
		{
			String target;

			if (url.starts_with(U"http://") || url.starts_with(U"https://")) // Web ページ
			{
				target = url;
			}
			else // ローカルファイル
			{
				const String extension = FileSystem::Extension(url); // 小文字の拡張子を取得

				const bool isHTML = ((extension == U"html") || (extension == U"htm"));

				if (not isHTML)
				{
					return false;
				}

				target = FileSystem::FullPath(url);
			}

			siv3dLaunchBrowser(target.data(), target.size());

			return true;
		}

		////////////////////////////////////////////////////////////////
		//
		//	ShowInFileManager
		//
		////////////////////////////////////////////////////////////////

		bool ShowInFileManager(const FilePathView)
		{
			return false;
		}

		////////////////////////////////////////////////////////////////
		//
		//	LaunchFile
		//
		////////////////////////////////////////////////////////////////

		bool LaunchFile(const FilePathView)
		{
			return false;
		}

		////////////////////////////////////////////////////////////////
		//
		//	LaunchFileWithTextEditor
		//
		////////////////////////////////////////////////////////////////

		bool LaunchFileWithTextEditor(const FilePathView)
		{
			return false;
		}
	}
}
