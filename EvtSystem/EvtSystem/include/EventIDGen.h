#pragma once
#include "PrimitiveTypes.h"

namespace evt
{
	/*
		EventIDGen class used by EventBase<EventType> template class to generated a unique ID for each event type.
	*/
	class EventIDGen final
	{	
		template<typename EventType>
		friend class EventBase;
	
	public:
		/*
			EventIDGen is not constructable
		*/
		EventIDGen() = delete;

	private:
		/*
			ID counter.
		*/
		inline static EventID_t s_counter{0};

		/*
			Function used by EventBase template class to generate ID
		*/
		static EventID_t genID() noexcept;
	};
}