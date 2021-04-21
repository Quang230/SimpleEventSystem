#include <assert.h>
#include <utility>
#include <include/ListenerInterface.h>
#include <include/EventInterface.h>
#include <include/EventInterface.h>
#include <include/Dispatcher.h>

namespace evt
{
	std::pair<EventID_t, ListenerID_t> Dispatcher::subscribe(ListenerShrPtr listener) noexcept
	{
		// Assert that this listener is not already subscribed
		assert(
			[&](){
				auto pressent = false;
				if (auto iter = m_listMap.find(listener->getDerievedEventID()); iter != m_listMap.end())
				{
					for (auto& elm : iter->second)
					{
						if (elm->getListenerID() == listener->getListenerID())
						{
							pressent = true;
						}
					}
				}
				return !pressent;
			}()
		);
		auto evtID = listener->getDerievedEventID();
		auto listID = listener->getListenerID();

		// Insert the listener
		m_listMap[evtID].push_back(listener);
		return { evtID, listID };
	}

	void Dispatcher::unsubscribe(ListenerShrPtr listener) noexcept
	{
		assert(listener != nullptr);
		if (auto iter = m_listMap.find(listener->getDerievedEventID()); iter != m_listMap.end())
		{
			removeListener(iter->second, listener->getListenerID());
		}
	}

	void Dispatcher::unsubscribe(EventID_t evtID, ListenerID_t listID) noexcept
	{
		assert(evtID != 0);
		assert(listID != 0);
		if (auto iter = m_listMap.find(evtID); iter != m_listMap.end())
		{
			removeListener(iter->second, listID);
		}
	}

	void Dispatcher::removeListener(ListenerVec& vec, ListenerID_t listID)
	{
		assert(listID != 0);
		for (auto iterVec = vec.begin(); iterVec != vec.end(); ++iterVec)
		{
			if ((*iterVec)->getListenerID() == listID)
			{
				std::iter_swap(iterVec, vec.end() - 1);
				vec.pop_back();
				break;
			}
		}
	}

	void Dispatcher::dispatch(EvtBaseShrPtr evtPtr)
	{
		assert(evtPtr != nullptr);
		if (auto iter = m_listMap.find(evtPtr->getDerivedID()); iter != m_listMap.end())
		{
			for (auto& listPtr : iter->second)
			{
				listPtr->notify(evtPtr);
			}
		}
	}

	void Dispatcher::push(EvtBaseShrPtr evtPtr)
	{
		assert(evtPtr != nullptr);
		m_evtQueues[m_actIdx].push(evtPtr);
	}

	void Dispatcher::update()
	{
		// Determine the queue to empty and increment the active queue
		auto& currQueue = m_evtQueues[m_actIdx];
		m_actIdx = (m_actIdx + 1) % m_evtQueues.size();

		// Empty the queue
		while (!currQueue.empty())
		{
			auto evt = currQueue.front();
			currQueue.pop();
			dispatch(evt);
		}
	}

}