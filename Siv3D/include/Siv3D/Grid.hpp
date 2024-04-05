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
# include "PointVector.hpp"

namespace s3d
{
	template <class Type, class Allocator>
	class Grid;

	/// @brief 二次元配列クラス
	/// @tparam Type 要素の型
	/// @tparam Allocator アロケータの型
	template <class Type, class Allocator = std::allocator<Type>>
	class Grid
	{
	public:

		using container_type			= Array<Type, Allocator>;
		using value_type				= typename container_type::value_type;
		using pointer					= typename container_type::pointer;
		using const_pointer				= typename container_type::const_pointer;
		using reference					= typename container_type::reference;
		using const_reference			= typename container_type::const_reference;
		using iterator					= typename container_type::iterator;
		using const_iterator			= typename container_type::const_iterator;
		using reverse_iterator			= typename container_type::reverse_iterator;
		using const_reverse_iterator	= typename container_type::const_reverse_iterator;
		using size_type					= typename container_type::size_type;
		using difference_type			= typename container_type::difference_type;
		using allocator_type			= typename container_type::allocator_type;

		////////////////////////////////////////////////////////////////
		//
		//	(constructor)
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		Grid() = default;

		[[nodiscard]]
		Grid(const Grid&) = default;

		[[nodiscard]]
		Grid(Grid&&) = default;

		/// @brief 二次元配列を作成します。
		/// @param w 幅
		/// @param h 高さ
		[[nodiscard]]
		constexpr Grid(size_type w, size_type h);

		/// @brief 二次元配列を作成します。
		/// @param w 幅
		/// @param h 高さ
		/// @param value 要素の初期値
		[[nodiscard]]
		constexpr Grid(size_type w, size_type h, const value_type& value);

		/// @brief 二次元配列を作成します。
		/// @param size 幅と高さ
		[[nodiscard]]
		constexpr explicit Grid(Size size);

		/// @brief 二次元配列を作成します。
		/// @param size 幅と高さ
		/// @param value 要素の初期値
		[[nodiscard]]
		constexpr Grid(Size size, const value_type& value);

		/// @brief 配列から二次元配列を作成します。
		/// @param w 幅
		/// @param h 高さ
		/// @param data 幅 × 高さの要素数を持つ配列
		[[nodiscard]]
		constexpr Grid(size_type w, size_type h, const Array<value_type>& data);

		/// @brief 配列から二次元配列を作成します。
		/// @param w 幅
		/// @param h 高さ
		/// @param data 幅 × 高さの要素数を持つ配列
		[[nodiscard]]
		constexpr Grid(size_type w, size_type h, Array<value_type>&& data);

		/// @brief 配列から二次元配列を作成します。
		/// @param size 幅と高さ
		/// @param data 幅 × 高さの要素数を持つ配列
		[[nodiscard]]
		constexpr Grid(Size size, const Array<value_type>& data);

		/// @brief 配列から二次元配列を作成します。
		/// @param size 幅と高さ
		/// @param data 幅 × 高さの要素数を持つ配列
		[[nodiscard]]
		constexpr Grid(Size size, Array<value_type>&& data);

		[[nodiscard]]
		constexpr Grid(const std::initializer_list<std::initializer_list<value_type>>& set);

		[[nodiscard]]
		constexpr Grid(size_type w, size_type h, Arg::generator_<FunctionRef<value_type()>> generator);

		[[nodiscard]]
		constexpr Grid(size_type w, size_type h, Arg::generator_<FunctionRef<value_type(int32, int32)>> generator);

		[[nodiscard]]
		constexpr Grid(size_type w, size_type h, Arg::generator_<FunctionRef<value_type(Point)>> generator);

		[[nodiscard]]
		constexpr Grid(Size size, Arg::generator_<FunctionRef<value_type()>> generator);

		[[nodiscard]]
		constexpr Grid(Size size, Arg::generator_<FunctionRef<value_type(int32, int32)>> generator);

		[[nodiscard]]
		constexpr Grid(Size size, Arg::generator_<FunctionRef<value_type(Point)>> generator);

		////////////////////////////////////////////////////////////////
		//
		//	operator =
		//
		////////////////////////////////////////////////////////////////

		/// @brief コピー代入演算子
		/// @param other コピーする配列
		/// @return *this
		Grid& operator =(const Grid&) = default;

		/// @brief ムーブ代入演算子
		/// @param other ムーブする配列
		/// @return *this
		Grid& operator =(Grid&&) = default;

		////////////////////////////////////////////////////////////////
		//
		//	assign
		//
		////////////////////////////////////////////////////////////////

		constexpr Grid& assign(size_type w, size_type h, const value_type& value);

		constexpr Grid& assign(Size size, const value_type& value);

		constexpr Grid& assign(const std::initializer_list<std::initializer_list<value_type>>& set);

		////////////////////////////////////////////////////////////////
		//
		//	get_allocator
		//
		////////////////////////////////////////////////////////////////

		/// @brief アロケータを返します。
		/// @return アロケータ
		[[nodiscard]]
		constexpr allocator_type get_allocator() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	getContainer
		//
		////////////////////////////////////////////////////////////////

		/// @brief Array への const 参照を返します。
		/// @return Array への const 参照
		[[nodiscard]]
		constexpr const container_type& getContainer() const& noexcept;

		/// @brief Array を返します。
		/// @return Array
		[[nodiscard]]
		constexpr container_type getContainer() && noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	at
		//
		////////////////////////////////////////////////////////////////

		constexpr reference at(size_type y, size_type x)&;

		constexpr const_reference at(size_type y, size_type x) const&;

		constexpr value_type at(size_type y, size_type x)&&;

		constexpr reference at(Point pos)&;

		constexpr const_reference at(Point pos) const&;

		constexpr value_type at(Point pos)&&;

		////////////////////////////////////////////////////////////////
		//
		//	operator []
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr pointer operator [](size_t y);

		[[nodiscard]]
		constexpr const_pointer operator [](size_t y) const;

		[[nodiscard]]
		constexpr reference operator [](Point pos)&;

		[[nodiscard]]
		constexpr const_reference operator [](Point pos) const&;

		[[nodiscard]]
		constexpr value_type operator [](Point pos)&&;

	# ifdef __cpp_multidimensional_subscript

		[[nodiscard]]
		constexpr reference operator [](size_t x, size_t y)&;

		[[nodiscard]]
		constexpr const_reference operator [](size_t x, size_t y) const&;

		[[nodiscard]]
		constexpr value_type operator [](size_t x, size_t y)&&;

	# endif

		////////////////////////////////////////////////////////////////
		//
		//	front
		//
		////////////////////////////////////////////////////////////////

		/// @brief 先頭の要素への参照を返します。
		/// @return 先頭の要素への参照
		[[nodiscard]]
		constexpr reference front() & noexcept;

		/// @brief 先頭の要素への参照を返します。
		/// @return 先頭の要素への参照
		[[nodiscard]]
		constexpr const_reference front() const& noexcept;

		/// @brief 先頭の要素を返します。
		/// @return 先頭の要素
		[[nodiscard]]
		constexpr value_type front() && noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	back
		//
		////////////////////////////////////////////////////////////////

		/// @brief 末尾の要素への参照を返します。
		/// @return 末尾の要素への参照
		[[nodiscard]]
		constexpr reference back() & noexcept;

		/// @brief 末尾の要素への参照を返します。
		/// @return 末尾の要素への参照
		[[nodiscard]]
		constexpr const_reference back() const& noexcept;

		/// @brief 末尾の要素を返します。
		/// @return 末尾の要素
		[[nodiscard]]
		constexpr value_type back() && noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	inBounds
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr bool inBounds(Point pos) const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	data
		//
		////////////////////////////////////////////////////////////////

		/// @brief 先頭の要素を指すポインタを返します。
		/// @return 先頭の要素を指すポインタ
		[[nodiscard]]
		constexpr pointer data() noexcept;

		/// @brief 先頭の要素を指すポインタを返します。
		/// @return 先頭の要素を指すポインタ
		[[nodiscard]]
		constexpr const_pointer data() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	begin, end
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列の先頭位置を指すイテレータを返します。
		/// @return 配列の先頭位置を指すイテレータ
		[[nodiscard]]
		constexpr iterator begin() noexcept;

		/// @brief 配列の終端位置を指すイテレータを返します。
		/// @remark 有効な範囲は [begin, end) であるため、この位置に要素は存在しません
		/// @return 配列の終端位置を指すイテレータ
		[[nodiscard]]
		constexpr iterator end() noexcept;

		/// @brief 配列の先頭位置を指すイテレータを返します。
		/// @return 配列の先頭位置を指すイテレータ
		[[nodiscard]]
		constexpr const_iterator begin() const noexcept;

		/// @brief 配列の終端位置を指すイテレータを返します。
		/// @remark 有効な範囲は [begin, end) であるため、この位置に要素は存在しません
		/// @return 配列の終端位置を指すイテレータ
		[[nodiscard]]
		constexpr const_iterator end() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	cbegin, cend
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列の先頭位置を指すイテレータを返します。
		/// @return 配列の先頭位置を指すイテレータ
		[[nodiscard]]
		constexpr const_iterator cbegin() const noexcept;

		/// @brief 配列の終端位置を指すイテレータを返します。
		/// @remark 有効な範囲は [begin, end) であるため、この位置に要素は存在しません
		/// @return 配列の終端位置を指すイテレータ
		[[nodiscard]]
		constexpr const_iterator cend() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	rbegin, rend
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列の末尾位置を指すリバース・イテレータを返します。
		/// @return 配列の末尾位置を指すリバース・イテレータ
		[[nodiscard]]
		constexpr reverse_iterator rbegin() noexcept;

		/// @brief 配列の先端位置を指すリバース・イテレータを返します。
		/// @remark 有効な範囲は [rbegin, rend) であるため、この位置に要素は存在しません
		/// @return 配列の先端位置を指すリバース・イテレータ
		[[nodiscard]]
		constexpr reverse_iterator rend() noexcept;

		/// @brief 配列の末尾位置を指すリバース・イテレータを返します。
		/// @return 配列の末尾位置を指すリバース・イテレータ
		[[nodiscard]]
		constexpr const_reverse_iterator rbegin() const noexcept;

		/// @brief 配列の先端位置を指すリバース・イテレータを返します。
		/// @remark 有効な範囲は [rbegin, rend) であるため、この位置に要素は存在しません
		/// @return 配列の先端位置を指すリバース・イテレータ
		[[nodiscard]]
		constexpr const_reverse_iterator rend() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	crbegin, crend
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列の末尾位置を指すリバース・イテレータを返します。
		/// @return 配列の末尾位置を指すリバース・イテレータ
		[[nodiscard]]
		constexpr const_reverse_iterator crbegin() const noexcept;

		/// @brief 配列の先端位置を指すリバース・イテレータを返します。
		/// @remark 有効な範囲は [rbegin, rend) であるため、この位置に要素は存在しません
		/// @return 配列の先端位置を指すリバース・イテレータ
		[[nodiscard]]
		constexpr const_reverse_iterator crend() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	empty
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列が空であるかを返します。
		/// @return 配列が空である場合 true, それ以外の場合は false
		[[nodiscard]]
		constexpr bool empty() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	isEmpty
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列が空であるかを返します。
		/// @remark `empty()` と同じです。
		/// @return 配列が空である場合 true, それ以外の場合は false
		[[nodiscard]]
		constexpr bool isEmpty() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	operator bool
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列が要素を持っているかを返します。
		/// @return 配列が要素を持っている場合 true, それ以外の場合は false
		[[nodiscard]]
		constexpr explicit operator bool() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	width, height
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr int32 width() const noexcept;

		[[nodiscard]]
		constexpr int32 height() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	size
		//
		////////////////////////////////////////////////////////////////
	
		[[nodiscard]]
		constexpr Size size() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	num_elements
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr size_t num_elements() const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	size_bytes
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr size_t size_bytes() const noexcept requires (Concept::TriviallyCopyable<value_type>);

		////////////////////////////////////////////////////////////////
		//
		//	reserve
		//
		////////////////////////////////////////////////////////////////

		constexpr void reserve(size_type w, size_type h);

		constexpr void reserve(Size size);

		////////////////////////////////////////////////////////////////
		//
		//	capacity
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr size_t capacity() const noexcept;

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

		/// @brief 配列を空にします。
		constexpr void clear() noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	release
		//
		////////////////////////////////////////////////////////////////

		/// @brief 配列の要素を全て消去し、メモリも解放します。
		constexpr void release();

		////////////////////////////////////////////////////////////////
		//
		//	swap
		//
		////////////////////////////////////////////////////////////////

		/// @brief 他の配列と要素を入れ替えます。
		/// @param other 入れ替える配列
		constexpr void swap(Grid& other) noexcept;




		////////////////////////////////////////////////////////////////
		//
		//	row
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr auto row(size_t y) noexcept;

		[[nodiscard]]
		constexpr auto row(size_t y) const noexcept;

		////////////////////////////////////////////////////////////////
		//
		//	column
		//
		////////////////////////////////////////////////////////////////

		[[nodiscard]]
		constexpr auto column(size_t x) noexcept;

		[[nodiscard]]
		constexpr auto column(size_t x) const noexcept;







		////////////////////////////////////////////////////////////////
		//
		//	operator <<
		//
		////////////////////////////////////////////////////////////////

		template <class CharType>
		friend std::basic_ostream<CharType>& operator <<(std::basic_ostream<CharType>& output, const Grid& value)
		{
			return output << Format(value);
		}

		////////////////////////////////////////////////////////////////
		//
		//	operator ==
		//
		////////////////////////////////////////////////////////////////

		/// @brief 2 つの配列が等しいかを返します。
		/// @param lhs 一方の配列
		/// @param rhs もう一方の配列
		/// @return 2 つの配列が等しい場合 true, それ以外の場合は false
		[[nodiscard]]
		friend constexpr bool operator ==(const Grid& lhs, const Grid& rhs)
		{
			return ((lhs.m_size == rhs.m_size)
				 && (lhs.m_container == rhs.m_container));
		}

		////////////////////////////////////////////////////////////////
		//
		//	swap
		//
		////////////////////////////////////////////////////////////////

		/// @brief 2 つの配列を入れ替えます。
		/// @param lhs 一方の配列
		/// @param rhs もう一方の配列
		friend constexpr void swap(Grid& lhs, Grid& rhs) noexcept
		{
			lhs.swap(rhs);
		}

	private:

		Size m_size{ 0, 0 };

		container_type m_container;

		[[noreturn]]
		static void ThrowAtOutOfRange();
	};

	////////////////////////////////////////////////////////////////
	//
	//	Formatter
	//
	////////////////////////////////////////////////////////////////

	template <class Type, class Allocator = std::allocator<Type>>
	void Formatter(FormatData& formatData, const Grid<Type, Allocator>& v);

	////////////////////////////////////////////////////////////////
	//
	//	(deduction guide)
	//
	////////////////////////////////////////////////////////////////

	template <class Type>
	Grid(size_t, size_t, Type) -> Grid<Type>;

	template <class Type>
	Grid(Size, Type) -> Grid<Type>;

	template <class Type>
	Grid(std::initializer_list<std::initializer_list<Type>>) -> Grid<Type>;
	
	template <class Type>
	Grid(size_t, size_t, Array<Type>) -> Grid<Type>;

	template <class Type>
	Grid(Size, Array<Type>) -> Grid<Type>;
}

# include "detail/Grid.ipp"
