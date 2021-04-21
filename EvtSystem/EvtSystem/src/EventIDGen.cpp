#include <assert.h>
#include <limits>
#include <include/EventIDGen.h>

namespace evt
{
	EventID_t EventIDGen::genID() noexcept
	{
		assert(std::numeric_limits<EventID_t>::max() != s_counter);
		return ++s_counter;
	}
}