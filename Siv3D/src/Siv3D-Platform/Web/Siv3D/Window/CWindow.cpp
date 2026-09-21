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

# include "CWindow.hpp"
# include <Siv3D/UserAction.hpp>
# include <Siv3D/Math.hpp>
# include <Siv3D/UserAction/IUserAction.hpp>
# include <Siv3D/Renderer/IRenderer.hpp>
# include <Siv3D/Error/InternalEngineError.hpp>
# include <Siv3D/Engine/Siv3DEngine.hpp>
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	extern "C"
	{
		void siv3dRequestFullscreen();
		void siv3dExitFullscreen();
		bool siv3dIsFullscreen();
		void siv3dGetWindowBounds(Rect* rect);
	}

	namespace
	{
		[[nodiscard]]
		CWindow* GetWindow(GLFWwindow* glfwWindow)
		{
			return static_cast<CWindow*>(::glfwGetWindowUserPointer(glfwWindow));
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	CWindow::~CWindow()
	{
		LOG_SCOPED_DEBUG("CWindow::~CWindow()");

		::glfwTerminate();
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void CWindow::init()
	{
		LOG_SCOPED_DEBUG("CWindow::init()");

		// Only WindowStyle::Sizable is supported on a web browser
		m_state.style = WindowStyle::Sizable;

		if (not ::glfwInit())
		{
			throw InternalEngineError{ "glfwInit() failed" };
		}

		{
			// OpenGL ES 3.0
			::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
			::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
			::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, (SIV3D_BUILD(DEBUG) ? GLFW_TRUE : GLFW_FALSE));
		}

		// ウィンドウを作成
		{
			m_glfwWindow = ::glfwCreateWindow(800, 600, "", nullptr, nullptr);

			if (not m_glfwWindow)
			{
				throw InternalEngineError{ U"glfwCreateWindow() failed. Your browser seems not to support WebGL 2.0, please enable WebGL 2.0." };
			}
		}

		m_windowTitle.refresh(m_glfwWindow);

		::glfwSetWindowUserPointer(m_glfwWindow, this);

		// TODO: GLFW が対応しないコールバックを調査
		::glfwSetWindowPosCallback(m_glfwWindow, CWindow::OnMove);
		::glfwSetWindowSizeCallback(m_glfwWindow, CWindow::OnResize);
		::glfwSetFramebufferSizeCallback(m_glfwWindow, CWindow::OnFrameBufferSize);
		::glfwSetWindowContentScaleCallback(m_glfwWindow, CWindow::OnScalingChange);
		::glfwSetWindowIconifyCallback(m_glfwWindow, CWindow::OnIconify);
		::glfwSetWindowMaximizeCallback(m_glfwWindow, CWindow::OnMaximize);
		::glfwSetWindowFocusCallback(m_glfwWindow, CWindow::OnFocus);

		updateState();
	}

	////////////////////////////////////////////////////////////////
	//
	//	update
	//
	////////////////////////////////////////////////////////////////

	void CWindow::update()
	{
		::glfwPollEvents();

		updateState();

		if constexpr (SIV3D_BUILD(DEBUG))
		{
			m_windowTitle.refresh(m_glfwWindow);
		}

		if (::glfwWindowShouldClose(m_glfwWindow))
		{
			SIV3D_ENGINE(UserAction)->reportUserActions(UserAction::CloseButtonClicked);
			::glfwSetWindowShouldClose(m_glfwWindow, GLFW_FALSE);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	setWindowTitle
	//
	////////////////////////////////////////////////////////////////

	void CWindow::setWindowTitle(const String& title)
	{
		m_windowTitle.set(m_glfwWindow, title);
	}

	////////////////////////////////////////////////////////////////
	//
	//	getWindowTitle
	//
	////////////////////////////////////////////////////////////////

	const String& CWindow::getWindowTitle() const noexcept
	{
		return m_windowTitle.title;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getHandle
	//
	////////////////////////////////////////////////////////////////

	void* CWindow::getHandle() const noexcept
	{
		return m_glfwWindow;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getState
	//
	////////////////////////////////////////////////////////////////

	const WindowState& CWindow::getState() const noexcept
	{
		return m_state;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setStyle
	//
	////////////////////////////////////////////////////////////////

	void CWindow::setStyle(const WindowStyle style)
	{
		LOG_SCOPED_DEBUG(fmt::format("CWindow::setStyle(style = {})", FromEnum(style)));

		if (m_state.fullscreen)
		{
			LOG_FAIL("A window in fullscreen mode cannot change its style");
			return;
		}

		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	setPos
	//
	////////////////////////////////////////////////////////////////

	void CWindow::setPos(const Point pos)
	{
		LOG_SCOPED_DEBUG(fmt::format("CWindow::setPos(pos = {})", pos));

		if (m_state.fullscreen)
		{
			LOG_FAIL("A window in fullscreen mode cannot be moved");
			return;
		}

		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	show
	//
	////////////////////////////////////////////////////////////////

	void CWindow::show() {} // do nothing

	////////////////////////////////////////////////////////////////
	//
	//	maximize
	//
	////////////////////////////////////////////////////////////////

	void CWindow::maximize()
	{
		LOG_DEBUG("CWindow::maximize()");

		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	restore
	//
	////////////////////////////////////////////////////////////////

	void CWindow::restore()
	{
		LOG_DEBUG("CWindow::restore()");

		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	minimize
	//
	////////////////////////////////////////////////////////////////

	void CWindow::minimize()
	{
		LOG_DEBUG("CWindow::minimize()");

		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	resizeByVirtualSize
	//
	////////////////////////////////////////////////////////////////

	bool CWindow::resizeByVirtualSize(const Size virtualSize)
	{
		LOG_DEBUG(fmt::format("CWindow::resizeByVirtualSize(size = {})", virtualSize));

		::glfwSetWindowSize(m_glfwWindow, virtualSize.x, virtualSize.y);

		updateState();

		return true;
	}

	////////////////////////////////////////////////////////////////
	//
	//	resizeByFrameBufferSize
	//
	////////////////////////////////////////////////////////////////

	bool CWindow::resizeByFrameBufferSize(const Size frameBufferSize)
	{
		LOG_DEBUG(fmt::format("CWindow::resizeByFrameBufferSize(size = {})", frameBufferSize));

		const double scaling = m_state.scaling;
		const Size newVirtualSize = Math::Round(frameBufferSize / scaling).asPoint();
		return resizeByVirtualSize(newVirtualSize);
	}

	////////////////////////////////////////////////////////////////
	//
	//	setMinimumFrameBufferSize
	//
	////////////////////////////////////////////////////////////////

	void CWindow::setMinimumFrameBufferSize(const Size size)
	{
		LOG_DEBUG(fmt::format("CWindow::setMinimumFrameBufferSize(size = {})", size));

		m_state.minFrameBufferSize = size;

		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	setFullscreen
	//
	////////////////////////////////////////////////////////////////

	void CWindow::setFullscreen(const bool fullscreen, const size_t monitorIndex)
	{
		LOG_DEBUG(fmt::format("CWindow::setFullscreen(fullscreen = {}, monitorIndex = {})", fullscreen, monitorIndex));

		if (fullscreen == m_state.fullscreen)
		{
			return;
		}

		if (m_state.fullscreen == false) // 現在ウィンドウモード
		{
			siv3dRequestFullscreen();
		}
		else
		{
			siv3dExitFullscreen();
		}

		m_state.fullscreen = fullscreen;

		updateState();

		if (Scene::GetResizeMode() != ResizeMode::Keep)
		{
			SIV3D_ENGINE(Renderer)->updateSceneSize();
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	setToggleFullscreenEnabled
	//
	////////////////////////////////////////////////////////////////

	void CWindow::setToggleFullscreenEnabled(const bool) {}

	////////////////////////////////////////////////////////////////
	//
	//	isToggleFullscreenEnabled
	//
	////////////////////////////////////////////////////////////////

	bool CWindow::isToggleFullscreenEnabled() const
	{
		return false;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setTaskbarProgressBar
	//
	////////////////////////////////////////////////////////////////

	void CWindow::setTaskbarProgressBar(const double)
	{
		// Not supported
	}

	////////////////////////////////////////////////////////////////
	//
	//	updateState
	//
	////////////////////////////////////////////////////////////////

	void CWindow::updateState()
	{
		// frameBufferSize
		::glfwGetFramebufferSize(m_glfwWindow, &m_state.frameBufferSize.x, &m_state.frameBufferSize.y);

		// scaling
		float xScale, yScale;
		::glfwGetWindowContentScale(m_glfwWindow, &xScale, &yScale);
		m_state.scaling = Max(xScale, yScale);

		// titleBarHeight
		// m_state.titleBarHeight = 0;  // Not supported

		// bounds
		siv3dGetWindowBounds(&m_state.bounds);

		// minimized
		// m_state.minimized = false;  // Not supported

		// maximized
		// m_state.maximized = false;  // Not supported

		// focused
		// m_state.focused = false;  // TODO

		// sizeMove
		if (m_state.sizeMove)
		{
			if (SecondsF{ 0.2 } < m_moveResizeStabilizer)
			{
				m_state.sizeMove = false;
				m_moveResizeStabilizer.reset();
			}
		}

		// fullscreen
		m_state.fullscreen = siv3dIsFullscreen();
	}

	////////////////////////////////////////////////////////////////
	//
	//	OnMove
	//
	////////////////////////////////////////////////////////////////

	void CWindow::OnMove(GLFWwindow* glfwWindow, const int flag, const int)
	{
		const bool move = (flag == 1);
		LOG_DEBUG(fmt::format("CWindow::OnMove({})", move));

		if (CWindow* pWindow = GetWindow(glfwWindow))
		{
			pWindow->m_state.sizeMove = true;
			pWindow->m_moveResizeStabilizer.restart();
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	OnResize
	//
	////////////////////////////////////////////////////////////////

	void CWindow::OnResize(GLFWwindow* glfwWindow, const int width, const int height)
	{
		const Size size{ width, height };
		LOG_DEBUG(fmt::format("CWindow::OnResize({})", size));

		if (CWindow* pWindow = GetWindow(glfwWindow))
		{
			pWindow->m_state.bounds.size = size;
			pWindow->m_state.virtualSize = size;
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	OnFrameBufferSize
	//
	////////////////////////////////////////////////////////////////

	void CWindow::OnFrameBufferSize(GLFWwindow*, const int width, const int height)
	{
		const Size size{ width, height };
		LOG_DEBUG(fmt::format("CWindow::OnFrameBufferSize({})", size));
	}

	////////////////////////////////////////////////////////////////
	//
	//	OnScalingChange
	//
	////////////////////////////////////////////////////////////////

	void CWindow::OnScalingChange(GLFWwindow*, const float sx, const float sy)
	{
		LOG_DEBUG(fmt::format("CWindow::OnScalingChange({}, {})", sx, sy));
	}

	////////////////////////////////////////////////////////////////
	//
	//	OnIconify
	//
	////////////////////////////////////////////////////////////////

	void CWindow::OnIconify(GLFWwindow* glfwWindow, const int iconified)
	{
		LOG_DEBUG(fmt::format("CWindow::OnIconify({})", (iconified == GLFW_TRUE)));

		if (CWindow* pWindow = GetWindow(glfwWindow))
		{
			if (iconified) // 最小化
			{
				pWindow->m_state.minimized = true;
				pWindow->m_state.maximized = false;
			}
			else // 最小化解除
			{
				pWindow->m_state.minimized = false;
				pWindow->m_state.maximized = false;
			}
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	OnMaximize
	//
	////////////////////////////////////////////////////////////////

	void CWindow::OnMaximize(GLFWwindow* glfwWindow, const int maximized)
	{
		LOG_DEBUG(fmt::format("CWindow::OnMaximize({})", (maximized == GLFW_TRUE)));

		if (CWindow* pWindow = GetWindow(glfwWindow))
		{
			if (maximized) // 最大化
			{
				pWindow->m_state.minimized = false;
				pWindow->m_state.maximized = true;
			}
			else // 最大化解除
			{
				pWindow->m_state.minimized = false;
				pWindow->m_state.maximized = false;
			}
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	OnFocus
	//
	////////////////////////////////////////////////////////////////

	void CWindow::OnFocus(GLFWwindow* glfwWindow, const int focused)
	{
		const bool hasFocus = (focused == GLFW_TRUE);
		LOG_DEBUG(fmt::format("CWindow::OnFocus({})", hasFocus));

		if (CWindow* pWindow = GetWindow(glfwWindow))
		{
			pWindow->m_state.focused = hasFocus;

			if (not hasFocus) // フォーカスが外れた場合
			{
				SIV3D_ENGINE(UserAction)->reportUserActions(UserAction::WindowDeactivated);
			}
		}
	}
}
