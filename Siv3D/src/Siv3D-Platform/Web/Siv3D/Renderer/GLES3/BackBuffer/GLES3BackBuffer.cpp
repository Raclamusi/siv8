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

# include "GLES3BackBuffer.hpp"
# include <Siv3D/Graphics.hpp>
# include <Siv3D/Window.hpp>
# include <Siv3D/WindowState.hpp>
# include <Siv3D/SamplerState.hpp>
# include <Siv3D/Error/InternalEngineError.hpp>
# include <Siv3D/Shader/IShader.hpp>
# include <Siv3D/Scene/SceneUtility.hpp>
# include <Siv3D/Cursor/ICursor.hpp>
# include <Siv3D/Renderer2D/IRenderer2D.hpp>
# include <Siv3D/EngineShader/IEngineShader.hpp>
# include <Siv3D/Renderer/GLES3/CRenderer_GLES3.hpp>
# include <Siv3D/Shader/GLES3/CShader_GLES3.hpp>
# include <Siv3D/Engine/Siv3DEngine.hpp>
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3BackBuffer::~GLES3BackBuffer()
	{
		if (m_fullScreenTriangleSampler)
		{
			::glDeleteSamplers(1, &m_fullScreenTriangleSampler);
			m_fullScreenTriangleSampler = 0;
		}

		if (m_fullScreenTriangleVAO)
		{
			::glDeleteVertexArrays(1, &m_fullScreenTriangleVAO);
			m_fullScreenTriangleVAO = 0;
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::init(CRenderer_GLES3* pRenderer)
	{
		LOG_SCOPED_DEBUG("GLES3BackBuffer::init()");
		LOG_INFO(fmt::format("ℹ️ Scene MSAA: x{}", m_sceneBuffers.sampleCount));

		m_pRenderer = pRenderer;

		m_pShader = static_cast<CShader_GLES3*>(SIV3D_ENGINE(Shader));

		m_backBufferSize = Window::GetState().frameBufferSize;

		{
			m_sceneState.configure(SceneConfig{}, Window::GetState());
			m_sceneBuffers = createSceneBuffers(m_sceneState.renderResolution(), m_sceneBuffers.sampleCount);
		}

		clear(GLES3ClearTarget::Scene | GLES3ClearTarget::BackBuffer);

		// full screen triangle
		{
			::glGenVertexArrays(1, &m_fullScreenTriangleVAO);

			::glGenSamplers(1, &m_fullScreenTriangleSampler);
			::glSamplerParameteri(m_fullScreenTriangleSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			::glSamplerParameteri(m_fullScreenTriangleSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			::glSamplerParameteri(m_fullScreenTriangleSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	clear
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::clear(const GLES3ClearTarget clearTarget)
	{
		if (clearTarget & GLES3ClearTarget::Scene)
		{
			m_sceneBuffers.resolved = false;
			m_sceneBuffers.getSceneTexture().clear(m_sceneStyle.getFrameBackgroundColor());
		}

		if (clearTarget & GLES3ClearTarget::BackBuffer)
		{
			const auto [r, g, b] = m_sceneStyle.letterboxColor.toFloat3();

			::glBindFramebuffer(GL_FRAMEBUFFER, 0);
			::glClearColor(r, g, b, 1.0f);
			::glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSceneStyle
	//
	////////////////////////////////////////////////////////////////

	SceneStyle& GLES3BackBuffer::getSceneStyle() noexcept
	{
		return m_sceneStyle;
	}

	////////////////////////////////////////////////////////////////
	//
	//	configureScene
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::configureScene(const SceneConfig& config)
	{
		auto state = m_sceneState;
		state.configure(config, Window::GetState());
		applySceneState(state, m_sceneBuffers.sampleCount);
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSceneConfig
	//
	////////////////////////////////////////////////////////////////

	SceneConfig GLES3BackBuffer::getSceneConfig() const noexcept
	{
		return m_sceneState.config;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setSceneSampleCount
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::setSceneSampleCount(const uint32 sampleCount)
	{
		applySceneState(m_sceneState, sampleCount);
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSceneSampleCount
	//
	////////////////////////////////////////////////////////////////

	uint32 GLES3BackBuffer::getSceneSampleCount() const noexcept
	{
		return m_sceneBuffers.sampleCount;
	}

	////////////////////////////////////////////////////////////////
	//
	//	updateSceneBufferSize
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::updateSceneBufferSize()
	{
		LOG_TRACE("GLES3BackBuffer::updateSceneBufferSize()");

		auto state = m_sceneState;
		state.updateWindow(Window::GetState());
		applySceneState(state, m_sceneBuffers.sampleCount);
	}

	////////////////////////////////////////////////////////////////
	//
	//	renderSceneToBackBuffer
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::renderSceneToBackBuffer()
	{
		resolveScene();

		::glActiveTexture(GL_TEXTURE0);
		::glBindTexture(GL_TEXTURE_2D, m_sceneBuffers.nonMSAA->getTexture());
		drawFullScreenTriangle();
		::glBindTexture(GL_TEXTURE_2D, 0);
	}

	////////////////////////////////////////////////////////////////
	//
	//	resizeBackBuffer
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::resizeBackBuffer(const Size backBufferSize)
	{
		assert((0 < backBufferSize.x) && (0 < backBufferSize.y));

		LOG_DEBUG(fmt::format("GLES3BackBuffer::resizeBackBuffer({})", backBufferSize));

		m_backBufferSize = backBufferSize;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getBackBufferSize
	//
	////////////////////////////////////////////////////////////////

	const Size& GLES3BackBuffer::getBackBufferSize() const noexcept
	{
		return m_backBufferSize;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSceneSize
	//
	////////////////////////////////////////////////////////////////

	const Size& GLES3BackBuffer::getSceneSize() const noexcept
	{
		return m_sceneState.sceneSize;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSceneBufferSize
	//
	////////////////////////////////////////////////////////////////

	const Size& GLES3BackBuffer::getSceneBufferSize() const noexcept
	{
		return m_sceneBuffers.getSceneTexture().size();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getLetterboxComposition
	//
	////////////////////////////////////////////////////////////////

	std::pair<double, RectF> GLES3BackBuffer::getLetterboxComposition() const noexcept
	{
		return m_sceneState.composition();
	}

	////////////////////////////////////////////////////////////////
	//
	//	bindSceneTextureAsRenderTarget
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::bindSceneTextureAsRenderTarget()
	{
		m_sceneBuffers.resolved = false;
		::glBindFramebuffer(GL_FRAMEBUFFER, m_sceneBuffers.getSceneTexture().getFrameBuffer());
	}

	////////////////////////////////////////////////////////////////
	//
	//	capture
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::capture()
	{
		const auto sceneSize = getSceneBufferSize();

		if (m_screenCaptureImage.size() != sceneSize)
		{
			m_screenCaptureImage.resize(sceneSize);
		}

		resolveScene();

		::glBindFramebuffer(GL_FRAMEBUFFER, m_sceneBuffers.nonMSAA->getFrameBuffer());
		::glReadPixels(0, 0, sceneSize.x, sceneSize.y, GL_RGBA, GL_UNSIGNED_BYTE, m_screenCaptureImage.data());
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	////////////////////////////////////////////////////////////////
	//
	//	getScreenCapture
	//
	////////////////////////////////////////////////////////////////

	const Image& GLES3BackBuffer::getScreenCapture() const noexcept
	{
		return m_screenCaptureImage;
	}

	////////////////////////////////////////////////////////////////
	//
	//	(private function)
	//
	////////////////////////////////////////////////////////////////

	void GLES3BackBuffer::drawFullScreenTriangle()
	{
		// view port
		{
			::glBindFramebuffer(GL_FRAMEBUFFER, 0);
			auto [s, viewRect] = getLetterboxComposition();
			::glViewport(
				static_cast<int32>(viewRect.x),
				static_cast<int32>(viewRect.y),
				static_cast<int32>(viewRect.w),
				static_cast<int32>(viewRect.h));
		}

		// render states
		{
			::glBindSampler(0, m_fullScreenTriangleSampler);
			::glSamplerParameteri(m_fullScreenTriangleSampler, GL_TEXTURE_MIN_FILTER, ToGLenum(m_sceneState.config.scalingFilter));
			::glSamplerParameteri(m_fullScreenTriangleSampler, GL_TEXTURE_MAG_FILTER, ToGLenum(m_sceneState.config.scalingFilter));

			m_pRenderer->getBlendState().set(BlendState::Opaque);
		//	m_pRenderer->getDepthStencilState().set(DepthStencilState::Default2D);
			m_pRenderer->getRasterizerState().set(RasterizerState::Default2D, false);
		}

		// shaders
		{
			const auto& vs = SIV3D_ENGINE(EngineShader)->getVS(EngineVS::FullScreenTriangle);
			const auto& ps = SIV3D_ENGINE(EngineShader)->getPS(EnginePS::FullScreenTriangle);

			m_pShader->setVS(vs.id());
			m_pShader->setPS(ps.id());
		}

		// draw fullscreen-triangle
		{
			m_pShader->usePipeline();

			::glBindVertexArray(m_fullScreenTriangleVAO);
			::glBindBuffer(GL_ARRAY_BUFFER, 0);
			::glDrawArrays(GL_TRIANGLES, 0, 3);
			::glBindVertexArray(0);
		}

		CheckOpenGLError();
	}

	void GLES3BackBuffer::resolveScene()
	{
		if (m_sceneBuffers.resolved)
		{
			return;
		}

		if (m_sceneBuffers.sampleCount > 1)
		{
			const auto sceneSize = m_sceneBuffers.msaa->size();

			::glBindFramebuffer(GL_READ_FRAMEBUFFER, m_sceneBuffers.msaa->getFrameBuffer());
			::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_sceneBuffers.nonMSAA->getFrameBuffer());
			::glBlitFramebuffer(0, 0, sceneSize.x, sceneSize.y, 0, 0, sceneSize.x, sceneSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		m_sceneBuffers.resolved = true;
	}

	GLES3BackBuffer::SceneBuffer GLES3BackBuffer::createSceneBuffers(const Size size, const uint32 sampleCount) const
	{
		SceneBuffer buffers;
		buffers.sampleCount = sampleCount;
		buffers.nonMSAA = GLES3InternalTexture2D::CreateRenderTexture(size);
		if (sampleCount > 1)
		{
			buffers.msaa = GLES3InternalTexture2D::CreateMSRenderTexture(size, sampleCount);
		}
		return buffers;
	}

	void GLES3BackBuffer::applySceneState(const SceneMisc::RenderState& state, const uint32 sampleCount)
	{
		const bool stateChanged = (state != m_sceneState);
		const bool samplesChanged = (sampleCount != m_sceneBuffers.sampleCount);
		if (not stateChanged && not samplesChanged)
		{
			return;
		}
		SceneMisc::ValidateSceneSize(state.sceneSize);
		const Size renderResolution = state.renderResolution();
		SceneMisc::ValidateSceneSize(renderResolution);
		const bool dimensionsChanged = ((renderResolution != getSceneBufferSize()) || (state.sceneSize != m_sceneState.sceneSize));
		const bool replaceBuffer = ((renderResolution != getSceneBufferSize()) || samplesChanged);
		if (dimensionsChanged || samplesChanged)
		{
			if (replaceBuffer)
			{
				m_sceneBuffers = createSceneBuffers(renderResolution, sampleCount);;
			}
			clear(GLES3ClearTarget::Scene | GLES3ClearTarget::BackBuffer);
		}
		if (dimensionsChanged)
		{
			SIV3D_ENGINE(Renderer2D)->setSceneScale(Float2{ (float(renderResolution.x) / state.sceneSize.x), (float(renderResolution.y) / state.sceneSize.y) });
		}
		if (stateChanged)
		{
			m_sceneState = state;
			SIV3D_ENGINE(Cursor)->updateBaseWindowTransform();
		}
	}
}
