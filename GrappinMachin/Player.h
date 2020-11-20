#pragma once
#include "ViewSprite.h"

//Classe du joueur
class Player : Controller
{
public:
	
	Player(EventManager *evManager);
	virtual ~Player();

	//Héritage: Gestion des événements
	virtual void notify(Events *events);

private:

	//Move player
	void move(sf::Vector2f direction);

	//Sector actuel du joueur
	int m_numSector;

	//Taille et position du joueur
	sf::Vector2f m_pos, m_size;

};


//Classe du grappin du jouer
class Grab : Controller
{
public:
	//Secteur actuel du grappin et sa position (= celle du joueur) + position de destination
	Grab(EventManager *evManager, int sector, sf::Vector2f pos, sf::Vector2f destPos);
	virtual ~Grab();


	//Héritage: Gestion des événements
	virtual void notify(Events *events);
private:
	//Position du grappin
	sf::Vector2f m_pos;

	//Destination du grappin
	sf::Vector2f m_destPos;

	//Vitesse du grappin (valeur définie)
	sf::Vector2f m_speed;

	//Position du grappin dans les secteurs
	int m_sector;

	//tester le trajet du grappin, et le coller si il touche quelque chose
	bool testGrab();

};