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

# include "GLES3VertexBufferManager2D.hpp"
# include "GLES3Renderer2DCommandManager.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	namespace
	{
		static void Resize(Array<Vertex2D>& vertexArray, const size_t vertexArrayWritePosTarget)
		{
			const size_t newVertexArraySize = std::bit_ceil(vertexArrayWritePosTarget);

			LOG_TRACE(fmt::format("ℹ️ Resized GLES3VertexBufferManager2D::m_vertexArray (size: {} -> {})", vertexArray.size(), newVertexArraySize));

			vertexArray.resize(newVertexArraySize);
		}

		static void Resize(Array<Vertex2D::IndexType>& indexArray, const size_t indexArrayWritePosTarget)
		{
			const size_t newIndexArraySize = std::bit_ceil(indexArrayWritePosTarget);

			LOG_TRACE(fmt::format("ℹ️ Resized GLES3VertexBufferManager2D::m_indexArray (size: {} -> {})", indexArray.size(), newIndexArraySize));

			indexArray.resize(newIndexArraySize);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	(constructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3VertexBufferManager2D::GLES3VertexBufferManager2D()
		: m_vertexArray(InitialVertexArraySize)
		, m_indexArray(InitialIndexArraySize)
		, m_batches(1) {}

	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	GLES3VertexBufferManager2D::~GLES3VertexBufferManager2D()
	{
		if (m_indexBuffer)
		{
			::glDeleteBuffers(1, &m_indexBuffer);
			m_indexBuffer = 0;
		}

		if (m_vertexBuffer)
		{
			::glDeleteBuffers(1, &m_vertexBuffer);
			m_vertexBuffer = 0;
		}

		if (m_vao)
		{
			::glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	bool GLES3VertexBufferManager2D::init()
	{
		::glGenVertexArrays(1, &m_vao);
		::glGenBuffers(1, &m_vertexBuffer);
		::glGenBuffers(1, &m_indexBuffer);

		::glBindVertexArray(m_vao);
		{
			{
				::glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
				::glBufferData(GL_ARRAY_BUFFER, (sizeof(Vertex2D) * VertexBufferSize), nullptr, GL_DYNAMIC_DRAW);
			}

			{
				::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<const void*>(offsetof(Vertex2D, pos)));
				::glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<const void*>(offsetof(Vertex2D, tex)));
				::glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<const void*>(offsetof(Vertex2D, color)));

				::glEnableVertexAttribArray(0);
				::glEnableVertexAttribArray(1);
				::glEnableVertexAttribArray(2);
			}

			{
				::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
				::glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(Vertex2D::IndexType) * IndexBufferSize), nullptr, GL_DYNAMIC_DRAW);
			}
		}
		::glBindVertexArray(0);

		return true;
	}

	////////////////////////////////////////////////////////////////
	//
	//	requestBuffer
	//
	////////////////////////////////////////////////////////////////

	Vertex2DBufferPointer GLES3VertexBufferManager2D::requestBuffer(const uint16 vertexCount, const uint32 indexCount, GLES3Renderer2DCommandManager& commandManager)
	{
		// VB
		{
			const uint32 vertexArrayWritePosTarget = (m_vertexArrayWritePos + vertexCount);

			if (m_vertexArray.size() < vertexArrayWritePosTarget)
			{
				if (MaxVertexArraySize < vertexArrayWritePosTarget)
				{
					return{ nullptr, 0, 0 };
				}

				Resize(m_vertexArray, vertexArrayWritePosTarget);
			}
		}

		// IB
		{
			const uint32 indexArrayWritePosTarget = (m_indexArrayWritePos + indexCount);

			if (m_indexArray.size() < indexArrayWritePosTarget)
			{
				if (MaxIndexArraySize < indexArrayWritePosTarget)
				{
					return{ nullptr, 0, 0 };
				}

				Resize(m_indexArray, indexArrayWritePosTarget);
			}
		}

		BatchBufferPos* pLastBatch = &m_batches.back();
		{
			if ((VertexBufferSize < (pLastBatch->vertexPos + vertexCount)
				|| (IndexBufferSize < (pLastBatch->indexPos + indexCount))))
			{
				commandManager.pushUpdateBuffers(static_cast<uint32>(m_batches.size()));
				m_batches.emplace_back();
				pLastBatch = &m_batches.back();
			}
		}

		const Vertex2DBufferPointer result
		{
			.pVertex		= (m_vertexArray.data() + m_vertexArrayWritePos),
			.pIndex			= (m_indexArray.data() + m_indexArrayWritePos),
			.indexOffset	= static_cast<Vertex2D::IndexType>(pLastBatch->vertexPos),
		};

		m_vertexArrayWritePos	+= vertexCount;
		m_indexArrayWritePos	+= indexCount;

		pLastBatch->vertexPos	+= vertexCount;
		pLastBatch->indexPos	+= indexCount;

		return result;
	}

	////////////////////////////////////////////////////////////////
	//
	//	num_batches
	//
	////////////////////////////////////////////////////////////////

	size_t GLES3VertexBufferManager2D::num_batches() const noexcept
	{
		return m_batches.size();
	}

	////////////////////////////////////////////////////////////////
	//
	//	reset
	//
	////////////////////////////////////////////////////////////////

	void GLES3VertexBufferManager2D::reset()
	{
		m_batches.assign(1, BatchBufferPos{});
		m_vertexArrayWritePos	= 0;
		m_indexArrayWritePos	= 0;
	}

	////////////////////////////////////////////////////////////////
	//
	//	setBuffers
	//
	////////////////////////////////////////////////////////////////

	void GLES3VertexBufferManager2D::setBuffers()
	{
		::glBindVertexArray(m_vao);
		::glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	}

	////////////////////////////////////////////////////////////////
	//
	//	commitBuffers
	//
	////////////////////////////////////////////////////////////////

	BatchInfo2D GLES3VertexBufferManager2D::commitBuffers(const size_t batchIndex)
	{
		assert(batchIndex < m_batches.size());

		size_t vertexArrayReadPos	= 0;
		size_t indexArrayReadPos	= 0;

		for (size_t i = 0; i < batchIndex; ++i)
		{
			const auto& batch	= m_batches[i];
			vertexArrayReadPos	+= batch.vertexPos;
			indexArrayReadPos	+= batch.indexPos;
		}

		::glBindVertexArray(m_vao);
		::glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		BatchInfo2D batchInfo;
		const auto& currentBatch = m_batches[batchIndex];

		// VB
		if (const uint32 vertexCount = currentBatch.vertexPos)
		{
			const Vertex2D* pSrc = (m_vertexArray.data() + vertexArrayReadPos);

			if (VertexBufferSize < (m_vertexBufferWritePos + vertexCount))
			{
				::glBufferData(GL_ARRAY_BUFFER, (sizeof(Vertex2D) * VertexBufferSize), nullptr, GL_DYNAMIC_DRAW);
				m_vertexBufferWritePos = 0;
			}

			const size_t sizeBytes = (sizeof(Vertex2D) * vertexCount);

			void* const pDst = ::glMapBufferRange(GL_ARRAY_BUFFER,
				(sizeof(Vertex2D) * m_vertexBufferWritePos), sizeBytes,
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			std::memcpy(pDst, pSrc, sizeBytes);
			::glUnmapBuffer(GL_ARRAY_BUFFER);

			batchInfo.baseVertexLocation = m_vertexBufferWritePos;
			m_vertexBufferWritePos += vertexCount;
		}

		// IB
		if (const uint32 indexCount = currentBatch.indexPos)
		{
			const Vertex2D::IndexType* pSrc = (m_indexArray.data() + indexArrayReadPos);

			if (IndexBufferSize < (m_indexBufferWritePos + indexCount))
			{
				::glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(Vertex2D::IndexType) * IndexBufferSize), nullptr, GL_DYNAMIC_DRAW);
				m_indexBufferWritePos = 0;
			}

			const size_t sizeBytes = (sizeof(Vertex2D::IndexType) * indexCount);

			void* const pDst = ::glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER,
				(sizeof(Vertex2D::IndexType) * m_indexBufferWritePos), sizeBytes,
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			std::memcpy(pDst, pSrc, sizeBytes);
			::glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

			batchInfo.indexCount = indexCount;
			batchInfo.startIndexLocation = m_indexBufferWritePos;
			m_indexBufferWritePos += indexCount;
		}

		return batchInfo;
	}
}
