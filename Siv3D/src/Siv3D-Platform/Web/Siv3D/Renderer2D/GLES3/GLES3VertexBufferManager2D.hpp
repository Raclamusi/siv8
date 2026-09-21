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

# pragma once
# include <Siv3D/Array.hpp>
# include <Siv3D/Vertex2D.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>
# include <Siv3D/Renderer2D/Vertex2DBufferPointer.hpp>
# include "BatchInfo2D.hpp"

namespace s3d
{
	class GLES3Renderer2DCommandManager;

	class GLES3VertexBufferManager2D
	{
	public:

		GLES3VertexBufferManager2D();

		~GLES3VertexBufferManager2D();

		bool init();

		[[nodiscard]]
		Vertex2DBufferPointer requestBuffer(uint16 vertexCount, uint32 indexCount, GLES3Renderer2DCommandManager& commandManager);

		[[nodiscard]]
		size_t num_batches() const noexcept;

		void reset();

		void setBuffers();

		[[nodiscard]]
		BatchInfo2D commitBuffers(size_t batchIndex);

	private:

		static constexpr uint32 InitialVertexArraySize	= (1 << 12);	// 4,096
		static constexpr uint32 InitialIndexArraySize	= (1 << 15);	// 32,768

		static constexpr uint32 MaxVertexArraySize		= (1 << 22);	// 4,194,304
		static constexpr uint32 MaxIndexArraySize		= (1 << 22);	// 4,194,304

		static constexpr uint32 VertexBufferSize		= 65535;
		static constexpr uint32 IndexBufferSize			= ((VertexBufferSize + 1) * 4); // 262,144

		struct BatchBufferPos
		{
			uint32 vertexPos	= 0;

			uint32 indexPos		= 0;
		};

		GLuint m_vao			= 0;

		GLuint m_vertexBuffer	= 0;
		uint32 m_vertexBufferWritePos	= 0;

		GLuint m_indexBuffer	= 0;
		uint32 m_indexBufferWritePos	= 0;

		Array<Vertex2D> m_vertexArray;
		uint32 m_vertexArrayWritePos	= 0;

		Array<Vertex2D::IndexType> m_indexArray;
		uint32 m_indexArrayWritePos		= 0;

		Array<BatchBufferPos> m_batches;
	};
}
