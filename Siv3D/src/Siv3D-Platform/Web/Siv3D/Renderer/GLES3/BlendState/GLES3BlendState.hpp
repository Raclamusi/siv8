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
# include <Siv3D/BlendState.hpp>
# include <Siv3D/PointVector.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>

namespace s3d
{
	class GLES3BlendState
	{
	public:

		void init();

		void set(const BlendState& state, const Float4& blendColor = Float4{ 0, 0, 0, 0 });

	private:

		std::pair<BlendState, Float4> m_currentState{ BlendState::Invalid(), Float4{ 0, 0, 0, 0} };
	};
}
