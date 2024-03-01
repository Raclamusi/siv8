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
	//	CopyOption
	//
	////////////////////////////////////////////////////////////////

	/// @brief ファイルコピー時の動作
	enum class CopyOption
	{
		/// @brief ファイル名が既に使われていた場合、コピーを失敗させる。 | Report an error when the file already exists.
		Default,

		/// @brief ファイル名が既に使われていた場合、エラーを発生させずにスキップする。 | Keep the existing file, without reporting an error.
		SkipExisting,

		/// @brief ファイル名が既に使われていた場合、ファイルを上書きする。 | Replace the existing file.
		OverwriteExisting,

		/// @brief ファイルが既に存在する場合、コピーするファイルが新しければ上書きする。 | Replace the existing file only if it is older than the file being copied.
		UpdateExisting,
	};
}
