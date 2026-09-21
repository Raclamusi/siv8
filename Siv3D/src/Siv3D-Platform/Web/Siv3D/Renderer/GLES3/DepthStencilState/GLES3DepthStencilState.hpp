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
# include <Siv3D/DepthStencilState.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>

namespace s3d
{
	class GLES3DepthStencilState
	{
	public:

		void init();

		void set(const DepthStencilState& state);

	private:

		DepthStencilState m_currentState = DepthStencilState::Invalid();
	};
}
