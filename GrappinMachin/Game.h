#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"


enum class GameState { Starting, Running, Paused, Stopped };

class GameController : Controller
{
public:
	GameController(EventManager *evManager);
	virtual ~GameController();
	
	void start();

	virtual void notify(Events *events);
protected:
	GameState m_state;
	Player m_player;


};

