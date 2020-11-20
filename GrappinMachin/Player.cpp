#include "Player.h"
#include <iostream>


Player::Player(EventManager *evManager): Controller(evManager), m_pos(sf::Vector2f(0,0)), m_size(sf::Vector2f(50, 100)), m_numSector(0)
{

	
}


Player::~Player()
{
}

void Player::notify(Events *events)
{

	if (events->getEventType() == Events::CREATE_PLAYER_EVENT) //Création du joueur
	{
		//Récupération de la position de départ
		CreatePlayerEvent *ev = (CreatePlayerEvent*)events;
		sf::Vector2f startPos = ev->getStartPos();

		sf::Vector2f screenSize(window->getSize());

		m_pos = sf::Vector2f((screenSize.x / 2) - (m_size.x / 2) + startPos.x, screenSize.y / 2 + startPos.y);
	}
	else if (events->getEventType() == Events::PLAYER_MOVE_REQUEST_EVENT)
	{
		//TODO Check if direction is ok

		PlayerMoveRequest *ev = (PlayerMoveRequest*)events;

		//Check if player change sector
		Sector *sect = ViewController::pview->getMap()->getSector(m_numSector);
		sf::FloatRect sectorBox(sect->getPosition(), sf::Vector2f(1000, 1000));
		
		//Position du personnage
		sf::Vector2f playerPoint(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2);
		//std::cout << "	|	 Player: " << playerPoint.x << "/" << playerPoint.y << std::endl;
;
		//std::cout << "Sector: " << sectorBox.top << "/" << sectorBox.left << "," << sectorBox.width << "x" << sectorBox.height <<"	|	 Player: " << playerPoint.x << "/" << playerPoint.y << std::endl;

		if (!sectorBox.contains(playerPoint))	//Si le joueur change de secteur
		{
			std::vector<int> neighbor = sect->getNeighbor();
			for (int n : neighbor) //On parcourt tous les secteurs voisins pour repositionne rle joueur
			{
			
				Sector *currsect = ViewController::pview->getMap()->getSector(n);
				if (currsect != nullptr)
				{
					sf::FloatRect currsectorBox(currsect->getPosition(), sf::Vector2f(1000, 1000));

					//std::cout << "Sector: " << currsectorBox.top << "/" << currsectorBox.left << "," << currsectorBox.width << "x" << currsectorBox.height <<"	|	 Player: " << playerPoint.x << "/" << playerPoint.y << std::endl;


					if (currsectorBox.contains(playerPoint))
					{
						m_numSector = n;
						ChangeSectorEvent *sectorEvent = new ChangeSectorEvent(m_numSector);
						m_evManager->post(sectorEvent);
						break;
					}
				}
			}

			
		}
		//Move player and SpritePlayer
		PlayerMoveEvent *moveEvent = new PlayerMoveEvent(ev->getCurrentMove());
		m_evManager->post(moveEvent); 
	}

	else if (events->getEventType() == Events::PLAYER_MOVE_EVENT)
	{
		//Move player
		PlayerMoveEvent *ev = (PlayerMoveEvent*)events;
		move(ev->getDirection());
	}

}

//Déplace le joueur
void Player::move(sf::Vector2f direction)
{
	m_pos += sf::Vector2f(-direction.x, -direction.y);
}










Grab::Grab(EventManager * evManager, int sector, sf::Vector2f pos, sf::Vector2f destPos) : Controller(evManager), m_sector(sector), m_pos(pos), m_destPos(destPos), m_speed(sf::Vector2f(100,100))
{
	//tester le trajet du grappin, et le coller si il touche quelque chose
	if (!testGrab())
	{
		//On lance le mouvement si le grappin ne touche rien
		sf::Vector2f add = m_destPos - m_pos;
		add = sf::Vector2f(add.x / m_speed.x, add.y / m_speed.y);
		MoveGrabEvent *ev = new MoveGrabEvent(Recursive::GRAB, add, add, m_destPos, true);
		m_evManager->post(ev);
	}
	
}

Grab::~Grab()
{
}

void Grab::notify(Events * events)
{
	if (events->getEventType() == Events::DELETE_GRAB_EVENT)
	{
	}
}

bool Grab::testGrab()
{
	return false;
}
