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

# include "GLES3SamplerState.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void GLES3SamplerState::init()
	{
		LOG_SCOPED_DEBUG("GLES3SamplerState::init()");
		m_currentVSStates.fill(SamplerState::Invalid());
		m_currentPSStates.fill(SamplerState::Invalid());
	}

	////////////////////////////////////////////////////////////////
	//
	//	setVS
	//
	////////////////////////////////////////////////////////////////

	void GLES3SamplerState::setVS(const uint32 slot, const SamplerState& state)
	{
		assert(slot < Graphics::TextureSlotCount);

		if (state == m_currentVSStates[slot])
		{
			return;
		}

		auto it = m_states.find(state);

		if (it == m_states.end())
		{
			it = create(state);

			if (it == m_states.end())
			{
				return;
			}
		}

		::glBindSampler(MakeSamplerSlot(ShaderStage::Vertex, slot), it->second->sampler);

		m_currentVSStates[slot] = state;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setPS
	//
	////////////////////////////////////////////////////////////////

	void GLES3SamplerState::setPS(const uint32 slot, const SamplerState& state)
	{
		assert(slot < Graphics::TextureSlotCount);

		if (state == m_currentPSStates[slot])
		{
			return;
		}

		auto it = m_states.find(state);

		if (it == m_states.end())
		{
			it = create(state);

			if (it == m_states.end())
			{
				return;
			}
		}

		::glBindSampler(MakeSamplerSlot(ShaderStage::Pixel, slot), it->second->sampler);

		m_currentPSStates[slot] = state;
	}

	////////////////////////////////////////////////////////////////
	//
	//	(private function)
	//
	////////////////////////////////////////////////////////////////

	GLES3SamplerState::StateTable::iterator GLES3SamplerState::create(const SamplerState& state)
	{
		auto samplerState = std::make_unique<SamplerState_GL>();

		const GLuint sampler = samplerState->sampler;

		using enum TextureFilter;
		const auto min = state.minFilter;
		const auto mip = state.mipFilter;
		::glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER,
			(min == Nearest && mip == Nearest) ? GL_NEAREST_MIPMAP_NEAREST :
			(min == Nearest && mip == Linear ) ? GL_NEAREST_MIPMAP_LINEAR :
			(min == Linear  && mip == Nearest) ? GL_LINEAR_MIPMAP_NEAREST :
			(min == Linear  && mip == Linear ) ? GL_LINEAR_MIPMAP_LINEAR : 0);

		::glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, ToGLenum(state.magFilter));
		::glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, ToGLenum(state.uAddressMode));
		::glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, ToGLenum(state.vAddressMode));
		::glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, ToGLenum(state.wAddressMode));
		::glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, state.maxAnisotropy);
		::glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		::glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, ToGLenum(state.compareFunction));

		static constexpr GLfloat SampleBorderColorTable[][4]
		{
			{ 0.0f, 0.0f, 0.0f, 0.0f },	// TransparentBlack
			{ 0.0f, 0.0f, 0.0f, 1.0f },	// OpaqueBlack
			{ 1.0f, 1.0f, 1.0f, 1.0f },	// OpaqueWhite
		};
		::glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, SampleBorderColorTable[FromEnum(state.borderColor)]);

		::glSamplerParameterf(sampler, GL_TEXTURE_MIN_LOD, state.minLOD);
		::glSamplerParameterf(sampler, GL_TEXTURE_MAX_LOD, 1000.0f);
		::glSamplerParameterf(sampler, GL_TEXTURE_LOD_BIAS, 0.0f);

		if (m_states.size() >= 1024)
		{
			m_states.clear();
		}

		return m_states.emplace(state, std::move(samplerState)).first;
	}
}
