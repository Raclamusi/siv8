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

# include "Siv3DEngine.hpp"
# include <Siv3D/Logger/ILogger.hpp>
# include <Siv3D/Console/IConsole.hpp>

namespace s3d
{
	namespace detail
	{
		template <class Tuple, size_t Index = (std::tuple_size_v<Tuple> -1)>
		void ReleaseTupleReverse(Tuple& tuple)
		{
			if constexpr (Index != static_cast<size_t>(-1))
			{
				std::get<Index>(tuple).release();
				ReleaseTupleReverse<Tuple, (Index - 1)>(tuple);
			}
		}
	}

	Siv3DEngine::Siv3DEngine() noexcept
	{
		pEngine = this;
	}

	Siv3DEngine::~Siv3DEngine()
	{
		detail::ReleaseTupleReverse(m_components);	
		pEngine = nullptr;
	}

	bool Siv3DEngine::isActive() noexcept
	{
		return (pEngine != nullptr);
	}
}