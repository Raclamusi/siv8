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

# include <chrono>
# include <sys/time.h>
# include <Siv3D/Time.hpp>

namespace s3d
{
	namespace
	{
		[[nodiscard]]
		static uint64 GetClockNanosec() noexcept
		{
			using namespace std::chrono;
			return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
		}

		const static uint64 g_nanosecBase = GetClockNanosec();
	}

	namespace Time
	{
		int64 GetSec() noexcept
		{
			return (GetNanosec() / 1'000'000'000);
		}

		int64 GetMillisec() noexcept
		{
			return (GetNanosec() / 1'000'000);
		}

		int64 GetMicrosec() noexcept
		{
			return (GetNanosec() / 1'000);
		}

		int64 GetNanosec() noexcept
		{
			return static_cast<int64>(GetClockNanosec() - g_nanosecBase);
		}

		int64 GetUptimeSec() noexcept
		{
			return static_cast<int64>(GetClockNanosec() / 1'000'000'000);
		}

		int64 GetUptimeMillisec() noexcept
		{
			return static_cast<int64>(GetClockNanosec() / 1'000'000);
		}

		uint64 GetSecSinceEpoch() noexcept
		{
			return (GetMicrosecSinceEpoch() / 1'000'000);
		}

		uint64 GetMillisecSinceEpoch() noexcept
		{
			return (GetMicrosecSinceEpoch() / 1'000);
		}

		uint64 GetMicrosecSinceEpoch() noexcept
		{
			::timeval tv;
			::gettimeofday(&tv, nullptr);
			return (tv.tv_sec * 1'000'000ULL + tv.tv_usec);
		}

		int32 UTCOffsetMinutes() noexcept
		{
			struct ::timezone tz;
			::gettimeofday(nullptr, &tz);
			return -tz.tz_minuteswest;
		}
	}
}
