#pragma once
#include "PrimitiveTypes.h"
#include <memory>

namespace evt
{
	/*
		Abstract interface for listener classes
	*/
	class ListenerInterface
	{
	public:
		/*
			Public member types
		*/
		using EvtBaseShrPtr = std::shared_ptr<class EventInterface>;

		/*
			Construction and copy control
		*/
		ListenerInterface() noexcept;
		ListenerInterface(const ListenerInterface&) = default;
		ListenerInterface(ListenerInterface&&) = default;
		ListenerInterface& operator=(const ListenerInterface&) = default;
		ListenerInterface& operator=(ListenerInterface&&) = default;
		virtual ~ListenerInterface() = default;

		/*
			Get the ID of the event that this listener reacts to.
		*/
		EventID_t getDerievedEventID() const noexcept;

		/*
			Get the ID of this Listener
		*/
		ListenerID_t getListenerID() const noexcept;

		/*
			Notify this listener of an event. 
			The listener will not react if the wront event type is given.
		*/
		void notify(EvtBaseShrPtr evt);

	private:
		/*
			ID counter for Listener
		*/
		inline static ListenerID_t s_listIDCounter{ 0 };

		/*
			Generate an ID
		*/
		static ListenerID_t genID() noexcept;

		/*
			Virtual function called by notify.
			This function should cast the pointer to the underlying event type.
		*/
		virtual void doNotify(EvtBaseShrPtr evt) = 0;

		/*
			Virtual function called by getDerivedEventID()
		*/
		virtual EventID_t doGetDerievedEventID() const noexcept = 0;
			
		/*
			The ID of this listener
		*/
		const ListenerID_t m_ID;
	};
}