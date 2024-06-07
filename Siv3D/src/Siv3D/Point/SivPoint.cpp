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

# include <Siv3D/PointVector.hpp>
# include <Siv3D/FormatData.hpp>
# include <Siv3D/IntFormatter.hpp>
# include <Siv3D/2DShapes.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	asCircle
	//
	////////////////////////////////////////////////////////////////

	Circle Point::asCircle(const double r) const noexcept
	{
		return{ *this, r };
	}

	////////////////////////////////////////////////////////////////
	//
	//	Formatter
	//
	////////////////////////////////////////////////////////////////

	void Formatter(FormatData& formatData, const Point& value)
	{
		constexpr size_t BufferSize = ((detail::Int32Width * 2) + 4 + 1);
		char32 buffer[BufferSize];
		char32* p = buffer;

		*(p++) = U'(';
		detail::AppendInt32(&p, value.x);
		*(p++) = U',';
		*(p++) = U' ';
		detail::AppendInt32(&p, value.y);
		*(p++) = U')';

		formatData.string.append(buffer, (p - buffer));
	}
}

////////////////////////////////////////////////////////////////
//
//	fmt
//
////////////////////////////////////////////////////////////////

fmt::format_context::iterator fmt::formatter<s3d::Point>::format(const s3d::Point& value, fmt::format_context& ctx)
{
	if (tag.empty())
	{
		return fmt::format_to(ctx.out(), "({}, {})", value.x, value.y);
	}
	else
	{
		const std::string format
			= ("({:" + tag + "}, {:" + tag + "})");
		return fmt::vformat_to(ctx.out(), format, fmt::make_format_args(value.x, value.y));
	}
}

s3d::ParseContext::iterator fmt::formatter<s3d::Point, s3d::char32>::parse(s3d::ParseContext& ctx)
{
	return s3d::FmtHelper::GetFormatTag(tag, ctx);
}

s3d::BufferContext::iterator fmt::formatter<s3d::Point, s3d::char32>::format(const s3d::Point& value, s3d::BufferContext& ctx)
{
	if (tag.empty())
	{
		return format_to(ctx.out(), U"({}, {})", value.x, value.y);
	}
	else
	{
		const std::u32string format
			= (U"({:" + tag + U"}, {:" + tag + U"})");
		return format_to(ctx.out(), format, value.x, value.y);
	}
}
