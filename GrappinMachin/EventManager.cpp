#include "EventManager.h"
#include <string>
#include <algorithm>



EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}


void EventManager::registerListener(Controller *listener)
{
	listeners.push_back(listener);
}


void EventManager::unregisterListener(Controller *listener)
{

	auto finded = std::find(listeners.begin(), listeners.end(), listener);
	if ( finded != listeners.end())
	{
		delete listeners[finded - listeners.begin()];
		listeners.erase(finded);
	}
}



