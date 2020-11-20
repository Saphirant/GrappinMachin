#include "Game.h"
#include <iostream>



GameController::GameController(EventManager *evManager): Controller(evManager), m_state(GameState::Starting), m_player(evManager)
{
	m_state = GameState::Starting;
	m_evManager->post(new BuildMapEvent("Ressources/Map"));
	


}

GameController::~GameController()
{
}

void GameController::start()
{
	m_state = GameState::Running;
	GameStartedEvent *ev = new GameStartedEvent(this);
	m_evManager->post<GameStartedEvent>(ev);
}

void GameController::notify(Events *events)
{
	if (events->getEventType() == Events::TICK_EVENT)
	{
		if (m_state == GameState::Starting)
		{
			start();
		}
	}
}