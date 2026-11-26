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

# include "CCursor.hpp"
# include <Siv3D/EngineLog.hpp>
# include <Siv3D/WindowState.hpp>
# include <Siv3D/Window/IWindow.hpp>
# include <Siv3D/Renderer/IRenderer.hpp>
# include <Siv3D/Engine/Siv3DEngine.hpp>

namespace s3d
{
	extern "C"
	{
		void siv3dGetCursorPos(double* x, double* y);
	}

	namespace
	{
		[[nodiscard]]
		static Point GetCursorScreenPos()
		{
			double x, y;
			siv3dGetCursorPos(&x, &y);
			return Point{ static_cast<int32>(x), static_cast<int32>(y) };
		}

		[[nodiscard]]
		static Point GetCursorRawClientPos(const Point screenPos)
		{
			const WindowState windowState = SIV3D_ENGINE(Window)->getState();
			return (screenPos - windowState.bounds.pos - Point{ 0, windowState.titleBarHeight });
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	~CCursor
	//
	////////////////////////////////////////////////////////////////

	CCursor::~CCursor()
	{
		LOG_SCOPED_DEBUG("CCursor::~CCursor()");
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void CCursor::init()
	{
		LOG_SCOPED_DEBUG("CCursor::init()");

		{
			const Point screenPos = GetCursorScreenPos();
			const Point rawClientPos = GetCursorRawClientPos(screenPos);
			updateHighTemporalResolutionCursorPos(rawClientPos);

			const double windowScaling = SIV3D_ENGINE(Window)->getState().scaling;
			m_transform.setBaseWindow({ windowScaling, RectF{} });
			m_state.advanceRaw(screenPos, rawClientPos);
			m_state.refreshTransformed(m_transform);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	updateHighTemporalResolutionCursorPos
	//
	////////////////////////////////////////////////////////////////

	void CCursor::updateHighTemporalResolutionCursorPos(const Point rawClientPos)
	{
		m_highTemporalResolutionCursor.add(rawClientPos);
	}

	////////////////////////////////////////////////////////////////
	//
	//	update
	//
	////////////////////////////////////////////////////////////////

	void CCursor::update()
	{
		if (m_clippedToWindow)
		{
			// [Siv3D ToDo]
		}

		m_highTemporalResolutionCursor.update();

		{
			m_transform.setBaseWindow(SIV3D_ENGINE(Renderer)->getLetterboxComposition());
		}

		{
			const Point screenPos = GetCursorScreenPos();
			const Point rawClientPos = GetCursorRawClientPos(screenPos);
			updateHighTemporalResolutionCursorPos(rawClientPos);

			m_state.advanceRaw(screenPos, rawClientPos);
			m_state.refreshTransformed(m_transform);
		}

		m_captured = false;
	}

	////////////////////////////////////////////////////////////////
	//
	//	updateBaseWindowTransform
	//
	////////////////////////////////////////////////////////////////

	void CCursor::updateBaseWindowTransform()
	{
		if (m_transform.setBaseWindow(SIV3D_ENGINE(Renderer)->getLetterboxComposition()))
		{
			m_state.refreshTransformed(m_transform);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	getState
	//
	////////////////////////////////////////////////////////////////

	const CursorState& CCursor::getState() const
	{
		return m_state;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getHighTemporalResolutionCursorPos
	//
	////////////////////////////////////////////////////////////////

	Array<std::pair<int64, Point>> CCursor::getHighTemporalResolutionCursorPos() const
	{
		return m_highTemporalResolutionCursor.get();
	}

	////////////////////////////////////////////////////////////////
	//
	//	setPos
	//
	////////////////////////////////////////////////////////////////

	void CCursor::setPos(const Point)
	{
		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	getBaseWindowTransform
	//
	////////////////////////////////////////////////////////////////

	const Mat3x2& CCursor::getBaseWindowTransform() const noexcept
	{
		return m_transform.baseWindow;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getCameraTransform
	//
	////////////////////////////////////////////////////////////////

	const Mat3x2& CCursor::getCameraTransform() const noexcept
	{
		return m_transform.camera;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getLocalTransform
	//
	////////////////////////////////////////////////////////////////

	const Mat3x2& CCursor::getLocalTransform() const noexcept
	{
		return m_transform.local;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setCameraTransform
	//
	////////////////////////////////////////////////////////////////

	void CCursor::setCameraTransform(const Mat3x2& matrix)
	{
		if (m_transform.setCamera(matrix))
		{
			m_state.refreshTransformed(m_transform);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	setLocalTransform
	//
	////////////////////////////////////////////////////////////////

	void CCursor::setLocalTransform(const Mat3x2& matrix)
	{
		if (m_transform.setLocal(matrix))
		{
			m_state.refreshTransformed(m_transform);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	isClippedToWindow
	//
	////////////////////////////////////////////////////////////////

	bool CCursor::isClippedToWindow() const noexcept
	{
		return m_clippedToWindow;
	}

	////////////////////////////////////////////////////////////////
	//
	//	clipToWindow
	//
	////////////////////////////////////////////////////////////////

	void CCursor::clipToWindow(const bool clip)
	{
		if (clip == m_clippedToWindow)
		{
			return;
		}

		m_clippedToWindow = clip;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setCapture
	//
	////////////////////////////////////////////////////////////////

	void CCursor::setCapture(const bool captured) noexcept
	{
		m_captured = captured;
	}

	////////////////////////////////////////////////////////////////
	//
	//	isCaptured
	//
	////////////////////////////////////////////////////////////////

	bool CCursor::isCaptured() const noexcept
	{
		return m_captured;
	}
}
