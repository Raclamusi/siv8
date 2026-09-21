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

# include "GLES3PixelShader.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3PixelShader::GLES3PixelShader(Null)
		: m_initialized{ true } {}

	GLES3PixelShader::GLES3PixelShader(const std::string& source)
	{
		m_shader = ::glCreateShader(GL_FRAGMENT_SHADER);

		if (m_shader == 0)
		{
			LOG_FAIL("❌ Pixel shader compilation failed: failed to create shader.");
			return;
		}

		// シェーダのコンパイル
		{
			const char* pSource = source.c_str();

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
				LOG_FAIL(fmt::format("❌ Pixel shader compilation failed: {}", log));
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

	GLES3PixelShader::~GLES3PixelShader()
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

	bool GLES3PixelShader::isInitialized() const noexcept
	{
		return m_initialized;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getShader
	//
	////////////////////////////////////////////////////////////////

	GLuint GLES3PixelShader::getShader() const noexcept
	{
		return m_shader;
	}
}
