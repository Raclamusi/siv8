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

# pragma once
# include "Common.hpp"

namespace s3d
{
	namespace Threading
	{
		////////////////////////////////////////////////////////////////
		//
		//	GetConcurrency
		//
		////////////////////////////////////////////////////////////////

		/// @brief サポートされるスレッド数を返します。 | Returns the number of concurrent threads supported by the implementation.
		/// @return サポートされるスレッド数 | Number of concurrent threads supported
		[[nodiscard]]
		size_t GetConcurrency() noexcept;
	}
}
