#pragma once
#include <memory>
#include <vector>
#include <queue>
#include <array>
#include <unordered_map>
#include "PrimitiveTypes.h"

namespace evt
{
	class ListenerInterface;
	/*
		The Dispatcher class makes it easy to dispatch an event to many listeners.
		A listener can subscribe to a Dispatcher using the subscribe() member function.
		A subscribed listener can unsubscribe using the unsubscribe() function. 
		An event can be dispatched to relevant listener using the dispatch() function.
		An event can be pushed onto the internal event queue using the push() function. 
		The event queue can be cleared using the update() function. 
	*/
	class Dispatcher final
	{
	public:
		/*
			Public implementation types
		*/
		using ListenerShrPtr = std::shared_ptr<class ListenerInterface>;
		using EvtBaseShrPtr = std::shared_ptr<class EventInterface>;

		/*
			Subscribe and reacive events from this dispatcher.
			Returns the ID of the event type and listener respectively.
		*/
		std::pair<EventID_t, ListenerID_t> subscribe(ListenerShrPtr listener) noexcept;

		/*
			Unsubscribe and stop recieving events from this dispatcher.
		*/
		void unsubscribe(ListenerShrPtr listener) noexcept;
		void unsubscribe(EventID_t evtID, ListenerID_t listID) noexcept;


		/*
			Immediately dispatch an event.
		*/
		void dispatch(EvtBaseShrPtr);

		/*
			Push an event onto the queue to be dispatched on the next call to update. 
		*/
		void push(EvtBaseShrPtr);

		/*
			Dispatch all events in internal event queue		
		*/
		void update();


	private:
		/*
			Implementation types 
		*/
		using ListenerVec = std::vector<ListenerShrPtr>;
		using ListIterator = ListenerVec::iterator;
		using ListConstIterator = ListenerVec::const_iterator;
		using ListenerMap = std::unordered_map<EventID_t, ListenerVec>;
		using EvtQueue = std::queue<EvtBaseShrPtr>;

		/*
			Implementation function
		*/
		void removeListener(ListenerVec& vec, ListenerID_t listener);

		/*
			Map of listeners
		*/
		ListenerMap m_listMap;

		/*
			Index of the active event queue
		*/
		size_t m_actIdx{ 0 };

		/*
			Event queues
		*/
		std::array<EvtQueue, 2> m_evtQueues;
	};
}

