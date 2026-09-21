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

# include "CNotifications.hpp"
# include <Siv3D/EngineLog.hpp>

namespace s3d
{
	CNotifications::~CNotifications()
	{
		LOG_SCOPED_DEBUG("CNotifications::~CNotifications()");

		dismissAll();
	}

	void CNotifications::init()
	{
		LOG_SCOPED_DEBUG("CNotifications::init()");
	}

	NotificationAvailability CNotifications::getAvailability()
	{
		// TODO
		return NotificationAvailability::Unavailable;
	}

	void CNotifications::requestPermission()
	{
		// TODO
	}

	Optional<NotificationID> CNotifications::show(const NotificationRequest&)
	{
		if (getAvailability() != NotificationAvailability::Available)
		{
			return none;
		}

		// TODO
		return none;
	}

	void CNotifications::dismiss(const NotificationID)
	{
		if (getAvailability() != NotificationAvailability::Available)
		{
			return;
		}

		// TODO
	}

	void CNotifications::dismissAll()
	{
		if (getAvailability() != NotificationAvailability::Available)
		{
			return;
		}

		// TODO
	}

	Array<NotificationResponse> CNotifications::extractResponses()
	{
		// TODO
		return {};
	}
}
