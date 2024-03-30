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

# include <Siv3D/Common.hpp>
# include <Siv3D/DateTime.hpp>
# include <Siv3D/Windows/MinWindows.hpp>

namespace s3d
{
	namespace
	{
		[[nodiscard]]
		static constexpr DateTime ToDateTime(const SYSTEMTIME& in) noexcept
		{
			return{ in.wYear, in.wMonth, in.wDay, in.wHour, in.wMinute, in.wSecond, in.wMilliseconds };
		}
	}

	DateTime DateTime::Now() noexcept
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		return ToDateTime(sysTime);
	}

	DateTime DateTime::NowUTC() noexcept
	{
		SYSTEMTIME sysTime;
		::GetSystemTime(&sysTime);
		return ToDateTime(sysTime);
	}
}
