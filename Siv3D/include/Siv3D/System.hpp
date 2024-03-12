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
# include "Duration.hpp"

namespace s3d
{
	namespace System
	{
		/// @brief 描画や入力情報など、フレームを更新します。
		/// @remarkアプリケーション終了トリガーが発生するか、内部で回復不能なエラーが発生した場合に false を返します。この関数が false を返したらプログラムを終了させるべきです。
		/// @return プログラムの続行の可否
		bool Update();

		/// @brief 現在のスレッドの実行を指定した時間だけ停止します。
		/// @param milliseconds スリープする時間（ミリ秒）
		void Sleep(int32 milliseconds);

		/// @brief 現在のスレッドの実行を指定した時間だけ停止します。
		/// @param duration スリープする時間
		void Sleep(const Duration& duration);
	}
}
