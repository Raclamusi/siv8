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

# include "CTexture_GLES3.hpp"
# include <Siv3D/Renderer/GLES3/CRenderer_GLES3.hpp>
# include <Siv3D/Shader/GLES3/CShader_GLES3.hpp>
# include <Siv3D/Error/InternalEngineError.hpp>
# include <Siv3D/Engine/Siv3DEngine.hpp>
# include <Siv3D/ImageFormat/BCnDecoder.hpp>
# include <Siv3D/Texture/TextureUtility.hpp>
# include <Siv3D/Renderer2D/IRenderer2D.hpp>
# include <Siv3D/RenderTextureOps/TextureProcessing.hpp>
# include <Siv3D/ScopeExit.hpp>

namespace s3d
{
	////////////////////////////////////////////////////////////////
	//
	//	(destructor)
	//
	////////////////////////////////////////////////////////////////

	CTexture_GLES3::~CTexture_GLES3()
	{
		LOG_SCOPED_DEBUG("CTexture_GLES3::~CTexture_GLES3()");

		m_textures.destroy();
	}

	////////////////////////////////////////////////////////////////
	//
	//	init
	//
	////////////////////////////////////////////////////////////////

	void CTexture_GLES3::init()
	{
		LOG_SCOPED_DEBUG("CTexture_GLES3::init()");

		m_pRenderer	= static_cast<CRenderer_GLES3*>(SIV3D_ENGINE(Renderer));

		// null Texture を管理に登録
		{
			const Image image{ 16, Palette::Yellow };
			const Array<Image> mipmaps = {
				Image{ 8, Palette::Yellow }, Image{ 4, Palette::Yellow },
				Image{ 2, Palette::Yellow }, Image{ 1, Palette::Yellow }
			};

			// null Texture を作成
			auto nullTexture = std::make_unique<GLES3Texture>(image, mipmaps, TextureDesc::Mipmap);

			if (not nullTexture->isInitialized()) // もし作成に失敗していたら
			{
				throw InternalEngineError{ "Failed to create a null texture" };
			}

			// 管理に登録
			m_textures.setNullData(std::move(nullTexture));
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	create
	//
	////////////////////////////////////////////////////////////////

	Texture::IDType CTexture_GLES3::create(std::unique_ptr<IReader> reader, FilePathView pathHint, const TextureDesc desc)
	{
		if (not reader)
		{
			return Texture::IDType::Null();
		}

		if (not reader->isOpen())
		{
			return Texture::IDType::Null();
		}

		const BCnDecoder bcnDecoder{};
		const bool isBCn = bcnDecoder.getImageInfo(*reader).has_value();

		if (isBCn)
		{
			return create(bcnDecoder.decodeNative(std::move(reader), desc.sRGB, pathHint));
		}
		else
		{
			const Image image{ std::move(reader) };
			return create(image.size(), std::as_bytes(std::span{ image }), (desc.sRGB ? TextureFormat::R8G8B8A8_Unorm_SRGB : TextureFormat::R8G8B8A8_Unorm), desc);
		}
	}

	Texture::IDType CTexture_GLES3::create(const Image& image, const std::span<const Image> mipmaps, const TextureDesc desc)
	{
		if (not image)
		{
			return Texture::IDType::Null();
		}

		std::unique_ptr<GLES3Texture> texture;

		if (mipmaps.empty())
		{
			const TextureFormat format = (desc.sRGB ? TextureFormat::R8G8B8A8_Unorm_SRGB : TextureFormat::R8G8B8A8_Unorm);
			texture = std::make_unique<GLES3Texture>(GLES3Texture::NoMipmap{}, image.size(), std::as_bytes(std::span{ image }), format, desc);
		}
		else
		{
			texture = std::make_unique<GLES3Texture>(image, mipmaps, desc);
		}

		if (not texture->isInitialized())
		{
			return Texture::IDType::Null();
		}

		const String info = texture->getDesc().toString();
		return m_textures.add(std::move(texture), info);
	}

	Texture::IDType CTexture_GLES3::create(const Size& size, const std::span<const Byte> data, const TextureFormat& format, const TextureDesc desc)
	{
		if ((size.x <= 0) || (size.y <= 0))
		{
			return Texture::IDType::Null();
		}

		std::unique_ptr<GLES3Texture> texture;

		if ((not desc.hasMipmap) || (size == Size{ 1, 1 }))
		{
			texture = std::make_unique<GLES3Texture>(GLES3Texture::NoMipmap{}, size, data, format, desc);
		}
		else
		{
			texture = std::make_unique<GLES3Texture>(GLES3Texture::GenerateMipmap{}, size, data, format, desc);
		}

		if (not texture->isInitialized())
		{
			return Texture::IDType::Null();
		}

		const String info = texture->getDesc().toString();
		return m_textures.add(std::move(texture), info);
	}

	Texture::IDType CTexture_GLES3::create(const BCnData& bcnData)
	{
		const auto mipmaps = GetValidBCnMipmaps(bcnData);
		if (mipmaps.empty())
		{
			return Texture::IDType::Null();
		}

		std::unique_ptr<GLES3Texture> texture = std::make_unique<GLES3Texture>(bcnData.size, bcnData.format, mipmaps);

		if (not texture->isInitialized())
		{
			return Texture::IDType::Null();
		}

		const String info = texture->getDesc().toString();
		return m_textures.add(std::move(texture), info);
	}

	Texture::IDType CTexture_GLES3::createDynamic(const Size& size, std::span<const Byte> data, const TextureFormat& format, const TextureDesc desc)
	{
		if ((size.x <= 0) || (size.y <= 0))
		{
			return Texture::IDType::Null();
		}

		std::unique_ptr<GLES3Texture> texture;

		if ((not desc.hasMipmap) || (size == Size{ 1, 1 }))
		{
			texture = std::make_unique<GLES3Texture>(GLES3Texture::Dynamic{}, GLES3Texture::NoMipmap{}, size, data, format, desc);
		}
		else
		{
			texture = std::make_unique<GLES3Texture>(GLES3Texture::Dynamic{}, GLES3Texture::GenerateMipmap{}, size, data, format, desc);
		}

		if (not texture->isInitialized())
		{
			return Texture::IDType::Null();
		}

		const String info = texture->getDesc().toString();
		return m_textures.add(std::move(texture), info);
	}

	////////////////////////////////////////////////////////////////
	//
	//	RenderTexture
	//
	////////////////////////////////////////////////////////////////

	Texture::IDType CTexture_GLES3::createRender(const Size& size, const std::span<const Byte> data, const TextureFormat& format, TextureDesc desc, const ColorF& color)
	{
		if (not IsValidRenderTexture(size, data, format))
		{
			return Texture::IDType::Null();
		}

		desc.sRGB = format.isSRGB();
		auto texture = std::make_unique<GLES3Texture>(GLES3Texture::Render{}, size, data, format, desc, color);

		if (not texture->isInitialized())
		{
			return Texture::IDType::Null();
		}

		const String info = texture->getDesc().toString();
		return m_textures.add(std::move(texture), info);
	}

	Texture::IDType CTexture_GLES3::createMSRender(const Size& size, const TextureFormat& format, TextureDesc desc, const ColorF& color, const uint32 sampleCount)
	{
		if (((sampleCount != 2) && (sampleCount != 4)) || not IsValidRenderTexture(size, {}, format))
		{
			LOG_FAIL("MSRenderTexture: Invalid size, format, or sample count");
			return Texture::IDType::Null();
		}

		desc.sRGB = format.isSRGB();
		auto texture = std::make_unique<GLES3Texture>(GLES3Texture::Render{}, size, std::span<const Byte>{}, format, desc, color, sampleCount);

		if (not texture->isInitialized())
		{
			LOG_FAIL(fmt::format("MSRenderTexture: Unsupported settings or allocation failure (size = {}x{}, format = {}, samples = {}, mipmaps = {})",
				size.x, size.y, format.name().toUTF8(), sampleCount, desc.hasMipmap));
			return Texture::IDType::Null();
		}

		const String info = texture->getDesc().toString();
		return m_textures.add(std::move(texture), info);
	}

	////////////////////////////////////////////////////////////////
	//
	//	processRenderTextures
	//
	////////////////////////////////////////////////////////////////

	void CTexture_GLES3::processRenderTextures(const std::span<const detail::TextureProcessingPass> passes)
	{
		using Kernel = detail::TextureProcessingKernel;
		std::array<detail::TextureProcessingBinding, detail::MaxProcessingPasses> bindings;
		bool draws = false;
		bool usesGaussian = false;
		// const auto validateFiltering = [this](const Texture::IDType id)
		// {
		// 	if (not m_filteringSupport[static_cast<size_t>(getFormat(id).value())])
		// 	{
		// 		throw Error{ U"RenderTextureOps: this device cannot linearly filter the texture format" };
		// 	}
		// };
		for (size_t i = 0; i < passes.size(); ++i)
		{
			const auto& pass = passes[i];
			if (pass.kernel == Kernel::Copy)
			{
				continue;
			}
			draws = true;
			usesGaussian |= (pass.kernel == Kernel::Gaussian);
			// if (pass.filter == TextureFilter::Linear)
			// {
			// 	validateFiltering(pass.source);
			// 	if (pass.secondary != Texture::IDType::Null())
			// 	{
			// 		validateFiltering(pass.secondary);
			// 	}
			// }
			bindings[i] = detail::MakeTextureProcessingBinding(pass, getSize(pass.source),
				(pass.secondary == Texture::IDType::Null()) ? Size{ 1, 1 } : getSize(pass.secondary));
		}
		auto* constants = draws ? m_processingConstants._base() : nullptr;
		auto* samples = usesGaussian ? m_gaussianSamples._base() : nullptr;
		if ((draws && not constants) || (usesGaussian && not samples))
		{
			throw InternalEngineError{ "RenderTextureOps: constant buffer creation failed" };
		}
		SIV3D_ENGINE(Renderer2D)->flush();
		const ScopeExit unbind{ [&]
		{
			::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		} };
		auto* shaders = SIV3D_ENGINE(EngineShader);
		auto* shader = static_cast<CShader_GLES3*>(SIV3D_ENGINE(Shader));
		if (draws)
		{
			m_pRenderer->getBlendState().set(BlendState::Opaque, Float4{ 0, 0, 0, 0 }, 1);
			m_pRenderer->getRasterizerState().set(RasterizerState::SolidCullNone, false);
			m_pRenderer->getDepthStencilState().set(DepthStencilState::Default2D);
			shader->setConstantBufferVS(0, constants);
			shader->setConstantBufferPS(0, constants);
			if (usesGaussian)
			{
				shader->setConstantBufferPS(1, samples);
			}
			shader->setVS(shaders->getVS(EngineVS::TextureProcessing).id());
		}
		for (size_t i = 0; i < passes.size(); ++i)
		{
			const auto& pass = passes[i];
			if (pass.kernel == Kernel::Copy)
			{
				const auto [srcX, srcY] = pass.sourceRect.pos;
				const auto [dstX, dstY] = pass.destinationRect.pos;
				const auto [w, h] = pass.sourceRect.size;
				::glBindFramebuffer(GL_READ_FRAMEBUFFER, getFrameBuffer(pass.source));
				::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getFrameBuffer(pass.destination));
				::glBlitFramebuffer(
					srcX, srcY, (srcX + w), (srcY + h),
					dstX, dstY, (dstX + w), (dstY + h),
					GL_COLOR_BUFFER_BIT, GL_NEAREST);
				::glBindFramebuffer(GL_FRAMEBUFFER, 0);
				continue;
			}
			::glBindFramebuffer(GL_FRAMEBUFFER, getFrameBuffer(pass.destination));
			::glViewport(pass.destinationRect.x, pass.destinationRect.y, pass.destinationRect.w, pass.destinationRect.h);
			m_processingConstants = bindings[i].constants;
			if (pass.kernel == Kernel::Gaussian)
			{
				std::copy(pass.samples.begin(), pass.samples.end(), m_gaussianSamples->begin());
				m_gaussianSamples.setActiveSize(pass.samples.size_bytes());
			}
			if (not m_processingConstants._update_if_dirty()
				|| ((pass.kernel == Kernel::Gaussian) && not m_gaussianSamples._update_if_dirty()))
			{
				throw InternalEngineError{ "RenderTextureOps: constant buffer update failed" };
			}
			shader->setPS(shaders->getPS(bindings[i].shader).id());
			shader->usePipeline();
			::glActiveTexture(GL_TEXTURE0 + MakeSamplerSlot(ShaderStage::Pixel, 0));
			::glBindTexture(GL_TEXTURE_2D, getTexture(pass.source));
			if (pass.secondary != Texture::IDType::Null())
			{
				::glActiveTexture(GL_TEXTURE0 + MakeSamplerSlot(ShaderStage::Pixel, 1));
				::glBindTexture(GL_TEXTURE_2D, getTexture(pass.secondary));
			}
			m_pRenderer->getSamplerState().setPS(0, (pass.filter == TextureFilter::Nearest) ? SamplerState::ClampNearest : SamplerState::ClampLinear);
			::glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	////////////////////////////////////////////////////////////////
	//
	//	resolveMSRT
	//
	////////////////////////////////////////////////////////////////

	void CTexture_GLES3::resolveMSRT(const Texture::IDType handleID)
	{
		auto* texture = m_textures[handleID];
		if (texture->getDesc().type != TextureType::MSRender)
		{
			return;
		}
		texture->resolveMSRT();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSampleCount
	//
	////////////////////////////////////////////////////////////////

	uint32 CTexture_GLES3::getSampleCount(const Texture::IDType handleID)
	{
		return m_textures[handleID]->getDesc().multiSampleCount;
	}

	////////////////////////////////////////////////////////////////
	//
	//	clearRT
	//
	////////////////////////////////////////////////////////////////

	void CTexture_GLES3::clearRT(const Texture::IDType handleID, const ColorF& color)
	{
		SIV3D_ENGINE(Renderer2D)->flush();
		m_textures[handleID]->clearRT(color);
	}

	////////////////////////////////////////////////////////////////
	//
	//	readRT
	//
	////////////////////////////////////////////////////////////////

	bool CTexture_GLES3::readRT(const Texture::IDType handleID, const std::span<Byte> destination, const uint32 mipLevel)
	{
		SIV3D_ENGINE(Renderer2D)->flush();
		return m_textures[handleID]->readRT(destination, mipLevel);
	}

	////////////////////////////////////////////////////////////////
	//
	//	release
	//
	////////////////////////////////////////////////////////////////

	void CTexture_GLES3::release(const Texture::IDType handleID)
	{
		m_textures.erase(handleID);
	}

	////////////////////////////////////////////////////////////////
	//
	//	getSize
	//
	////////////////////////////////////////////////////////////////

	Size CTexture_GLES3::getSize(const Texture::IDType handleID)
	{
		return m_textures[handleID]->getDesc().size;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getMipLevels
	//
	////////////////////////////////////////////////////////////////

	uint32 CTexture_GLES3::getMipLevels(const Texture::IDType handleID)
	{
		return m_textures[handleID]->getDesc().mipLevels;
	}

	////////////////////////////////////////////////////////////////
	//
	//	getDesc
	//
	////////////////////////////////////////////////////////////////

	TextureDesc CTexture_GLES3::getDesc(const Texture::IDType handleID)
	{
		const auto& desc = m_textures[handleID]->getDesc();
		return{ desc.hasMipmap, desc.sRGB, desc.isSDF };
	}

	////////////////////////////////////////////////////////////////
	//
	//	getFormat
	//
	////////////////////////////////////////////////////////////////

	TextureFormat CTexture_GLES3::getFormat(const Texture::IDType handleID)
	{
		return m_textures[handleID]->getDesc().format;
	}

	////////////////////////////////////////////////////////////////
	//
	//	hasDepth
	//
	////////////////////////////////////////////////////////////////

	bool CTexture_GLES3::hasDepth(const Texture::IDType handleID)
	{
		return m_textures[handleID]->hasDepth();
	}

	////////////////////////////////////////////////////////////////
	//
	//	fill
	//
	////////////////////////////////////////////////////////////////

	bool CTexture_GLES3::fill(const Texture::IDType handleID, const ColorF& color, const bool wait)
	{
		return m_textures[handleID]->fill(color, wait);
	}

	bool CTexture_GLES3::fill(const Texture::IDType handleID, const std::span<const Byte> src, const uint32 srcBytesPerRow, const bool wait)
	{
		return m_textures[handleID]->fill(src, srcBytesPerRow, wait);
	}

	////////////////////////////////////////////////////////////////
	//
	//	fillRegion
	//
	////////////////////////////////////////////////////////////////

	bool CTexture_GLES3::fillRegion(const Texture::IDType handleID, const ColorF& color, const Rect& rect, const bool wait)
	{
		return m_textures[handleID]->fillRegion(color, rect, wait);
	}

	bool CTexture_GLES3::fillRegion(const Texture::IDType handleID, const std::span<const Byte> src, const uint32 srcBytesPerRow, const Rect& rect, const bool wait)
	{
		return m_textures[handleID]->fillRegion(src, srcBytesPerRow, rect, wait);
	}

	////////////////////////////////////////////////////////////////
	//
	//	generateMips
	//
	////////////////////////////////////////////////////////////////

	void CTexture_GLES3::generateMips(const Texture::IDType handleID)
	{
		m_textures[handleID]->generateMipmaps();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getTexture
	//
	////////////////////////////////////////////////////////////////

	GLuint CTexture_GLES3::getTexture(const Texture::IDType handleID)
	{
		return m_textures[handleID]->getTexture();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getMultiSampleTexture
	//
	////////////////////////////////////////////////////////////////

	GLuint CTexture_GLES3::getMultiSampleTexture(const Texture::IDType handleID)
	{
		return m_textures[handleID]->getMultiSampleTexture();
	}

	////////////////////////////////////////////////////////////////
	//
	//	getFrameBuffer
	//
	////////////////////////////////////////////////////////////////

	GLuint CTexture_GLES3::getFrameBuffer(const Texture::IDType handleID)
	{
		return m_textures[handleID]->getFrameBuffer();
	}
}
