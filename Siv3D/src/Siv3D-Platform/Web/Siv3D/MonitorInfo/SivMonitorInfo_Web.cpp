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

# include <Siv3D/MonitorInfo.hpp>

namespace s3d
{
	namespace System
	{
		////////////////////////////////////////////////////////////////
		//
		//	EnumerateMonitors
		//
		////////////////////////////////////////////////////////////////

		Array<MonitorInfo> EnumerateMonitors()
		{
			// TODO
			return { MonitorInfo{} };
		}

		////////////////////////////////////////////////////////////////
		//
		//	GetCurrentMonitorIndex
		//
		////////////////////////////////////////////////////////////////

		size_t GetCurrentMonitorIndex()
		{
			return 0;
		}
	}
}
