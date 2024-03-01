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
	//	get
	//
	////////////////////////////////////////////////////////////////

	inline char32 UTF8toUTF32_Converter::get() const noexcept
	{
		return m_result;
	}

	inline char32 UTF16toUTF32_Converter::get() const noexcept
	{
		return m_result;
	}

	inline std::array<char8, 4> UTF32toUTF8_Converter::get() const noexcept
	{
		return m_buffer;
	}

	inline std::array<char8, 4>::const_iterator UTF32toUTF8_Converter::begin() const noexcept
	{
		return m_buffer.begin();
	}

	inline std::array<char16, 2> UTF32toUTF16_Converter::get() const noexcept
	{
		return m_buffer;
	}

	inline std::array<char16, 2>::const_iterator UTF32toUTF16_Converter::begin() const noexcept
	{
		return m_buffer.begin();
	}
}
