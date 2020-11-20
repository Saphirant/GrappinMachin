#include "Events.h"
#include <iostream>

//Basic Event
Events::Events()
{
	m_name = "Generic Event";

	m_code = GENERIC_EVENT;
}

Events::~Events()
{
}

int Events::getEventType()
{
	return m_code;
}

std::string Events::getEventName()
{
	return m_name;
}


//Physics time init
double PhysicsTime::tpm = 0;


//Init des éléments récursifs
std::vector<Recursive*> Recursive::recur = {};



Recursive::Recursive(int name, sf::Vector2f move, sf::Vector2f add, sf::Vector2f finaleValue, bool sup) : m_typeMove(name), m_move(move), m_add(add), m_finaleValue(finaleValue), m_sup(sup)
{
	m_name = "Generic Recursive Events";
	m_code = GENERIC_EVENT;
}

Recursive::~Recursive()
{
}

bool Recursive::exist(int type)
{
	for (Recursive * r : recur)
	{
		if (r->m_typeMove == type)
		{
			return true;
		}
	}
	return false;
}

sf::Vector2f Recursive::getCurrentMove()
{

	return sf::Vector2f(PhysicsTime::tpm * m_move.x, PhysicsTime::tpm * m_move.y);
}

bool Recursive::nextStep()
{

	//On ajoute le pas, que l'on recalcule selon les fps actuels
	//m_move = sf::Vector2f(m_move.x + m_add.x, m_move.y + m_add.y);
	m_move += m_add;
	//On vérifie que ca ne dépasse pas la valeur max (ou min)
	if (m_move.x <= m_finaleValue.x && m_move.y <= m_finaleValue.y)
	{
		return !m_sup;
	}
	else
	{
		return m_sup;
	}
	
}





//Tick Event
TickEvent::TickEvent(double currentFps)
{
	m_name = "CPU Tick Event";

	m_code = TICK_EVENT;

	PhysicsTime::tpm = 60 / currentFps;
}

TickEvent::~TickEvent()
{
}





//Quit Event
QuitEvent::QuitEvent()
{
	m_name = "Program Quit Event";

	m_code = QUIT_EVENT;
}

QuitEvent::~QuitEvent()
{
}






//Game started
GameStartedEvent::GameStartedEvent(GameController *game): m_game(game)
{
	m_name = "Game Started Event";

	m_code = GAME_STARTED_EVENT;


}

GameStartedEvent::~GameStartedEvent()
{
}






PlayerMoveRequest::PlayerMoveRequest(int name,sf::Vector2f move, sf::Vector2f add, sf::Vector2f finaleValue, bool sup) : Recursive(name, move, add, finaleValue, sup) 
{
	recur.push_back(this);

	m_name = "Player Move Request Event";

	m_code = PLAYER_MOVE_REQUEST_EVENT;
}

PlayerMoveRequest::PlayerMoveRequest(sf::Vector2f move) : Recursive(Recursive::NONE, move, sf::Vector2f(0, 0), sf::Vector2f(0, 0), true)
{
	m_name = "Player Move Request Event";

	m_code = PLAYER_MOVE_REQUEST_EVENT;
}


PlayerMoveRequest::~PlayerMoveRequest()
{
}





PlayerMoveEvent::PlayerMoveEvent(sf::Vector2f direction) : m_direction(direction)
{
	m_name = "Player Move Event";

	m_code = PLAYER_MOVE_EVENT;
}

PlayerMoveEvent::~PlayerMoveEvent()
{
}

sf::Vector2f PlayerMoveEvent::getDirection()
{
	return m_direction;
}






BuildMapEvent::BuildMapEvent(std::string map) : m_map(map)
{
	m_name = "Build Map Event";

	m_code = BUILD_MAP_EVENT;
}

BuildMapEvent::~BuildMapEvent()
{
}

std::string BuildMapEvent::getMap()
{
	return m_map;
}






CreatePlayerEvent::CreatePlayerEvent(std::string path, sf::Vector2f startPos) : m_path(path), m_startPos(startPos)
{
	m_name = "Create Player Event";

	m_code = CREATE_PLAYER_EVENT;
}

CreatePlayerEvent::~CreatePlayerEvent()
{
}

std::string CreatePlayerEvent::getPath()
{
	return m_path;
}

sf::Vector2f CreatePlayerEvent::getStartPos()
{
	return m_startPos;
}






ChangeSectorEvent::ChangeSectorEvent(int newSector) : m_newSector(newSector)
{
	m_name = "Change Sector Event";

	m_code = CHANGE_SECTOR_EVENT;
}

ChangeSectorEvent::~ChangeSectorEvent()
{
}

int ChangeSectorEvent::getNewSector()
{
	return m_newSector;
}







ResizeEvent::ResizeEvent(sf::Event::SizeEvent newSize, sf::Vector2f oldSize) : m_newSize(newSize), m_oldSize(oldSize)
{
	m_name = "Resize Event";

	m_code = RESIZE_EVENT;
}

ResizeEvent::~ResizeEvent()
{
}

sf::Event::SizeEvent ResizeEvent::getSize()
{
	return m_newSize;
}

sf::Vector2f ResizeEvent::getDifference()
{
	return sf::Vector2f(m_newSize.width - m_oldSize.x, m_newSize.height - m_oldSize.y);
}







CreateGrabEvent::CreateGrabEvent(sf::Vector2f mousePos, sf::Vector2f playerPos): m_mousePos(mousePos), m_playerPos(playerPos)
{
	m_name = "Create Grab Event";

	m_code = CREATE_GRAB_EVENT;
}

CreateGrabEvent::~CreateGrabEvent()
{
}

sf::Vector2f CreateGrabEvent::getPos()
{
	return m_mousePos;
}

sf::Vector2f CreateGrabEvent::getPlayerPos()
{
	return m_playerPos;
}






DeleteGrabEvent::DeleteGrabEvent()
{
	m_name = "Delete Grab Event";

	m_code = DELETE_GRAB_EVENT;
}

DeleteGrabEvent::~DeleteGrabEvent()
{
}

MoveGrabEvent::MoveGrabEvent(int name, sf::Vector2f move, sf::Vector2f add, sf::Vector2f finaleValue, bool sup): Recursive(name, move, add, finaleValue, sup)
{
	recur.push_back(this);

	m_name = "Grab Move Event";

	m_code = MOVE_GRAB_EVENT;
}

MoveGrabEvent::~MoveGrabEvent()
{
}
