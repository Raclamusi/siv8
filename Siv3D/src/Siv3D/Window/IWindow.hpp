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
# include <Siv3D/Common.hpp>

namespace s3d
{
	class String;
	struct WindowState;
	enum class WindowStyle : uint8;
	struct Point;

	class SIV3D_NOVTABLE ISiv3DWindow
	{
	public:

		[[nodiscard]]
		static ISiv3DWindow* Create();

		virtual ~ISiv3DWindow() = default;

		virtual void init() = 0;

		virtual void update() = 0;

		virtual void setWindowTitle(const String& title) = 0;

		virtual const String& getWindowTitle() const noexcept = 0;

		virtual void* getHandle() const noexcept = 0;
	
		virtual const WindowState& getState() const noexcept = 0;

		virtual void setStyle(WindowStyle style) = 0;

		virtual void setPos(Point pos) = 0;

		virtual void show() = 0;

		virtual void maximize() = 0;

		virtual void restore() = 0;

		virtual void minimize() = 0;
	};
}
