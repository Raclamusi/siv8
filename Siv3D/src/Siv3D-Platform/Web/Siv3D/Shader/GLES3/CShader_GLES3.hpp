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
# include <Siv3D/Blob.hpp>
# include <Siv3D/Shader/IShader.hpp>
# include <Siv3D/AssetHandleManager/AssetHandleManager.hpp>
# include "VertexShader/GLES3VertexShader.hpp"
# include "PixelShader/GLES3PixelShader.hpp"

namespace s3d
{
	class CShader_GLES3 final : public ISiv3DShader
	{
	public:

		~CShader_GLES3() override;

		void init() override;

		VertexShader::IDType createVSFromReader(std::unique_ptr<IReader> reader, FilePathView pathHint, StringView entryPoint) override;

		VertexShader::IDType createVSFromSource(const std::string& source, StringView entryPoint) override;

		VertexShader::IDType createVSFromBytecode(const Blob& bytecode) override;

		PixelShader::IDType createPSFromReader(std::unique_ptr<IReader> reader, FilePathView pathHint, StringView entryPoint) override;

		PixelShader::IDType createPSFromSource(const std::string& source, StringView entryPoint) override;

		PixelShader::IDType createPSFromBytecode(const Blob& bytecode) override;

		void releaseVS(VertexShader::IDType handleID) override;

		void releasePS(PixelShader::IDType handleID) override;

		void setVS(VertexShader::IDType handleID) override;

		void setVSNull() override;

		void setPS(PixelShader::IDType handleID) override;

		void setPSNull() override;

		const Blob& getBytecodeVS(VertexShader::IDType handleID) override;

		const Blob& getBytecodePS(PixelShader::IDType handleID) override;

		void setConstantBufferVS(uint32 slot, IConstantBuffer* cb) override;

		void setConstantBufferPS(uint32 slot, IConstantBuffer* cb) override;

		void usePipeline();

	private:

		// VS の管理
		AssetHandleManager<VertexShader::IDType, GLES3VertexShader> m_vertexShaders{ "VertexShader" };

		// PS の管理
		AssetHandleManager<PixelShader::IDType, GLES3PixelShader> m_pixelShaders{ "PixelShader" };

		VertexShader::IDType m_currentVS;

		PixelShader::IDType m_currentPS;

		HashMap<std::pair<VertexShader::IDType, PixelShader::IDType>, GLuint> m_programs;

		VertexShader::IDType createVS(const std::string& source);

		PixelShader::IDType createPS(const std::string& source);

		GLuint createProgram();
	};
}
