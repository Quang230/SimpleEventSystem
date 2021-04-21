#include <assert.h>
#include <include/EventInterface.h>

namespace evt
{
	EventID_t EventInterface::getDerivedID() const noexcept
	{
		auto ID = doGetDerivedID();
		assert(ID != static_cast<EventID_t>(0));
		return ID;
	}
}