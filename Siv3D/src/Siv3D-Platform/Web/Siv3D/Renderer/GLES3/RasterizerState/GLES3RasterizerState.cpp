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

# include "GLES3RasterizerState.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void GLES3RasterizerState::init()
	{
		LOG_SCOPED_DEBUG("GLES3RasterizerState::init()");
	}

	////////////////////////////////////////////////////////////////
	//
	//	set
	//
	////////////////////////////////////////////////////////////////

	void GLES3RasterizerState::set(const RasterizerState& state, const bool scissorEnabled)
	{
		const auto newState = std::make_pair(state, scissorEnabled);

		if (newState == m_currentState)
		{
			return;
		}

		const auto& [currentState, currentScissorEnabled] = m_currentState;

		if (state.triangleFillMode != currentState.triangleFillMode)
		{
			LOG_ERROR("WireFrame rendendering is not supported because OpenGL ES is lacking of glPolygonMode.");
			// ::glPolygonMode(GL_FRONT_AND_BACK, state.triangleFillMode == TriangleFillMode::Solid ? GL_FILL : GL_LINE);
		}

		if (state.cullMode != currentState.cullMode)
		{
			if (state.cullMode == CullMode::None)
			{
				::glDisable(GL_CULL_FACE);
			}
			else
			{
				::glEnable(GL_CULL_FACE);

				::glFrontFace(GL_CW);

				// CullMode::Front = GL_BACK, CullMode::Back = GL_FRONT
				::glCullFace(state.cullMode == CullMode::Front ? GL_BACK : GL_FRONT);
			}
		}

		if (state.depthBias != currentState.depthBias)
		{
			if (state.depthBias)
			{
				// ::glEnable(GL_POLYGON_OFFSET_LINE);
				::glEnable(GL_POLYGON_OFFSET_FILL);
				::glPolygonOffset(0, state.depthBias);
			}
			else
			{
				// ::glDisable(GL_POLYGON_OFFSET_LINE);
				::glDisable(GL_POLYGON_OFFSET_FILL);
			}
		}

		if (scissorEnabled != currentScissorEnabled)
		{
			if (scissorEnabled)
			{
				::glEnable(GL_SCISSOR_TEST);
			}
			else
			{
				::glDisable(GL_SCISSOR_TEST);
			}
		}

		m_currentState = newState;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setScissorRect
	//
	////////////////////////////////////////////////////////////////

	void GLES3RasterizerState::setScissorRect(const Rect& scissorRect)
	{
		::glScissor(scissorRect.x, scissorRect.y, scissorRect.w, scissorRect.h);
	}
}
