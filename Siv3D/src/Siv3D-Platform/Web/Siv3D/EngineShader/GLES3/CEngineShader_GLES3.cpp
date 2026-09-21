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

# include "CEngineShader_GLES3.hpp"
# include <Siv3D/ESSL.hpp>
# include <Siv3D/Resource.hpp>
# include <Siv3D/Error/InternalEngineError.hpp>
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	CEngineShader_GLES3::~CEngineShader_GLES3()
	{
		LOG_SCOPED_DEBUG("CEngineShader_GLES3::~CEngineShader_GLES3()");
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void CEngineShader_GLES3::init()
	{
		LOG_SCOPED_DEBUG("CEngineShader_GLES3::init()");

		{
			m_vertexShaders << ESSL{ Resource(U"engine/shader/gles3/fullscreen_triangle.vert") };
			m_vertexShaders << ESSL{ Resource(U"engine/shader/gles3/2d.vert") };
			m_vertexShaders << ESSL{ Resource(U"engine/shader/gles3/2d_quadwarp.vert") };

			if (not m_vertexShaders.all([](const auto& vs) { return static_cast<bool>(vs); })) // もしロードに失敗したシェーダがあれば
			{
				throw InternalEngineError{ "Failed to load a engine shader" };
			}
		}

		{
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/fullscreen_triangle.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_shape.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_texture.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_quadwarp.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_line_dot.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_line_dash.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_line_long_dash.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_line_dash_dot.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_line_round_dot.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_pattern_polka_dot.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_pattern_stripe.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_pattern_grid.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_pattern_checker.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_pattern_triangle.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/2d_pattern_hex_grid.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/msdffont.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/msdffont_outline.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/msdffont_shadow.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/msdffont_outline_shadow.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/msdffont_glow.frag") };
			m_pixelShaders << ESSL{ Resource(U"engine/shader/gles3/msdffont_print.frag") };

			if (not m_pixelShaders.all([](const auto& ps) { return static_cast<bool>(ps); })) // もしロードに失敗したシェーダがあれば
			{
				throw InternalEngineError{ "Failed to load a engine shader" };
			}
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	getVS
	//
	////////////////////////////////////////////////////////////////

	const VertexShader& CEngineShader_GLES3::getVS(const EngineVS vs) const
	{
		return m_vertexShaders[FromEnum(vs)];
	}

	////////////////////////////////////////////////////////////////
	//
	//	getPS
	//
	////////////////////////////////////////////////////////////////

	const PixelShader& CEngineShader_GLES3::getPS(const EnginePS ps) const
	{
		return m_pixelShaders[FromEnum(ps)];
	}
}
