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

# include "GLES3BlendState.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void GLES3BlendState::init()
	{
		LOG_SCOPED_DEBUG("GLES3BlendState::init()");
	}

	////////////////////////////////////////////////////////////////
	//
	//	set
	//
	////////////////////////////////////////////////////////////////

	void GLES3BlendState::set(const BlendState& state, const Float4& blendColor)
	{
		const auto newState = std::make_pair(state, blendColor);

		if (newState == m_currentState)
		{
			return;
		}

		const auto& [currentState, currentBlendColor] = m_currentState;

		if (state.enabled != currentState.enabled)
		{
			if (state.enabled)
			{
				::glEnable(GL_BLEND);
			}
			else
			{
				::glDisable(GL_BLEND);
			}
		}

		if (state.alphaToCoverageEnabled != currentState.alphaToCoverageEnabled)
		{
			if (state.alphaToCoverageEnabled)
			{
				::glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
			else
			{
				::glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
		}

		if (state.writeR != currentState.writeR
			|| state.writeG != currentState.writeG
			|| state.writeB != currentState.writeB
			|| state.writeA != currentState.writeA)
		{
			::glColorMask(state.writeR, state.writeG, state.writeB, state.writeA);
		}

		if (state.rgbOperation != currentState.rgbOperation || state.alphaOperation != currentState.alphaOperation)
		{
			::glBlendEquationSeparate(
				ToGLenum(state.rgbOperation),
				ToGLenum(state.alphaOperation));
		}

		if (state.sourceRGB != currentState.sourceRGB || state.destinationRGB != currentState.destinationRGB
			|| state.sourceAlpha != currentState.sourceAlpha || state.destinationAlpha != currentState.destinationAlpha)
		{
			::glBlendFuncSeparate(
				ToGLenum(state.sourceRGB),
				ToGLenum(state.destinationRGB),
				ToGLenum(state.sourceAlpha),
				ToGLenum(state.destinationAlpha));
		}

		if (blendColor != currentBlendColor)
		{
			::glBlendColor(blendColor.x, blendColor.y, blendColor.z, blendColor.w);
		}

		m_currentState = newState;
	}
}
