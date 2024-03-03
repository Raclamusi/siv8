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
# include "Types.hpp"

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	LogType
	//
	////////////////////////////////////////////////////////////////

	/// @brief ログの種類 | The type of the log message
	enum class LogType : uint8
	{
		////////////////////////////////////////////////////////////////
		//
		//	LogType::Error
		//
		////////////////////////////////////////////////////////////////

		/// @brief エラー | Error
		Error,

		////////////////////////////////////////////////////////////////
		//
		//	LogType::Fail
		//
		////////////////////////////////////////////////////////////////

		/// @brief 失敗 | Fail
		Fail,

		////////////////////////////////////////////////////////////////
		//
		//	LogType::Warning
		//
		////////////////////////////////////////////////////////////////

		/// @brief 警告 | Warning
		Warning,

		////////////////////////////////////////////////////////////////
		//
		//	LogType::App
		//
		////////////////////////////////////////////////////////////////

		/// @brief アプリケーション | Application
		App,

		////////////////////////////////////////////////////////////////
		//
		//	LogType::Info
		//
		////////////////////////////////////////////////////////////////

		/// @brief 情報 | Information
		Info,

		////////////////////////////////////////////////////////////////
		//
		//	LogType::Trace
		//
		////////////////////////////////////////////////////////////////

		/// @brief トレース | Trace
		/// @remark LogLevel::Debug 以上でのみ出力されます。 | Output only when LogLevel::Debug or higher.
		Trace,

		////////////////////////////////////////////////////////////////
		//
		//	LogType::Verbose
		//
		////////////////////////////////////////////////////////////////

		/// @brief 詳細 | Verbose
		/// @remark LogLevel::Verbose でのみ出力されます。 | Output only when LogLevel::Verbose.
		Verbose,
	};
}