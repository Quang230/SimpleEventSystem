#pragma once
#include "PrimitiveTypes.h"

namespace evt
{
	/*
		Abstact interface for all event types.
	*/
	class EventInterface
	{
	public:
		/*
			Construction and copy control
		*/
		EventInterface() = default;
		EventInterface(const EventInterface&) = default;
		EventInterface(EventInterface&&) = default;
		EventInterface& operator=(const EventInterface&) = default;
		EventInterface& operator=(EventInterface&&) = default;
		virtual ~EventInterface() = 0 {};

		/*
			Returns the ID of the underlying event.
		*/
		EventID_t getDerivedID() const noexcept;

	private:
		/*
			Virtual function called by getDerivedID().
		*/
		virtual EventID_t doGetDerivedID() const noexcept = 0;
	};
}