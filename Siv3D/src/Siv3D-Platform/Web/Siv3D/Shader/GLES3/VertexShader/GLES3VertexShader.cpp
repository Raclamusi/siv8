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

# include "GLES3VertexShader.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3VertexShader::GLES3VertexShader(Null)
		: m_initialized{ true } {}

	GLES3VertexShader::GLES3VertexShader(const std::string& source)
	{
		m_shader = ::glCreateShader(GL_VERTEX_SHADER);

		if (m_shader == 0)
		{
			LOG_FAIL("❌ Vertex shader compilation failed: failed to create shader.");
			return;
		}

		// シェーダのコンパイル
		{
			std::string sourceData = source;

			for (uint32 slot = 0; slot < Graphics::TextureSlotCount; ++slot)
			{
				const GLuint samplerSlot = MakeSamplerSlot(ShaderStage::Vertex, slot);
				const std::string oldName = fmt::format("Texture{}", slot);
				const std::string newName = fmt::format("Texture{}", samplerSlot);

				size_t pos = 0;
				while ((pos = sourceData.find(oldName, pos)) != std::string::npos)
				{
					const char before = ((pos >= 1) ? sourceData[pos - 1] : '\0');
					const char after = ((pos + oldName.length() < sourceData.length()) ? sourceData[pos + oldName.length()] : '\0');
					if (std::isalnum(before) || (before == '_') || std::isalnum(after) || (after == '_'))
					{
						pos += oldName.length();
						continue;
					}

					sourceData.replace(pos, oldName.length(), newName);
					pos += newName.length();
				}
			}

			const char* pSource = sourceData.c_str();

			::glShaderSource(m_shader, 1, &pSource, NULL);
			::glCompileShader(m_shader);

			GLint status = GL_FALSE;
			::glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);

			GLint logLen = 0;
			::glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logLen);

			// ログメッセージ
			if (logLen > 4)
			{
				std::string log(logLen + 1, '\0');
				::glGetShaderInfoLog(m_shader, logLen, &logLen, log.data());
				LOG_FAIL(fmt::format("❌ Vertex shader compilation failed: {}", log));
			}

			if (status == GL_FALSE)
			{
				::glDeleteShader(m_shader);
				m_shader = 0;
			}
		}

		m_initialized = true;
	}

	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3VertexShader::~GLES3VertexShader()
	{
		if (m_shader)
		{
			::glDeleteShader(m_shader);
			m_shader = 0;
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	isInitialized
	//
	////////////////////////////////////////////////////////////////

	bool GLES3VertexShader::isInitialized() const noexcept
	{
		return m_initialized;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getShader
	//
	////////////////////////////////////////////////////////////////

	GLuint GLES3VertexShader::getShader() const noexcept
	{
		return m_shader;
	}
}
