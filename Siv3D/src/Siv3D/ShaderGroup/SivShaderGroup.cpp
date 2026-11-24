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

# include <Siv3D/ShaderGroup.hpp>
# include <Siv3D/VertexShader.hpp>
# include <Siv3D/PixelShader.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	ShaderGroup::ShaderGroup(const Optional<HLSL>& hlsl, const Optional<MSL>& msl, const Optional<ESSL>& essl)
		: m_hlsl{ hlsl }
		, m_msl{ msl }
		, m_essl{ essl } {}

	////////////////////////////////////////////////////////////////
	//
	//	operator VertexShader
	//
	////////////////////////////////////////////////////////////////

	ShaderGroup::operator VertexShader() const
	{
	# if SIV3D_PLATFORM(WINDOWS)
		assert(m_hlsl);
		return *m_hlsl;
	# elif SIV3D_PLATFORM(MACOS)
		assert(m_msl);
		return *m_msl;
	# elif SIV3D_PLATFORM(WEB)
		assert(m_essl);
		return *m_essl;
	# else
		return{};
	# endif
	}

	////////////////////////////////////////////////////////////////
	//
	//	operator PixelShader
	//
	////////////////////////////////////////////////////////////////

	ShaderGroup::operator PixelShader() const
	{
	# if SIV3D_PLATFORM(WINDOWS)
		assert(m_hlsl);
		return *m_hlsl;
	# elif SIV3D_PLATFORM(MACOS)
		assert(m_msl);
		return *m_msl;
	# elif SIV3D_PLATFORM(WEB)
		assert(m_essl);
		return *m_essl;
	# else
		return{};
	# endif
	}

	////////////////////////////////////////////////////////////////
	//
	//	operator |
	//
	////////////////////////////////////////////////////////////////

	ShaderGroup ShaderGroup::operator |(const HLSL& hlsl) const
	{
		return{ hlsl, m_msl, m_essl };
	}

	ShaderGroup ShaderGroup::operator |(const MSL& msl) const
	{
		return{ m_hlsl, msl, m_essl };
	}

	ShaderGroup ShaderGroup::operator |(const ESSL& essl) const
	{
		return{ m_hlsl, m_msl, essl };
	}
}
