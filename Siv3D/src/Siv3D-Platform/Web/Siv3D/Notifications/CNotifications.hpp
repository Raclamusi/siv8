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
# include <Siv3D/Notifications/INotifications.hpp>

namespace s3d
{
	class CNotifications final : public ISiv3DNotifications
	{
	public:

		~CNotifications() override;

		void init() override;

		NotificationAvailability getAvailability() override;

		void requestPermission() override;

		Optional<NotificationID> show(const NotificationRequest& request) override;

		void dismiss(NotificationID id) override;

		void dismissAll() override;

		Array<NotificationResponse> extractResponses() override;
	};
}
