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

# include "CShader_GLES3.hpp"
# include <Siv3D/ShaderStage.hpp>
# include <Siv3D/TextFileReader.hpp>
# include <Siv3D/BinaryFileReader.hpp>
# include <Siv3D/Error/InternalEngineError.hpp>
# include <Siv3D/ConstantBuffer/GLES3/ConstantBuffer_GLES3.hpp>
# include <Siv3D/Engine/Siv3DEngine.hpp>
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	CShader_GLES3::~CShader_GLES3()
	{
		LOG_SCOPED_DEBUG("CShader_GLES3::~CShader_GLES3()");

		// PS の管理を破棄
		m_pixelShaders.destroy();

		// VS の管理を破棄
		m_vertexShaders.destroy();

		// プログラムの破棄
		for (const auto& [_, program] : m_programs)
		{
			::glDeleteProgram(program);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::init()
	{
		LOG_SCOPED_DEBUG("CShader_GLES3::init()");

		// null VS を管理に登録
		{
			auto nullVertexShader = std::make_unique<GLES3VertexShader>(GLES3VertexShader::Null{});

			if (not nullVertexShader->isInitialized()) // もし作成に失敗していたら
			{
				throw InternalEngineError{ "Failed to create a null vertex shader" };
			}

			m_vertexShaders.setNullData(std::move(nullVertexShader));
		}

		// null PS を管理に登録
		{
			auto nullPixelShader = std::make_unique<GLES3PixelShader>(GLES3PixelShader::Null{});

			if (not nullPixelShader->isInitialized()) // もし作成に失敗していたら
			{
				throw InternalEngineError{ "Failed to create a null pixel shader" };
			}

			m_pixelShaders.setNullData(std::move(nullPixelShader));
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	createVSFromReader
	//
	////////////////////////////////////////////////////////////////

	VertexShader::IDType CShader_GLES3::createVSFromReader(std::unique_ptr<IReader> reader, const FilePathView pathHint, const StringView)
	{
		if (not reader)
		{
			return VertexShader::IDType::Null();
		}

		if (not reader->isOpen())
		{
			if (pathHint) // ファイルの場合
			{
				LOG_FAIL(fmt::format("CShader_GLES3::createVSFromFile(): failed to open `{}`", pathHint.toUTF8()));
			}
			else // その他の Reader オブジェクトの場合
			{
				LOG_FAIL("CShader_GLES3::createVSFromFile(): failed to read the shader source");
			}

			return VertexShader::IDType::Null();
		}

		TextFileReader textFileReader{ std::move(reader) };
		std::string source;

		if (not textFileReader.readAll(source))
		{
			LOG_FAIL("CShader_GLES3::createVSFromFile(): failed to read the shader source");
			return VertexShader::IDType::Null();
		}

		return createVS(source);
	}

	////////////////////////////////////////////////////////////////
	//
	//	createVSFromSource
	//
	////////////////////////////////////////////////////////////////

	VertexShader::IDType CShader_GLES3::createVSFromSource(const std::string& source, const StringView)
	{
		return createVS(source);
	}

	////////////////////////////////////////////////////////////////
	//
	//	createVSFromBytecode
	//
	////////////////////////////////////////////////////////////////

	VertexShader::IDType CShader_GLES3::createVSFromBytecode(const Blob& bytecode)
	{
		return VertexShader::IDType::Null();
	}

	////////////////////////////////////////////////////////////////
	//
	//	createPSFromReader
	//
	////////////////////////////////////////////////////////////////

	PixelShader::IDType CShader_GLES3::createPSFromReader(std::unique_ptr<IReader> reader, const FilePathView pathHint, const StringView entryPoint)
	{
		if (not reader)
		{
			return PixelShader::IDType::Null();
		}

		if (not reader->isOpen())
		{
			if (pathHint) // ファイルの場合
			{
				LOG_FAIL(fmt::format("CShader_GLES3::createPSFromFile(): failed to open `{}`", pathHint.toUTF8()));
			}
			else // その他の Reader オブジェクトの場合
			{
				LOG_FAIL("CShader_GLES3::createPSFromFile(): failed to read the shader source");
			}

			return PixelShader::IDType::Null();
		}

		TextFileReader textFileReader{ std::move(reader) };
		std::string source;

		if (not textFileReader.readAll(source))
		{
			LOG_FAIL("CShader_GLES3::createPSFromFile(): failed to read the shader source");
			return PixelShader::IDType::Null();
		}

		return createPS(source);
	}

	////////////////////////////////////////////////////////////////
	//
	//	createPSFromSource
	//
	////////////////////////////////////////////////////////////////

	PixelShader::IDType CShader_GLES3::createPSFromSource(const std::string& source, const StringView entryPoint)
	{
		return createPS(source);
	}

	////////////////////////////////////////////////////////////////
	//
	//	createPSFromBytecode
	//
	////////////////////////////////////////////////////////////////

	PixelShader::IDType CShader_GLES3::createPSFromBytecode(const Blob& bytecode)
	{
		return PixelShader::IDType::Null();
	}

	////////////////////////////////////////////////////////////////
	//
	//	releaseVS
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::releaseVS(const VertexShader::IDType handleID)
	{
		m_vertexShaders.erase(handleID);
	}

	////////////////////////////////////////////////////////////////
	//
	//	releasePS
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::releasePS(const PixelShader::IDType handleID)
	{
		m_pixelShaders.erase(handleID);
	}

	////////////////////////////////////////////////////////////////
	//
	//	setVS
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::setVS(const VertexShader::IDType handleID)
	{
		m_currentVS = handleID;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setVSNull
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::setVSNull()
	{
		m_currentVS = VertexShader::IDType::Null();
	}

	////////////////////////////////////////////////////////////////
	//
	//	setPS
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::setPS(const PixelShader::IDType handleID)
	{
		m_currentPS = handleID;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setPSNull
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::setPSNull()
	{
		m_currentPS = PixelShader::IDType::Null();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getBytecodeVS
	//
	////////////////////////////////////////////////////////////////

	const Blob& CShader_GLES3::getBytecodeVS(const VertexShader::IDType)
	{
		static const Blob blob;
		return blob;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getBytecodePS
	//
	////////////////////////////////////////////////////////////////

	const Blob& CShader_GLES3::getBytecodePS(const PixelShader::IDType)
	{
		static const Blob blob;
		return blob;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setConstantBufferVS
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::setConstantBufferVS(const uint32 slot, IConstantBuffer* cb)
	{
		assert(slot < ConstantBufferSlotCount);

		const uint32 vsUniformBlockBinding = MakeUniformBlockBinding(ShaderStage::Vertex, slot);
		::glBindBufferBase(GL_UNIFORM_BUFFER, vsUniformBlockBinding, static_cast<const ConstantBuffer_GLES3*>(cb)->getBuffer());
	}

	////////////////////////////////////////////////////////////////
	//
	//	setConstantBufferPS
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::setConstantBufferPS(const uint32 slot, IConstantBuffer* cb)
	{
		assert(slot < ConstantBufferSlotCount);

		const uint32 psUniformBlockBinding = MakeUniformBlockBinding(ShaderStage::Pixel, slot);
		::glBindBufferBase(GL_UNIFORM_BUFFER, psUniformBlockBinding, static_cast<const ConstantBuffer_GLES3*>(cb)->getBuffer());
	}

	////////////////////////////////////////////////////////////////
	//
	//	usePipeline
	//
	////////////////////////////////////////////////////////////////

	void CShader_GLES3::usePipeline()
	{
		const auto [it, inserted] = m_programs.try_emplace({ m_currentVS, m_currentPS });

		if (inserted)
		{
			it->second = createProgram();
		}

		::glUseProgram(it->second);
	}

	////////////////////////////////////////////////////////////////
	//
	//	(private function)
	//
	////////////////////////////////////////////////////////////////

	VertexShader::IDType CShader_GLES3::createVS(const std::string& source)
	{
		auto vertexShader = std::make_unique<GLES3VertexShader>(source);

		if (not vertexShader->isInitialized())
		{
			return VertexShader::IDType::Null();
		}

		return m_vertexShaders.add(std::move(vertexShader));
	}

	PixelShader::IDType CShader_GLES3::createPS(const std::string& source)
	{
		auto pixelShader = std::make_unique<GLES3PixelShader>(source);

		if (not pixelShader->isInitialized())
		{
			return PixelShader::IDType::Null();
		}

		return m_pixelShaders.add(std::move(pixelShader));
	}

	GLuint CShader_GLES3::createProgram()
	{
		const auto pVertexShader = m_vertexShaders[m_currentVS];
		const auto pPixelShader = m_pixelShaders[m_currentPS];

		const auto vs = pVertexShader->getShader();
		const auto ps = pPixelShader->getShader();

		if (vs == 0 || ps == 0)
		{
			return 0;
		}

		GLuint program = ::glCreateProgram();

		// 頂点シェーダとピクセルシェーダをリンク
		{
			::glAttachShader(program, vs);
			::glAttachShader(program, ps);

			::glLinkProgram(program);

			GLint status = GL_FALSE;
			::glGetProgramiv(program, GL_LINK_STATUS, &status);

			::glDetachShader(program, vs);
			::glDetachShader(program, ps);

			if (status == GL_FALSE)
			{
				LOG_FAIL("CShader_GLES3::useProgram(): failed to link program");
				::glDeleteProgram(program);
				return 0;
			}
		}

		// 定数バッファのバインディングを設定
		{
			constexpr std::pair<const char*, GLuint> Binding[]
			{
				{ "VSConstants2D", MakeUniformBlockBinding(ShaderStage::Vertex, 0) },
				{ "PSConstants2D", MakeUniformBlockBinding(ShaderStage::Pixel, 0) },
				{ "PSEffectConstants2D", MakeUniformBlockBinding(ShaderStage::Pixel, 1) },
			};

			for (const auto& [name, index] : Binding)
			{
				const GLuint blockIndex = ::glGetUniformBlockIndex(program, name);

				if (blockIndex != GL_INVALID_INDEX)
				{
					::glUniformBlockBinding(program, blockIndex, index);
				}
			}
		}

		// テクスチャのスロットを設定
		{
			Array<std::pair<uint32, GLint>> textureLocations;

			for (uint32 slot = 0; slot < (Graphics::TextureSlotCount * 2); ++slot)
			{
				const std::string name = fmt::format("Texture{}", slot);
				const GLint location = ::glGetUniformLocation(program, name.c_str());

				if (location != -1)
				{
					textureLocations.emplace_back(slot, location);
				}
			}

			if (not textureLocations.isEmpty())
			{
				::glUseProgram(program);
				for (const auto& [slot, location] : textureLocations)
				{
					::glUniform1i(location, slot);
				}
				::glUseProgram(0);
			}
		}

		return program;
	}
}
