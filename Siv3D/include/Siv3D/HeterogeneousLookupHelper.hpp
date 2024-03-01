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
# include "StringView.hpp"
# include "String.hpp"

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	StringCompare
	//
	////////////////////////////////////////////////////////////////

	struct StringCompare
	{
		using is_transparent = void;

		constexpr bool operator ()(const char32* key, const char32* s) const noexcept { return (StringView{ key } == s); }
		constexpr bool operator ()(const char32* key, StringView s) const noexcept { return (key == s); }
		constexpr bool operator ()(const char32* key, const String& s) const noexcept { return (key == s); }

		constexpr bool operator ()(StringView key, const char32* s) const noexcept { return (key == s); }
		constexpr bool operator ()(StringView key, StringView s) const noexcept { return (key == s); }
		constexpr bool operator ()(StringView key, const String& s) const noexcept { return (key == s); }

		constexpr bool operator ()(const String& key, const char32* s) const noexcept { return (key == s); }
		constexpr bool operator ()(const String& key, StringView s) const noexcept { return (key == s); }
		constexpr bool operator ()(const String& key, const String& s) const noexcept { return (key == s); }
	};

	////////////////////////////////////////////////////////////////
	//
	//	StringHash
	//
	////////////////////////////////////////////////////////////////

	struct StringHash
	{
		using is_transparent = void;
		using transparent_key_equal = StringCompare;
		using hash_type = std::hash<StringView>;
		
		size_t operator ()(StringView s) const noexcept { return hash_type{}(s); }
		size_t operator ()(const String& s) const noexcept { return hash_type{}(s); }
		size_t operator ()(const char32* s) const noexcept { return hash_type{}(s); }
	};
}
