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

# pragma once
# include <Siv3D/Common.hpp>
# include <Siv3D/SamplerState.hpp>
# include <Siv3D/Graphics.hpp>
# include <Siv3D/HashMap.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>

namespace s3d
{
	class GLES3SamplerState
	{
	public:

		void init();

		void setVS(uint32 slot, const SamplerState& state);

		void setPS(uint32 slot, const SamplerState& state);

	private:

		struct SamplerState_GL
		{
			SamplerState_GL()
			{
				::glGenSamplers(1, &sampler);
			}

			~SamplerState_GL()
			{
				if (sampler)
				{
					::glDeleteSamplers(1, &sampler);
				}
			}

			GLuint sampler = 0;
		};

		using StateTable = HashMap<SamplerState, std::unique_ptr<SamplerState_GL>>;

		StateTable m_states;

		std::array<SamplerState, Graphics::TextureSlotCount> m_currentVSStates;

		std::array<SamplerState, Graphics::TextureSlotCount> m_currentPSStates;

		StateTable::iterator create(const SamplerState& state);
	};
}
