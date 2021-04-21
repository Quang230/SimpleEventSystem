#pragma once
#include <type_traits>
#include "PrimitiveTypes.h"
#include "EventInterface.h"
#include "EventIDGen.h"

namespace evt
{
	/*
		An event of type EventType must derive publicly directly from EventBase<EventType> to be used. 
		By publicly inheriting from EventBase<EventType> the compiler will generate a unique ID to 
		designate events of type EventType. This ID can be accessed either through the static 
		function getID(), or the virtual function getDeriveID(). The indirect base EventInterface 
		can be used as a uniform interface for all events.
	*/
	template<typename EventType>
	class EventBase : public EventInterface
	{
	public:
		/*
			The type of the underlying event.
		*/
		using Event_t = EventType;

		/*
			Default construction cathces the most naive errors.
		*/
		EventBase() noexcept;
		EventBase(const EventBase&) = default;
		EventBase(EventBase&&) = default;
		EventBase& operator=(const EventBase&) = default;
		EventBase& operator=(EventBase&&) = default;
		virtual ~EventBase() = 0 {};

		// Return the unique ID of this event type.
		static EventID_t getID() noexcept;

	private:
		/*
			ID of this event type
		*/
		const inline static EventID_t s_ID{ EventIDGen::genID() };

		/*
			Override the doGetDeriveID() function inherited from EventInterface. 
		*/
		EventID_t doGetDerivedID() const noexcept override;
	};


	template<typename EventType>
	EventBase<EventType>::EventBase() noexcept
	{
		static_assert(
			std::is_base_of_v<EventBase<EventType>, EventType>,
			"Template paramter EventType must derive from EventBase<EventType>.\n"
		);
	}

	template<typename EventType>
	EventID_t EventBase<EventType>::getID() noexcept
	{
		static_assert(
			std::is_base_of_v<EventBase<EventType>, EventType>,
			"Template paramter EventType must derive from EventBase<EventType>.\n"
		);
		return s_ID;
	}

	template<typename EventType>
	EventID_t EventBase<EventType>::doGetDerivedID() const noexcept
	{
		return getID();
	}

}