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

# pragma once
# include "Common.hpp"
# include "Array.hpp"
# include "Byte.hpp"
# include "IReader.hpp"

namespace s3d
{
	class MD5Value;
	class Base64Value;

	////////////////////////////////////////////////////////////////
	//
	//	Blob
	//
	////////////////////////////////////////////////////////////////

	/// @brief メモリ上のバイナリデータ
	class Blob
	{
	public:

		using base_type					= Array<Byte>;
		using iterator					= base_type::iterator;
		using const_iterator			= base_type::const_iterator;
		using reverse_iterator			= base_type::reverse_iterator;
		using const_reverse_iterator	= base_type::const_reverse_iterator;

		////////////////////////////////////////////////////////////////
		//
		//	(constructor)
		//
		////////////////////////////////////////////////////////////////

		/// @brief 空のバイナリデータを作成します。
		[[nodiscard]]
		Blob() = default;

		[[nodiscard]]
		Blob(const Blob&) = default;

		[[nodiscard]]
		Blob(Blob&&) noexcept = default;

		/// @brief 指定したサイズのバイナリデータを作成します。
		/// @param sizeBytes バイナリデータのサイズ（バイト）
		[[nodiscard]]
		explicit constexpr Blob(size_t sizeBytes);

		/// @brief 空のバイナリデータを作成します。
		/// @param reserveSizeBytes reserve するサイズ（バイト）
		[[nodiscard]]
		explicit constexpr Blob(Arg::reserve_<size_t> reserveSizeBytes);

		/// @brief ファイルの中身をコピーしたバイナリデータを作成します。
		/// @param path ファイルパス
		[[nodiscard]]
		explicit Blob(FilePathView path);

		/// @brief IReader からデータを読み込んでバイナリデータを作成します。
		/// @param reader IReader
		[[nodiscard]]
		explicit Blob(IReader& reader);

		/// @brief バイナリデータをコピーして作成します。
		/// @param src コピーするデータの先頭ポインタ
		/// @param sizeBytes コピーするデータのサイズ（バイト）
		[[nodiscard]]
		Blob(const void* src, size_t sizeBytes);

		/// @brief バイナリデータをコピーして作成します。
		/// @param data コピーするデータ
		[[nodiscard]]
		explicit constexpr Blob(const Array<Byte>& data);

		/// @brief バイナリデータをムーブして作成します。
		/// @param data ムーブするデータ
		[[nodiscard]]
		explicit constexpr Blob(Array<Byte>&& data);

		////////////////////////////////////////////////////////////////
		//
		//	operator =
		//
		////////////////////////////////////////////////////////////////

		constexpr Blob& operator =(const Array<Byte>& other);

		constexpr Blob& operator =(Array<Byte>&& other) noexcept;

		constexpr Blob& operator =(const Blob& other);

		constexpr Blob& operator =(Blob&& other) noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	createFrom
		//
		////////////////////////////////////////////////////////////////

		constexpr void createFrom(const Blob& other);

		constexpr void createFrom(Blob&& other);

		void createFrom(const void* src, size_t sizeBytes);

		constexpr void createFrom(const Array<Byte>& data);

		constexpr void createFrom(Array<Byte>&& data);

		////////////////////////////////////////////////////////////////
		//
		//	createFromFile
		//
		////////////////////////////////////////////////////////////////

		bool createFromFile(FilePathView path);

		////////////////////////////////////////////////////////////////
		//
		//	operator []
		//
		////////////////////////////////////////////////////////////////

		/// @brief 指定したインデックスにあるバイナリ値の参照を返します。
		/// @param index インデックス
		/// @return 指定したインデックスにあるバイナリ値の参照
		[[nodiscard]]
		constexpr const Byte& operator [](size_t index) const;

		/// @brief 指定したインデックスにあるバイナリ値の参照を返します。
		/// @param index インデックス
		/// @return 指定したインデックスにあるバイナリ値の参照
		[[nodiscard]]
		constexpr Byte& operator [](size_t index);

		////////////////////////////////////////////////////////////////
		//
		//	data
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータを格納する配列の先頭ポインタを返します。
		/// @return バイナリデータを格納する配列の先頭ポインタ
		[[nodiscard]]
		constexpr const Byte* data() const noexcept;

		/// @brief バイナリデータを格納する配列の先頭ポインタを返します。
		/// @return バイナリデータを格納する配列の先頭ポインタ
		[[nodiscard]]
		constexpr Byte* data() noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	asArray
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータを格納する配列にアクセスします。
		/// @return バイナリデータを格納する配列
		[[nodiscard]]
		constexpr const Array<Byte>& asArray() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	view
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータ全体を参照するビューを返します。
		/// @return バイナリデータ全体を参照するビュー
		[[nodiscard]]
		constexpr std::span<const Byte> view() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	subView
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータの一部を参照するビューを返します。
		/// @param offset 範囲の先頭のオフセット（バイト）
		/// @param count 範囲のサイズ（バイト）
		/// @return バイナリデータの一部を参照するビュー
		[[nodiscard]]
		constexpr std::span<const Byte> subView(size_t offset, size_t count) const;

		////////////////////////////////////////////////////////////////
		//
		//	empty
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータが空であるかを返します。
		/// @return バイナリデータが空である場合 true, それ以外の場合は false
		[[nodiscard]]
		constexpr bool empty() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	isEmpty
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータが空であるかを返します。
		/// @return バイナリデータが空である場合 true, それ以外の場合は false
		/// @remark `empty()` と同じです。
		[[nodiscard]]
		constexpr bool isEmpty() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	operator bool
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータが空でないかを返します。
		/// @return バイナリデータが空でない場合 true, それ以外の場合は false
		[[nodiscard]]
		constexpr explicit operator bool() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	size
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータのサイズ（バイト）を返します。
		/// @remark `size_bytes()` と同じです。
		/// @return バイナリデータのサイズ（バイト）
		[[nodiscard]]
		constexpr size_t size() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	ssize
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータのサイズ（バイト）を返します。
		/// @return バイナリデータのサイズ（バイト）
		[[nodiscard]]
		constexpr isize ssize() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	size_bytes
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータのサイズ（バイト）を返します。
		/// @remark `size` と同じです。
		/// @return バイナリデータのサイズ（バイト）
		[[nodiscard]]
		constexpr size_t size_bytes() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	capacity
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr size_t capacity() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	reserve
		//
		////////////////////////////////////////////////////////////////

		constexpr void reserve(size_t sizeBytes);

		////////////////////////////////////////////////////////////////
		//
		//	resize
		//
		////////////////////////////////////////////////////////////////

		constexpr void resize(size_t sizeBytes);

		////////////////////////////////////////////////////////////////
		//
		//	shrink_to_fit
		//
		////////////////////////////////////////////////////////////////

		constexpr void shrink_to_fit();

		////////////////////////////////////////////////////////////////
		//
		//	clear
		//
		////////////////////////////////////////////////////////////////

		constexpr void clear();

		////////////////////////////////////////////////////////////////
		//
		//	release
		//
		////////////////////////////////////////////////////////////////

		constexpr void release();

		////////////////////////////////////////////////////////////////
		//
		//	begin / end
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr iterator begin() noexcept;

		[[nodiscard]]
		constexpr iterator end() noexcept;

		[[nodiscard]]
		constexpr const_iterator begin() const noexcept;

		[[nodiscard]]
		constexpr const_iterator end() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	cbegin / cend
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr const_iterator cbegin() const noexcept;

		[[nodiscard]]
		constexpr const_iterator cend() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	rbegin / rend
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr reverse_iterator rbegin() noexcept;

		[[nodiscard]]
		constexpr reverse_iterator rend() noexcept;

		[[nodiscard]]
		constexpr const_reverse_iterator rbegin() const noexcept;

		[[nodiscard]]
		constexpr const_reverse_iterator rend() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	crbegin / crend
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr const_reverse_iterator crbegin() const noexcept;

		[[nodiscard]]
		constexpr const_reverse_iterator crend() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	push_back
		//
		////////////////////////////////////////////////////////////////

		constexpr void push_back(Byte value);

		////////////////////////////////////////////////////////////////
		//
		//	pop_back
		//
		////////////////////////////////////////////////////////////////

		constexpr void pop_back();

		////////////////////////////////////////////////////////////////
		//
		//	append
		//
		////////////////////////////////////////////////////////////////

		constexpr void append(const Blob& other);

		/// @brief 末尾にバイナリデータを追加します。
		/// @param src 追加するデータの先頭ポインタ
		/// @param sizeBytes 追加するデータのサイズ
		void append(const void* src, size_t sizeBytes);

		////////////////////////////////////////////////////////////////
		//
		//	insert
		//
		////////////////////////////////////////////////////////////////

		constexpr iterator insert(const_iterator pos, Byte value);

		template <class Iterator>
		constexpr iterator insert(const_iterator pos, Iterator first, Iterator last);

		////////////////////////////////////////////////////////////////
		//
		//	save
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータをファイルに保存します。
		/// @param path ファイルパス
		/// @return 保存に成功した場合 true, それ以外の場合は false
		bool save(FilePathView path) const;

		////////////////////////////////////////////////////////////////
		//
		//	md5
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータから MD5 ハッシュ値を計算します。
		/// @return バイナリデータの MD5 ハッシュ値
		[[nodiscard]]
		MD5Value md5() const;

		////////////////////////////////////////////////////////////////
		//
		//	base64
		//
		////////////////////////////////////////////////////////////////

		/// @brief バイナリデータを Base64 エンコードします。
		/// @return エンコードされたデータ
		[[nodiscard]]
		Base64Value base64() const;

		/// @brief バイナリデータを Base64 エンコードし、dst に格納します。
		/// @param dst エンコードされたデータの格納先
		void base64(Base64Value& dst) const;

		////////////////////////////////////////////////////////////////
		//
		//	operator ==
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		friend constexpr bool operator ==(const Blob& lhs, const Blob& rhs) = default;

	private:

		base_type m_data;
	};
}

# include "detail/Blob.ipp"
