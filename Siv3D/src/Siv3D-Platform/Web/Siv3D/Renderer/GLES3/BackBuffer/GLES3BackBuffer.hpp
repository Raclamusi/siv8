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
# include <Siv3D/Common.hpp>
# include <Siv3D/Scene.hpp>
# include <Siv3D/EngineOption.hpp>
# include <Siv3D/Graphics.hpp>
# include <Siv3D/Image.hpp>
# include <Siv3D/Scene/SceneStyle.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>
# include "GLES3ClearTarget.hpp"
# include "GLES3InternalTexture2D.hpp"

namespace s3d
{
	class CRenderer_GLES3;
	class CShader_GLES3;

	class GLES3BackBuffer
	{
	public:

		~GLES3BackBuffer();

		void init(CRenderer_GLES3* pRenderer);

		void clear(GLES3ClearTarget clearTarget);

		[[nodiscard]]
		SceneStyle& getSceneStyle() noexcept;

		void setSceneResizeMode(ResizeMode resizeMode);

		[[nodiscard]]
		ResizeMode getSceneResizeMode() const noexcept;

		void updateSceneBufferSize();

		void renderSceneToBackBuffer();

		void resizeBackBuffer(Size backBufferSize);

		[[nodiscard]]
		const Size& getBackBufferSize() const noexcept;

		void resizeSceneBuffer(Size size);

		[[nodiscard]]
		const Size& getSceneBufferSize() const noexcept;

		std::pair<double, RectF> getLetterboxComposition() const noexcept;

		void bindSceneTextureAsRenderTarget();

		void capture();

		[[nodiscard]]
		const Image& getScreenCapture() const noexcept;

	private:

		CRenderer_GLES3* m_pRenderer	= nullptr;

		CShader_GLES3* m_pShader		= nullptr;

		Size m_backBufferSize;

		SceneStyle m_sceneStyle;

		ResizeMode m_sceneResizeMode	= Scene::DefaultResizeMode;

		struct SceneBuffer
		{
			uint32 sampleCount = FromEnum(Scene::DefaultMSAASampleCount);

			std::unique_ptr<GLES3InternalTexture2D> msaa;

			std::unique_ptr<GLES3InternalTexture2D> nonMSAA;

			GLES3InternalTexture2D& getSceneTexture() noexcept
			{
				return ((sampleCount == 1) ? *nonMSAA : *msaa);
			}

			const GLES3InternalTexture2D& getSceneTexture() const noexcept
			{
				return ((sampleCount == 1) ? *nonMSAA : *msaa);
			}

		} m_sceneBuffers;

		Image m_screenCaptureImage;

		GLuint m_fullScreenTriangleVAO = 0;

		GLuint m_fullScreenTriangleSampler = 0;

		void drawFullScreenTriangle();
	};
}
