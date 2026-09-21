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

# include <Siv3D/UserInfo.hpp>
# include <Siv3D/Unicode.hpp>
# include <cstdlib>
# include <clocale>

namespace s3d
{
	namespace
	{
		[[nodiscard]]
		static String ComputerName()
		{
			return U"emscripten";
		}

		[[nodiscard]]
		static String UserName()
		{
			if (const char* username = std::getenv("USER"))
			{
				return Unicode::FromUTF8(username);
			}
			else if (const char* username = std::getenv("USERNAME"))
			{
				return Unicode::FromUTF8(username);
			}
			else if (const char* username = std::getenv("LOGNAME"))
			{
				return Unicode::FromUTF8(username);
			}
			else
			{
				return{};
			}
		}

		[[nodiscard]]
		static String FullUserName()
		{
			return U"";
		}

		[[nodiscard]]
		static String DefaultLocale()
		{
			if (const char* locale = std::setlocale(LC_ALL, ""))
			{
				return Unicode::FromUTF8(locale).replace(U'_', U'-');
			}
			else
			{
				return U"en-US";
			}
		}

		[[nodiscard]]
		static String DefaultLanguage()
		{
			if (const char* language = std::getenv("LANG"))
			{
				return Unicode::FromUTF8(language).replace(U'_', U'-');
			}
			else
			{
				return U"en-US";
			}
		}
	}

	namespace System
	{
		////////////////////////////////////////////////////////////////
		//
		//	GetUserInfo
		//
		////////////////////////////////////////////////////////////////

		const UserInfo& GetUserInfo()
		{
			static const UserInfo userInfo = []()
			{
				UserInfo info;
				info.computerName		= ComputerName();
				info.userName			= UserName();
				info.fullUserName		= FullUserName();
				info.defaultLocale		= DefaultLocale();
				info.defaultLanguage	= DefaultLanguage();
				return info;
			}();

			return userInfo;
		}

		////////////////////////////////////////////////////////////////
		//
		//	IsRunningInVisualStudio
		//
		////////////////////////////////////////////////////////////////

		bool IsRunningInVisualStudio()
		{
			return false;
		}

		////////////////////////////////////////////////////////////////
		//
		//	IsRunningInXcode
		//
		////////////////////////////////////////////////////////////////

		bool IsRunningInXcode()
		{
			return false;
		}
	}
}
