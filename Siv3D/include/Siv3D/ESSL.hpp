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
# include "Common.hpp"
# include "String.hpp"
# include "Result.hpp"
# include "ShaderStage.hpp"

namespace s3d
{
	class VertexShader;
	class PixelShader;
	class ShaderGroup;
	class HLSL;
	class MSL;

	////////////////////////////////////////////////////////////////
	//
	//	ESSL
	//
	////////////////////////////////////////////////////////////////

	/// @brief ESSL (OpenGL ES Shading Language) ファイル
	class ESSL
	{
	public:

		////////////////////////////////////////////////////////////////
		//
		//	(constructor)
		//
		////////////////////////////////////////////////////////////////

		/// @brief デフォルトコンストラクタ
		[[nodiscard]]
		ESSL() = default;

		/// @brief ファイルパスで ESSL を指定します。
		/// @param path ESSL ファイルのパス
		[[nodiscard]]
		ESSL(FilePath path);

		////////////////////////////////////////////////////////////////
		//
		//	operator |
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		ShaderGroup operator |(const HLSL& hlsl) const;

		[[nodiscard]]
		ShaderGroup operator |(const MSL& msl) const;

		////////////////////////////////////////////////////////////////
		//
		//	operator VertexShader
		//
		////////////////////////////////////////////////////////////////

		/// @brief ESSL から頂点シェーダを作成します。
		[[nodiscard]]
		operator VertexShader() const;

		////////////////////////////////////////////////////////////////
		//
		//	operator PixelShader
		//
		////////////////////////////////////////////////////////////////

		/// @brief ESSL からピクセルシェーダを作成します。
		[[nodiscard]]
		operator PixelShader() const;

		////////////////////////////////////////////////////////////////
		//
		//	FromSource
		//
		////////////////////////////////////////////////////////////////

		static ESSL FromSource(std::string source);

		static ESSL FromSource(StringView source);

	private:

		/// @brief ESSL ファイルのパス
		FilePath m_path;

		/// @brief ESSL ソースコード
		std::string m_source;
	};
}
