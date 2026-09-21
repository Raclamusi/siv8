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

# include "GLES3/CTexture_GLES3.hpp"

namespace s3d
{
	ISiv3DTexture* ISiv3DTexture::Create()
	{
		return new CTexture_GLES3;
	}
}
