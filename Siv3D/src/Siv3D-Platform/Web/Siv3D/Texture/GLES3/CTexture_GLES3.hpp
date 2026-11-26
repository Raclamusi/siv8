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
# include <Siv3D/Texture.hpp>
# include <Siv3D/Texture/ITexture.hpp>
# include <Siv3D/Renderer/GLES3/OpenGL.hpp>
# include <Siv3D/AssetHandleManager/AssetHandleManager.hpp>
# include <Siv3D/RenderTextureOps/TextureProcessing.hpp>
# include <Siv3D/ConstantBuffer.hpp>
# include "GLES3Texture.hpp"

namespace s3d
{
	class CRenderer_GLES3;

	class CTexture_GLES3 final : public ISiv3DTexture
	{
	public:

		~CTexture_GLES3() override;

		void init() override;

		[[nodiscard]]
		Texture::IDType create(std::unique_ptr<IReader> reader, FilePathView pathHint, TextureDesc desc) override;

		[[nodiscard]]
		Texture::IDType create(const Image& image, std::span<const Image> mipmaps, TextureDesc desc) override;

		[[nodiscard]]
		Texture::IDType create(const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc) override;

		[[nodiscard]]
		Texture::IDType create(const BCnData& bcnData) override;

		[[nodiscard]]
		Texture::IDType createDynamic(const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc) override;

		[[nodiscard]]
		Texture::IDType createRender(const Size& size, std::span<const Byte> data, const TextureFormat& format, TextureDesc desc, const ColorF& color) override;

		[[nodiscard]]
		Texture::IDType createMSRender(const Size& size, const TextureFormat& format, TextureDesc desc, const ColorF& color, uint32 sampleCount) override;

		void processRenderTextures(std::span<const detail::TextureProcessingPass> passes) override;

		void resolveMSRT(Texture::IDType handleID) override;

		[[nodiscard]]
		uint32 getSampleCount(Texture::IDType handleID) override;

		void clearRT(Texture::IDType handleID, const ColorF& color) override;

		bool readRT(Texture::IDType handleID, std::span<Byte> destination, uint32 mipLevel) override;

		void release(Texture::IDType handleID) override;

		[[nodiscard]]
		Size getSize(Texture::IDType handleID) override;

		[[nodiscard]]
		uint32 getMipLevels(Texture::IDType handleID) override;

		[[nodiscard]]
		TextureDesc getDesc(Texture::IDType handleID) override;

		[[nodiscard]]
		TextureFormat getFormat(Texture::IDType handleID) override;

		[[nodiscard]]
		bool hasDepth(Texture::IDType handleID) override;

		bool fill(Texture::IDType handleID, const ColorF& color, bool wait) override;

		bool fill(Texture::IDType handleID, std::span<const Byte> src, uint32 srcBytesPerRow, bool wait) override;

		bool fillRegion(Texture::IDType handleID, const ColorF& color, const Rect& rect, bool wait) override;

		bool fillRegion(Texture::IDType handleID, std::span<const Byte> src, uint32 srcBytesPerRow, const Rect& rect, bool wait) override;

		void generateMips(Texture::IDType handleID) override;

		[[nodiscard]]
		GLuint getTexture(Texture::IDType handleID);

		[[nodiscard]]
		GLuint getMultiSampleTexture(Texture::IDType handleID);

		[[nodiscard]]
		GLuint getFrameBuffer(Texture::IDType handleID);

	private:

		ConstantBuffer<detail::TextureProcessingConstants> m_processingConstants;

		ConstantBuffer<std::array<Float4, detail::MaxGaussianSamples>> m_gaussianSamples;

		CRenderer_GLES3* m_pRenderer = nullptr;

		// Texture の管理
		AssetHandleManager<Texture::IDType, GLES3Texture> m_textures{ "Texture" };
	};
}
