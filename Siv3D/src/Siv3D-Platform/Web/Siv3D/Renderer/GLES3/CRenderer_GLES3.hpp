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
# include <Siv3D/Renderer/IRenderer.hpp>
# include "OpenGL.hpp"
# include "BackBuffer/GLES3BackBuffer.hpp"
# include "BlendState/GLES3BlendState.hpp"
# include "RasterizerState/GLES3RasterizerState.hpp"
# include "SamplerState/GLES3SamplerState.hpp"
# include "DepthStencilState/GLES3DepthStencilState.hpp"

namespace s3d
{
	class CTexture_GLES3;
	class CRenderer2D_GLES3;

	class CRenderer_GLES3 final : public ISiv3DRenderer
	{
	public:

		~CRenderer_GLES3() override;

		[[nodiscard]]
		StringView getName() const override;

		void init() override;

		void waitForFrame() override;

		void beginFrame() override;

		void flush() override;

		bool present() override;

		[[nodiscard]]
		SceneStyle& getSceneStyle() noexcept override;

		void configureScene(const SceneConfig& config) override;

		[[nodiscard]]
		SceneConfig getSceneConfig() const noexcept override;

		void setSceneSampleCount(uint32 sampleCount) override;

		[[nodiscard]]
		uint32 getSceneSampleCount() const noexcept override;

		[[nodiscard]]
		const Size& getSceneSize() const noexcept override;

		[[nodiscard]]
		const Size& getSceneBufferSize() const noexcept override;

		[[nodiscard]]
		std::pair<double, RectF> getLetterboxComposition() const noexcept override;

		void updateSceneSize() override;

		void setVSyncEnabled(bool enabled) override;

		[[nodiscard]]
		bool isVSyncEnabled() const override;

		[[nodiscard]]
		bool captureScreenshot() override;

		[[nodiscard]]
		const Image& getScreenCapture() const override;

		[[nodiscard]]
		GLES3BackBuffer& getBackBuffer() noexcept;

		[[nodiscard]]
		GLES3BlendState& getBlendState() noexcept;

		[[nodiscard]]
		GLES3RasterizerState& getRasterizerState() noexcept;

		[[nodiscard]]
		GLES3SamplerState& getSamplerState() noexcept;

		[[nodiscard]]
		GLES3DepthStencilState& getDepthStencilState() noexcept;

	private:

		CTexture_GLES3* m_pTexture = nullptr;

		CRenderer2D_GLES3* m_pRenderer2D = nullptr;

		GLFWwindow* m_window = nullptr;

		GLES3BackBuffer m_backBuffer;

		GLES3BlendState m_blendState;

		GLES3RasterizerState m_rasterizerState;

		GLES3SamplerState m_samplerState;

		GLES3DepthStencilState m_depthStencilState;

		bool m_vSyncEnabled = true;
	};
}
