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

# include "Vertex2DBuilder.hpp"
# include <Siv3D/LineStyle.hpp>

namespace s3d
{
	namespace
	{
		static constexpr Vertex2D::IndexType RectIndexTable[6] = { 0, 1, 2, 2, 1, 3 };

		static const std::array<Float2, 2016> SinCosTable = []()
		{
			std::array<Float2, 2016> table;

			Float2* pDst = table.data();

			for (int32 quality = 1; quality <= 63; ++quality)
			{
				const float radDelta = (Math::HalfPiF / quality);

				for (int32 i = 0; i < quality; ++i)
				{
					const float rad = (radDelta * i);
					*pDst++ = { std::sin(rad), -std::cos(rad) };
				}
			}

			return table;
		}();

		[[nodiscard]]
		static constexpr size_t GetSinCosTableIndex(Vertex2D::IndexType quality)
		{
			return ((quality - 1) * quality / 2);
		}

		/// @brief 円の品質を計算します。
		/// @param r 円の半径
		/// @return 円の品質
		[[nodiscard]]
		static constexpr Vertex2D::IndexType CalculateCircleQuality(const float r) noexcept
		{
			if (r <= 2.0f)
			{
				return 2;
			}
			else if (r <= 4.0f)
			{
				return 3;
			}
			else if (r < 8.0f)
			{
				return 4;
			}
			else
			{
				return static_cast<Vertex2D::IndexType>(Min((5 + static_cast<int32>((r - 8.0f) / 8.0f)), 63));
			}
		}
	}

	namespace Vertex2DBuilder
	{
		////////////////////////////////////////////////////////////////
		//
		//	BuildLine
		//
		////////////////////////////////////////////////////////////////

		Vertex2D::IndexType BuildLine(const BufferCreatorFunc& bufferCreator, const LineStyle& style, const Float2& start, const Float2& end, const float thickness, const Float4(&colors)[2], const float scale)
		{
			if (style.hasSquareCap())
			{
				return BuildCappedLine(bufferCreator, start, end, thickness, colors);
			}
			else if (style.hasNoCap())
			{
				return BuildUncappedLine(bufferCreator, start, end, thickness, colors);
			}
			//else if (style.hasRoundCap())
			//{
			//	float startAngle = 0.0f;
			//	Vertex2D::IndexType indexCount = BuildUncappedLine(bufferCreator, start, end, thickness, colors, startAngle);

			//	const float thicknessHalf = (thickness * 0.5f);
			//	indexCount += BuildCirclePie(bufferCreator, begin, thicknessHalf, startAngle, Math::PiF, colors[0], colors[0], scale);
			//	indexCount += BuildCirclePie(bufferCreator, end, thicknessHalf, startAngle + Math::PiF, Math::PiF, colors[1], colors[1], scale);

			//	return indexCount;
			//}
			//else if (style.hasSquareDot())
			//{
			//	return BuildSquareDotLine(bufferCreator, start, end, thickness, colors, static_cast<float>(style.dotOffset), scale);
			//}
			//else if (style.hasRoundDot())
			//{
			//	return BuildRoundDotLine(bufferCreator, start, end, thickness, colors, static_cast<float>(style.dotOffset), style.hasAlignedDot);
			//}
			else
			{
				return 0;
			}
		}

		////////////////////////////////////////////////////////////////
		//
		//	BuildCappedLine
		//
		////////////////////////////////////////////////////////////////

		Vertex2D::IndexType BuildCappedLine(const BufferCreatorFunc& bufferCreator, const Float2& start, const Float2& end, const float thickness, const Float4(&colors)[2])
		{
			constexpr Vertex2D::IndexType VertexSize	= 4;
			constexpr Vertex2D::IndexType IndexSize		= 6;
			auto [pVertex, pIndex, indexOffset]			= bufferCreator(VertexSize, IndexSize);

			if (not pVertex)
			{
				return 0;
			}

			const float halfThickness = (thickness * 0.5f);
			const Float2 line = (end - start).normalized();
			const Float2 vNormal{ (-line.y * halfThickness), (line.x * halfThickness) };
			const Float2 lineHalf{ line * halfThickness };
			const Float2 start2	= (start - lineHalf);
			const Float2 end2	= (end + lineHalf);

			pVertex[0].set((start2 + vNormal), colors[0]);
			pVertex[1].set((start2 - vNormal), colors[0]);
			pVertex[2].set((end2 + vNormal), colors[1]);
			pVertex[3].set((end2 - vNormal), colors[1]);

			for (Vertex2D::IndexType i = 0; i < IndexSize; ++i)
			{
				*pIndex++ = (indexOffset + RectIndexTable[i]);
			}

			return IndexSize;
		}

		////////////////////////////////////////////////////////////////
		//
		//	BuildUncappedLine
		//
		////////////////////////////////////////////////////////////////

		Vertex2D::IndexType BuildUncappedLine(const BufferCreatorFunc& bufferCreator, const Float2& start, const Float2& end, const float thickness, const Float4(&colors)[2])
		{
			constexpr Vertex2D::IndexType VertexSize	= 4;
			constexpr Vertex2D::IndexType IndexSize		= 6;
			auto [pVertex, pIndex, indexOffset]			= bufferCreator(VertexSize, IndexSize);

			if (not pVertex)
			{
				return 0;
			}

			const float halfThickness = (thickness * 0.5f);
			const Float2 line = (end - start).normalized();
			const Float2 vNormal{ (-line.y * halfThickness), (line.x * halfThickness) };

			pVertex[0].set((start + vNormal), colors[0]);
			pVertex[1].set((start - vNormal), colors[0]);
			pVertex[2].set((end + vNormal), colors[1]);
			pVertex[3].set((end - vNormal), colors[1]);

			for (Vertex2D::IndexType i = 0; i < IndexSize; ++i)
			{
				*pIndex++ = (indexOffset + RectIndexTable[i]);
			}

			return IndexSize;
		}

		////////////////////////////////////////////////////////////////
		//
		//	BuildTriangle
		//
		////////////////////////////////////////////////////////////////

		Vertex2D::IndexType BuildTriangle(const BufferCreatorFunc& bufferCreator, const Float2(&points)[3], const Float4& color)
		{
			constexpr Vertex2D::IndexType VertexSize	= 3;
			constexpr Vertex2D::IndexType IndexSize		= 3;
			auto [pVertex, pIndex, indexOffset]			= bufferCreator(VertexSize, IndexSize);
			
			if (not pVertex)
			{
				return 0;
			}

			pVertex[0].set(points[0], color);
			pVertex[1].set(points[1], color);
			pVertex[2].set(points[2], color);

			pIndex[0] = indexOffset;
			pIndex[1] = (indexOffset + 1);
			pIndex[2] = (indexOffset + 2);

			return IndexSize;
		}

		Vertex2D::IndexType BuildTriangle(const BufferCreatorFunc& bufferCreator, const Float2(&points)[3], const Float4(&colors)[3])
		{
			constexpr Vertex2D::IndexType VertexSize	= 3;
			constexpr Vertex2D::IndexType IndexSize		= 3;
			auto [pVertex, pIndex, indexOffset]			= bufferCreator(VertexSize, IndexSize);
			
			if (not pVertex)
			{
				return 0;
			}

			pVertex[0].set(points[0], colors[0]);
			pVertex[1].set(points[1], colors[1]);
			pVertex[2].set(points[2], colors[2]);

			pIndex[0] = indexOffset;
			pIndex[1] = (indexOffset + 1);
			pIndex[2] = (indexOffset + 2);

			return IndexSize;
		}

		////////////////////////////////////////////////////////////////
		//
		//	BuildRect
		//
		////////////////////////////////////////////////////////////////

		Vertex2D::IndexType BuildRect(const BufferCreatorFunc& bufferCreator, const FloatRect& rect, const Float4& color)
		{
			constexpr Vertex2D::IndexType VertexSize	= 4;
			constexpr Vertex2D::IndexType IndexSize		= 6;
			auto [pVertex, pIndex, indexOffset]			= bufferCreator(VertexSize, IndexSize);

			if (not pVertex)
			{
				return 0;
			}

			pVertex[0].set(rect.left, rect.top, color);
			pVertex[1].set(rect.right, rect.top, color);
			pVertex[2].set(rect.left, rect.bottom, color);
			pVertex[3].set(rect.right, rect.bottom, color);

			for (Vertex2D::IndexType i = 0; i < IndexSize; ++i)
			{
				*pIndex++ = (indexOffset + RectIndexTable[i]);
			}

			return IndexSize;
		}

		Vertex2D::IndexType BuildRect(const BufferCreatorFunc& bufferCreator, const FloatRect& rect, const Float4(&colors)[4])
		{
			constexpr Vertex2D::IndexType VertexSize	= 4;
			constexpr Vertex2D::IndexType IndexSize		= 6;
			auto [pVertex, pIndex, indexOffset]			= bufferCreator(VertexSize, IndexSize);

			if (not pVertex)
			{
				return 0;
			}

			pVertex[0].set(rect.left, rect.top, colors[0]);
			pVertex[1].set(rect.right, rect.top, colors[1]);
			pVertex[2].set(rect.left, rect.bottom, colors[3]);
			pVertex[3].set(rect.right, rect.bottom, colors[2]);

			for (Vertex2D::IndexType i = 0; i < IndexSize; ++i)
			{
				*pIndex++ = (indexOffset + RectIndexTable[i]);
			}

			return IndexSize;
		}

		////////////////////////////////////////////////////////////////
		//
		//	BuildCircle
		//
		////////////////////////////////////////////////////////////////

		Vertex2D::IndexType BuildCircle(const BufferCreatorFunc& bufferCreator, const Float2& center, const float r, const Float4& innerColor, const Float4& outerColor, const float scale)
		{
			const Vertex2D::IndexType Quality = CalculateCircleQuality(r * scale); // 円周の 1/4 に相当する品質
			const Vertex2D::IndexType FullQuality = (Quality * 4);
			const Vertex2D::IndexType VertexSize = (FullQuality + 1);
			const Vertex2D::IndexType IndexSize = (FullQuality * 3);
			auto [pVertex, pIndex, indexOffset] = bufferCreator(VertexSize, IndexSize);

			if (not pVertex)
			{
				return 0;
			}

			const float centerX = center.x;
			const float centerY = center.y;
			pVertex[0].pos.set(centerX, centerY);

			const Float2* pCS = (SinCosTable.data() + GetSinCosTableIndex(Quality));
			
			Vertex2D* pDst0 = &pVertex[1];
			Vertex2D* pDst1 = (pDst0 + Quality);
			Vertex2D* pDst2 = (pDst1 + Quality);
			Vertex2D* pDst3 = (pDst2 + Quality);

			for (Vertex2D::IndexType i = 0; i < Quality; ++i)
			{
				const Float2* cs = (pCS + i);
				const float x = (cs->x * r);
				const float y = (cs->y * r);

				pDst0->pos.set((centerX + x), (centerY + y));
				pDst1->pos.set((centerX - y), (centerY + x));
				pDst2->pos.set((centerX - x), (centerY - y));
				pDst3->pos.set((centerX + y), (centerY - x));

				++pDst0;
				++pDst1;
				++pDst2;
				++pDst3;
			}

			{
				(pVertex++)->color = innerColor;

				for (size_t i = 1; i < VertexSize; ++i)
				{
					(pVertex++)->color = outerColor;
				}
			}

			{
				for (Vertex2D::IndexType i = 0; i < (FullQuality - 1); ++i)
				{
					*pIndex++ = indexOffset + (i + 1);
					*pIndex++ = indexOffset;
					*pIndex++ = indexOffset + (i + 2);
				}

				*pIndex++ = (indexOffset + FullQuality);
				*pIndex++ = indexOffset;
				*pIndex++ = (indexOffset + 1);
			}

			return IndexSize;
		}
	}
}
