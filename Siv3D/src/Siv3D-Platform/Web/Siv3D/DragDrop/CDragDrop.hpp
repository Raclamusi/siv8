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
# include <atomic>
# include <Siv3D/DragDrop/IDragDrop.hpp>

namespace s3d
{
	class CDragDrop final : public ISiv3DDragDrop
	{
	public:

		~CDragDrop() override;

		void init() override;

		void update() override;

		[[nodiscard]]
		bool isAcceptingFilePaths() const override;

		[[nodiscard]]
		bool isAcceptingText() const override;

		void acceptFilePaths(bool accept) override;

		void acceptText(bool accept) override;

		[[nodiscard]]
		Optional<DragStatus> dragOver() const override;

		[[nodiscard]]
		bool hasNewFilePaths() const override;

		[[nodiscard]]
		bool hasNewText() const override;

		void clear() override;

		[[nodiscard]]
		Array<DroppedFilePath> extractDroppedFilePaths() override;

		[[nodiscard]]
		Array<DroppedText> extractDroppedTexts() override;

		[[nodiscard]]
		bool beginDragFile(FilePathView path) override;

		[[nodiscard]]
		bool beginDragFiles(const Array<FilePath>& paths) override;

		[[nodiscard]]
		bool beginDragText(StringView text) override;

	private:

		std::atomic_bool m_acceptFilePaths{ true };

		std::atomic_bool m_acceptText{ false };

		Array<DroppedFilePath> m_droppedFilePaths;

		Array<DroppedText> m_droppedTexts;

		Optional<DragStatus> m_dragOver;
	};
}
