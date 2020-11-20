#pragma once
#include "EventManager.h"
#include "Events.h"
#include "SFML\Graphics.hpp"




class Controller
{
public:
	Controller(EventManager *evManager);
	virtual ~Controller();

	virtual void notify(Events *events) = 0;
protected:
	EventManager *m_evManager;
	static sf::RenderWindow *window;
};

//S'occupe de gérer les entrées clavier et les événements SFML
class KeyboardController : Controller
{
public:
	KeyboardController(EventManager *evManager);
	virtual ~KeyboardController();

	virtual void notify(Events *events);
	

};


//S'occupe de générer les tick event
class CPUSpinnerController : Controller
{
public:
	CPUSpinnerController(EventManager *evManager);
	virtual ~CPUSpinnerController();

	virtual void notify(Events *events);

	virtual void run();

protected:
	bool m_keepGoing;
};


