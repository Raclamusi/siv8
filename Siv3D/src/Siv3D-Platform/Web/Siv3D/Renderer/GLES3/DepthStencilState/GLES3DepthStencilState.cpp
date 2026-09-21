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

# include "GLES3DepthStencilState.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void GLES3DepthStencilState::init()
	{
		LOG_SCOPED_DEBUG("GLES3DepthStencilState::init()");
	}

	////////////////////////////////////////////////////////////////
	//
	//	set
	//
	////////////////////////////////////////////////////////////////

	void GLES3DepthStencilState::set(const DepthStencilState& state)
	{
		if (state == m_currentState)
		{
			return;
		}

		if (state.depthTestEnabled != m_currentState.depthTestEnabled)
		{
			if (state.depthTestEnabled)
			{
				::glEnable(GL_DEPTH_TEST);
			}
			else
			{
				::glDisable(GL_DEPTH_TEST);
			}
		}

		if (state.depthWriteEnabled != m_currentState.depthWriteEnabled)
		{
			if (state.depthWriteEnabled)
			{
				::glDepthMask(GL_TRUE);
			}
			else
			{
				::glDepthMask(GL_FALSE);
			}
		}

		if (state.depthCompareFunction != m_currentState.depthCompareFunction)
		{
			::glDepthFunc(ToGLenum(state.depthCompareFunction));
		}

		m_currentState = state;
	}
}
