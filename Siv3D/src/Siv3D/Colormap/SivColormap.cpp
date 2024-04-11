﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2024 Ryo Suzuki
//	Copyright (c) 2016-2024 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/Colormap.hpp>
# include <ThirdParty/tinycolormap/tinycolormap.hpp>

namespace s3d
{
	ColorF Colormap(const double t, const ColorScale scale)
	{
		const auto c = tinycolormap::GetColor(t, ToEnum<tinycolormap::ColormapType>(FromEnum(scale)));
		return{ c.r(), c.g(), c.b() };
	}

	ColorF Colormap(const double t, const double min, const double max, const ColorScale scale)
	{
		return Colormap(((t - min) / (max - min)), scale);
	}
}
