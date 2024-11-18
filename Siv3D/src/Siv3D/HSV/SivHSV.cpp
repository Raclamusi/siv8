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

# include <Siv3D/ColorHSV.hpp>
# include <Siv3D/FloatFormatter.hpp>

namespace s3d
{
	namespace
	{
		[[nodiscard]]
		inline static double Fraction(const double x) noexcept
		{
			return (x - std::floor(x));
		}

		inline constexpr uint8 HSVConversionTable[6][3] =
		{
			{ 3, 2, 0 },
			{ 1, 3, 0 },
			{ 0, 3, 2 },
			{ 0, 1, 3 },
			{ 2, 0, 3 },
			{ 3, 0, 1 },
		};

		//
		// http://lol.zoy.org/blog/2013/01/13/fast-rgb-to-hsv
		//
		[[nodiscard]]
		inline static HSV RGBAToHSV(double r, double g, double b, double a) noexcept
		{
			double K = 0.0;

			if (g < b)
			{
				std::ranges::swap(g, b);
				K = -360.0;
			}

			if (r < g)
			{
				std::ranges::swap(r, g);
				K = -720.0 / 6.0 - K;
			}

			const double delta = (g - b) * (360.0 / 6.0);
			const double chroma = r - Min(g, b);
			return HSV(Abs(K + delta / (chroma + 1e-20)), chroma / (r + 1e-20), r, a);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	HSV::HSV(const Color color) noexcept
	{
		*this = RGBAToHSV((color.r / 255.0), (color.g / 255.0), (color.b / 255.0), (color.a / 255.0));
	}

	HSV::HSV(const ColorF& color) noexcept
	{
		*this = RGBAToHSV(color.r, color.g, color.b, color.a);
	}

	////////////////////////////////////////////////////////////////
	//
	//	removeSRGBCurve, applySRGBCurve
	//
	////////////////////////////////////////////////////////////////

	ColorF HSV::removeSRGBCurve() const noexcept
	{
		return toColorF().removeSRGBCurve();
	}

	ColorF HSV::applySRGBCurve() const noexcept
	{
		return toColorF().applySRGBCurve();
	}

	////////////////////////////////////////////////////////////////
	//
	//	toColor
	//
	////////////////////////////////////////////////////////////////

	Color HSV::toColor() const noexcept
	{
		return toColor(Color::ToUint8(a));
	}

	Color HSV::toColor(const uint32 alpha) const noexcept
	{
		const double hue01 = Fraction(h / 360.0);
		const double hueF = (hue01 * 6.0);
		const int32 hueI = static_cast<int32>(hueF);
		const double fr = (hueF - hueI);
		const uint8 vals[4] =
		{
			static_cast<uint8>(v * (1.0 - s) * 255.0 + 0.5),
			static_cast<uint8>(v * (1.0 - s * fr) * 255.0 + 0.5),
			static_cast<uint8>(v * (1.0 - s * (1.0 - fr)) * 255.0 + 0.5),
			static_cast<uint8>(v * 255.0 + 0.5)
		};

		return{ vals[HSVConversionTable[hueI][0]],
				vals[HSVConversionTable[hueI][1]],
				vals[HSVConversionTable[hueI][2]],
				static_cast<uint8>(alpha) };
	}

	////////////////////////////////////////////////////////////////
	//
	//	toColorF
	//
	////////////////////////////////////////////////////////////////

	ColorF HSV::toColorF() const noexcept
	{
		return toColorF(a);
	}

	ColorF HSV::toColorF(const double alpha) const noexcept
	{
		const double hue01 = Fraction(h / 360.0);
		const double hueF = (hue01 * 6.0);
		const int32 hueI = static_cast<int32>(hueF);
		const double fr = (hueF - hueI);
		const double vals[4] =
		{
			(v * (1.0 - s)),
			(v * (1.0 - s * fr)),
			(v * (1.0 - s * (1.0 - fr))),
			v
		};

		return{ vals[HSVConversionTable[hueI][0]],
				vals[HSVConversionTable[hueI][1]],
				vals[HSVConversionTable[hueI][2]],
				alpha };
	}

	////////////////////////////////////////////////////////////////
	//
	//	Formatter
	//
	////////////////////////////////////////////////////////////////

	void Formatter(FormatData& formatData, const HSV& value)
	{
		formatData.string.push_back(U'(');
		detail::AppendFloat(formatData.string, value.h);
		formatData.string.append(U", "_sv);
		detail::AppendFloat(formatData.string, value.s);
		formatData.string.append(U", "_sv);
		detail::AppendFloat(formatData.string, value.v);
		formatData.string.append(U", "_sv);
		detail::AppendFloat(formatData.string, value.a);
		formatData.string.push_back(U')');
	}

	////////////////////////////////////////////////////////////////
	//
	//	HueToColor
	//
	////////////////////////////////////////////////////////////////

	Color HueToColor(const double hue) noexcept
	{
		const double hue01 = Fraction(hue / 360.0);
		const double hueF = (hue01 * 6.0);
		const int32 hueI = static_cast<int32>(hueF);
		const double fr = (hueF - hueI);
		const uint8 vals[4] =
		{
			0,
			static_cast<uint8>((1.0 - fr) * 255.0 + 0.5),
			static_cast<uint8>((1.0 - (1.0 - fr)) * 255.0 + 0.5),
			255
		};

		return{ vals[HSVConversionTable[hueI][0]],
				vals[HSVConversionTable[hueI][1]],
				vals[HSVConversionTable[hueI][2]],
				255 };
	}

	////////////////////////////////////////////////////////////////
	//
	//	HueToColorF
	//
	////////////////////////////////////////////////////////////////

	ColorF HueToColorF(const double hue) noexcept
	{
		const double hue01 = Fraction(hue / 360.0);
		const double hueF = (hue01 * 6.0);
		const int32 hueI = static_cast<int32>(hueF);
		const double fr = (hueF - hueI);
		const double vals[4] =
		{
			0.0,
			(1.0 - fr),
			(1.0 - (1.0 - fr)),
			1.0
		};

		return{ vals[HSVConversionTable[hueI][0]],
				vals[HSVConversionTable[hueI][1]],
				vals[HSVConversionTable[hueI][2]],
				1.0 };
	}
}

////////////////////////////////////////////////////////////////
//
//	fmt
//
////////////////////////////////////////////////////////////////

s3d::ParseContext::iterator fmt::formatter<s3d::HSV, s3d::char32>::parse(s3d::ParseContext& ctx)
{
	return s3d::FmtHelper::GetFormatTag(tag, ctx);
}

s3d::BufferContext::iterator fmt::formatter<s3d::HSV, s3d::char32>::format(const s3d::HSV& value, s3d::BufferContext& ctx)
{
	if (tag.empty())
	{
		return format_to(ctx.out(), U"({}, {}, {}, {})", value.h, value.s, value.v, value.a);
	}
	else
	{
		const std::u32string format
			= (U"({:" + tag + U"}, {:" + tag + U"}, {:" + tag + U"}, {:" + tag + U"})");
		return format_to(ctx.out(), format, value.h, value.s, value.v, value.a);
	}
}
