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
# include <Siv3D/RasterizerState.hpp>
# include <Siv3D/2DShapes.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>

namespace s3d
{
	class GLES3RasterizerState
	{
	public:

		void init();

		void set(const RasterizerState& state, bool scissorEnabled);

		void setScissorRect(const Rect& scissorRect);

	private:

		std::pair<RasterizerState, bool> m_currentState = { RasterizerState::Invalid(), false };
	};
}
