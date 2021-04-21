#pragma once
#include "ListenerInterface.h"
#include "EventInterface.h"
#include "EventBase.h"
#include <assert.h>
#include <memory>

namespace evt
{
	/*
		All Listener classes reacting to events of type EventType must derive publicly from ListenerBase<EventType>.
		The listener class inherits two important functions from ListenerBase<EventType>. The first, notify(EvtBaseShrPtr), allows the listener
		to be alerted to a new event. Second, react(EvtShrPtr), allows the listener to react to an event of type EventType.
		notify(EvtBaseShrPtr) will only call react(EvtShrPtr) if the underlying type of EvtBaseShrPtr is EventType.
	*/
	template<typename EventType>
	class ListenerBase : public ListenerInterface
	{
	public:
		/*
			std::shared_ptr to the type of events that this listener reacts to. 
		*/
		using EvtShrPtr = std::shared_ptr<EventType>;

		/*
			Construction
		*/
		ListenerBase() noexcept;

	private:
		/*
			Virtual function when an event of type EventType is registered
		*/
		virtual void react(EvtShrPtr evt) = 0;

		/*
			Overide of the doNotify function. 
			This function converts the pointer to the correct type.
		*/
		void doNotify(EvtBaseShrPtr evt) override final;

		/*
			Overide the doGetDerievedEventID().
			This function returns the event ID of the EventType.
		*/
		EventID_t doGetDerievedEventID() const noexcept override final;
	};

	template<typename EventType> 
	ListenerBase<EventType>::ListenerBase() noexcept : 
		ListenerInterface{}
	{
		static_assert(
			std::is_base_of_v<EventBase<EventType>, EventType>,
			"Template paramter EventType must derive from EventBase<EventType>.\n"
		);
	}

	template<typename EventType> 
	void ListenerBase<EventType>::doNotify(EvtBaseShrPtr evt)
	{
		/*
			Assert that the pointer is indeed the correct type.
		*/
		assert(evt->getDerivedID() == EventType::getID());
		assert(std::dynamic_pointer_cast<EventType>(evt) != nullptr);
		react(std::static_pointer_cast<EventType>(evt));
	}

	template<typename EventType>
	EventID_t ListenerBase<EventType>::doGetDerievedEventID() const noexcept
	{
		return EventType::getID();
	}

}