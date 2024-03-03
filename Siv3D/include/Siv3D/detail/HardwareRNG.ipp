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

namespace s3d
{		
	////////////////////////////////////////////////////////////////
	//
	//	min, max
	//
	////////////////////////////////////////////////////////////////

	constexpr HardwareRNG::result_type HardwareRNG::min() noexcept
	{
		return engine_type::min();
	}

	constexpr HardwareRNG::result_type HardwareRNG::max() noexcept
	{
		return engine_type::max();
	}
		
	////////////////////////////////////////////////////////////////
	//
	//	operator ()
	//
	////////////////////////////////////////////////////////////////

	inline HardwareRNG::result_type HardwareRNG::operator ()()
	{
		return m_engine();
	}
		
	////////////////////////////////////////////////////////////////
	//
	//	generateSeeds
	//
	////////////////////////////////////////////////////////////////

	template <size_t N>
	std::array<HardwareRNG::result_type, N> HardwareRNG::generateSeeds() noexcept
	{
		std::array<result_type, N> seeds;

		for (auto& seed : seeds)
		{
			seed = m_engine();
		}

		return seeds;
	}
}