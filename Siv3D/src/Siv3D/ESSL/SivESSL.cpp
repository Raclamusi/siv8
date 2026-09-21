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

# include <Siv3D/ESSL.hpp>
# include <Siv3D/VertexShader.hpp>
# include <Siv3D/PixelShader.hpp>
# include <Siv3D/ShaderGroup.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	ESSL::ESSL(FilePath path)
		: m_path{ std::move(path) } {}

	////////////////////////////////////////////////////////////////
	//
	//	operator |
	//
	////////////////////////////////////////////////////////////////

	ShaderGroup ESSL::operator |(const HLSL& hlsl) const
	{
		return{ hlsl, none, *this };
	}

	ShaderGroup ESSL::operator |(const MSL& msl) const
	{
		return{ none, msl, *this };
	}

	////////////////////////////////////////////////////////////////
	//
	//	operator VertexShader
	//
	////////////////////////////////////////////////////////////////

	ESSL::operator VertexShader() const
	{
		if (not m_source.empty())
		{
			return VertexShader::ESSL(m_source);
		}

		return VertexShader::ESSL(m_path);
	}

	////////////////////////////////////////////////////////////////
	//
	//	operator PixelShader
	//
	////////////////////////////////////////////////////////////////

	ESSL::operator PixelShader() const
	{
		if (not m_source.empty())
		{
			return PixelShader::ESSL(m_source);
		}

		return PixelShader::ESSL(m_path);
	}

	////////////////////////////////////////////////////////////////
	//
	//	FromSource
	//
	////////////////////////////////////////////////////////////////

	ESSL ESSL::FromSource(std::string source)
	{
		ESSL essl{};
		essl.m_source = std::move(source);
		return essl;
	}

	ESSL ESSL::FromSource(const StringView source)
	{
		return FromSource(source.toUTF8());
	}
}
