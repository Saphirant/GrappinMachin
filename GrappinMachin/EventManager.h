#pragma once
#include <vector>
#include <memory>


class Controller;

class EventManager
{
public:
	EventManager();
	~EventManager();


	void registerListener(Controller *listener);

	void unregisterListener(Controller *listener);

	template<typename T>
	void post(T *events);

private:
	 std::vector<Controller*> listeners;


};

template<typename T>
inline void EventManager::post(T *events)
{
	if (events->getEventType() != Events::TICK_EVENT && events->getEventType() != Events::PLAYER_MOVE_REQUEST_EVENT && events->getEventType() != Events::PLAYER_MOVE_EVENT && events->getEventType() != Events::CHANGE_SECTOR_EVENT)
	{
		std::cout << "Event raised : " << events->getEventName() << std::endl;
	}
	for (Controller *listener : listeners)
	{
		listener->notify(events);
	}
}
