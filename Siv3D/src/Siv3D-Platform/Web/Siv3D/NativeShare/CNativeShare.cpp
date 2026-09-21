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

# include "CNativeShare.hpp"
# include <Siv3D/Image.hpp>
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	CNativeShare::~CNativeShare()
	{
		LOG_SCOPED_DEBUG("CNativeShare::~CNativeShare()");
	}

	void CNativeShare::init()
	{
		LOG_SCOPED_DEBUG("CNativeShare::init()");
	}

	bool CNativeShare::show(const Image&)
	{
		// TODO
		return false;
	}
}
