# Simple Event System
Simple Event System is a simple event based communication system to be used in a future engine. The system is not thread safe, and should be used only in single threaded applications. Furthermore, the system requires c++17.

The system is based on three components.

* Events
* Listeners
* Dispatcher

The following examples assume the following using directive have been used and the header evtSystem.h is included.

```cpp
#include <evtSystem.h>
using namespace evt;
```

## Events
Events carry information to listeners.  In this system, an event is any class that publicly derive directly from the template class EventBase as follows
```cpp
class DemoEvent : public EventBase<DemoEvent>
	{
	public:
		int getData() const noexcept { return m_data; }

	private:
		int m_data{ 42 };
};
```
The template parameter EventBase must be the same type as the deriving class. This allows the compiler to generate a unique ID to each event type. 
This unique ID can be accessed through the inherited static member function getID().

``` cpp
DemoEvent::getID();
```

The abstract interface class EventInterface can be used as an uniform interface for all event types. 

```cpp
std::shared_ptr<EventInterface> newEvt = std::make_shared<DemoEvent>();
auto ID = newEvt->getDerivedID();
```

Here the ID of the underlying event type is accessed through the function getDerivedID().

## Listeners

Listeners define reactions to events. A listener is any class that publicly derive from the template class ListenerBase and override the virtual react member function as follows.

```cpp
class DemoListener : public ListenerBase<DemoEvent>
{
    void react(EvtShrPtr evtPtr) override 
    {
		std::cout << evtPtr->getData() << "\n";        
    }
};
```

The ListenerBase template class is declared as follows

```cpp
template<typename EventType>
class ListenerBase : public ListenerInterface;
```

Any listener can only react to a single type of events. This type of events is defined as the template parameter EventType. This allows the compiler to cast an abstract interface EventInterface pointer to an pointer to EventType. The type alias EvtShrPtr is a shared pointer to EventType.

To notify a listener to an event, use the inherited notify member function.

``` cpp
auto listener = DemoListener{};
auto newEvt = std::make_shared<DemoEvent>();
listener.notify(newEvt);
```

The notify member function takes a shared pointer to an EventInterface, hence any event can be given. The notify member function checks if the underlying type of the given event and EventType match. If this is the case, then the given pointer is casted to an shared pointer to EventType and react is called. If this is not the case, then nothing happens. 

Each instance of an listener has a unique ID. This ID can be accessed through the member function getListenerID().

``` cpp
auto listener = DemoListener{};
auto ID = listener.getListenerID();
```

The abstract interface class ListenerInterface can be used as a uniform interface for all listener types.

``` cpp
std::shared_ptr<ListenerInterface> listPtr = std::make_shared<DemoListener>();
auto evtID = listPtr->getDerivedEventID();
```

Here the member function getDerivedEventID is used to get the ID of the template parameter EventType.

## Dispatcher

Dispatcher is the tool which decouples classes that dispatch events and classes that listens for events using listeners. A Dispatcher can only be default constructed, and hence is easy to create.

```cpp
auto disp = Dispatcher{};
```

A listener can subscribe to an dispatcher using the subscribe function.

```cpp
auto listPtr = std::make_shared<DemoListener>();
disp.subscribe(listPtr);
```

When an listener that reacts to events of type EventType is subscribed to an dispatcher, it will be notified when any event of type EventType is dispatched using the dispatcher. Note that an listener can be subscribed to multiple dispatchers. To dispatch an event use the dispatch member function.

```cpp
auto evtPtr = std::make_shared<DemoListener>();
disp.dispatch(evtPtr);
```

The dispatch member function takes a shared pointer to EventInterface, an as such, can take any event type. Using the dispatch function, any relevant listener will be notified immediately. If this is not desirable, then one can push the event onto the internal event queue using the push member function.

```cpp
auto evtPtr = std::make_shared<DemoListener>();
disp.push(evtPtr);
// Some other code
// ...
disp.update();
```

 Here the queue is cleared and dispatch is called on any event in the queue using the update() member function. New events are allowed to be pushed while the update function is notifying listeners of events. 

To unsubscribe an listener from a dispatcher use the unsubscribe member function.

```cpp
auto listPtr = std::make_shared<DemoListener>();
disp.subscribe(listPtr);
// Some other code
// ....
disp.unsubscribe(listPtr);
```

After the call to unsubscribe, the dispatcher gives up it's shared ownership over the listener.

