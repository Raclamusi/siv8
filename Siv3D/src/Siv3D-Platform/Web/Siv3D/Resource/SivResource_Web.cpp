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

# include <Siv3D/Resource.hpp>

namespace s3d
{
	namespace detail::init
	{
		[[nodiscard]]
		const Array<FilePath>& GetResourceFiles() noexcept;
	}

	const Array<FilePath>& EnumResourceFiles() noexcept
	{
		return detail::init::GetResourceFiles();
	}

	FilePath Resource(const FilePathView path)
	{
		if (path.starts_with(U"/resources/"))
		{
			return FilePath{ path };
		}

		return (U"/resources/" + path);
	}
}
