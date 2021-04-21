#include <assert.h>
#include <limits>
#include <include/EventInterface.h>
#include <include/ListenerInterface.h>

namespace evt
{
	ListenerInterface::ListenerInterface() noexcept : 
		m_ID{genID()} {}

	EventID_t ListenerInterface::getDerievedEventID() const noexcept
	{
		auto ID = doGetDerievedEventID();
		assert(ID != static_cast<EventID_t>(0));
		return ID;
	}
	
	ListenerID_t ListenerInterface::getListenerID() const noexcept
	{
		assert(m_ID != static_cast<ListenerID_t>(0));
		return m_ID;
	}

	ListenerID_t ListenerInterface::genID() noexcept
	{
		assert(std::numeric_limits<ListenerID_t>::max() != s_listIDCounter);
		return ++s_listIDCounter;
	}

	void ListenerInterface::notify(EvtBaseShrPtr evt)
	{
		if (evt->getDerivedID() == getDerievedEventID())
		{
			doNotify(evt);
		}
	}

}