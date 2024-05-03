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
//# include "Array.hpp"
# include "InputGroups.hpp"

namespace s3d
{
	namespace Mouse
	{
		inline constexpr size_t NumButtons = 8;

		////////////////////////////////////////////////////////////////
		//
		//	GetAllInputs
		//
		////////////////////////////////////////////////////////////////

		/// @brief 現在のフレームで何らかの入力操作があるマウスのボタンの一覧を取得します。
		/// @return 現在のフレームで何らかの入力操作があるマウスのボタンの一覧
		[[nodiscard]]
		const Array<Input>& GetAllInputs() noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	ClearLRInput
		//
		////////////////////////////////////////////////////////////////

		/// @brief 現在のフレームで、以降の MouseL / MouseR の down() と pressed() を強制的に false にします。
		void ClearLRInput();

		////////////////////////////////////////////////////////////////
		//
		//	Wheel
		//
		////////////////////////////////////////////////////////////////

		/// @brief マウスホイールのスクロール量を返します。
		/// @return マウスホイールのスクロール量
		[[nodiscard]]
		double Wheel() noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	WheelH
		//
		////////////////////////////////////////////////////////////////

		/// @brief マウスの水平ホイールのスクロール量を返します。
		/// @return マウスの水平ホイールのスクロール量
		[[nodiscard]]
		double WheelH() noexcept;
	}

	/// @brief マウス左ボタン
	inline constexpr Input MouseL{ InputDevice::Mouse, 0 };

	/// @brief マウス右ボタン
	inline constexpr Input MouseR{ InputDevice::Mouse, 1 };

	/// @brief マウス中央ボタン
	inline constexpr Input MouseM{ InputDevice::Mouse, 2 };

	/// @brief マウス X1 ボタン
	inline constexpr Input MouseX1{ InputDevice::Mouse, 3 };

	/// @brief マウス X2 ボタン
	inline constexpr Input MouseX2{ InputDevice::Mouse, 4 };

	/// @brief マウス X3 ボタン
	inline constexpr Input MouseX3{ InputDevice::Mouse, 5 };

	/// @brief マウス X4 ボタン
	inline constexpr Input MouseX4{ InputDevice::Mouse, 6 };

	/// @brief マウス X5 ボタン
	inline constexpr Input MouseX5{ InputDevice::Mouse, 7 };
}
