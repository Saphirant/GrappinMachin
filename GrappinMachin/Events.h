#pragma once
#include <iostream>
#include "SFML\Graphics.hpp"

class GameController;

class Events
{
public:
	Events();
	virtual ~Events();

	virtual int getEventType();

	virtual std::string getEventName();

	enum { GENERIC_EVENT, TICK_EVENT, QUIT_EVENT, GAME_STARTED_EVENT , PLAYER_MOVE_REQUEST_EVENT, PLAYER_MOVE_EVENT, BUILD_MAP_EVENT, CREATE_PLAYER_EVENT, 
		CHANGE_SECTOR_EVENT, RESIZE_EVENT, CREATE_GRAB_EVENT, DELETE_GRAB_EVENT, MOVE_GRAB_EVENT};

protected:
	std::string m_name;
	int m_code;
};

//S'occupe de la physique du jeu (Nombre de déplacement par secondes, selon le framerate
class PhysicsTime
{
protected:
	static double tpm;
};


//S'occupe des évènements récursifs
class Recursive : PhysicsTime, public Events
{
public:
	//fonction statique, renvoie si un event recursif du type demandée existe déjà
	static bool exist(int type);


	//Récursif avec un nom, un premier mouvement, un pas par tick et une valeur finale et si il faut que la valeur soit supérieur
	Recursive(int name, sf::Vector2f move, sf::Vector2f add, sf::Vector2f finaleValue, bool sup);

	virtual ~Recursive();

	//Mouvement recursif possible
	enum {NONE, JUMP, GRAB};


	//Renvoie le mouvement pour le tick actuelle
	sf::Vector2f getCurrentMove();

	//Calcul le mouvement suivant (si le mouvement est attendu)
	bool nextStep();



	//Variable reprenant tous les évènements récursifs
	static std::vector<Recursive *> recur;

protected:
	//valeur ajoutée a chaque tick
	sf::Vector2f m_add;
	//Destination du mouvement
	sf::Vector2f m_dest;

	//Premier mouvement fait
	sf::Vector2f m_move;
	//Valeur du dernier mouvement
	sf::Vector2f m_finaleValue;
	//La valeur doit être suppérieur
	bool m_sup;
	//Nom du mouvement (pour ne pas le répéter) dans une enum
	int m_typeMove;

};


class TickEvent : public Events, PhysicsTime
{
public:
	TickEvent(double currentFps);
	virtual ~TickEvent();

};

class QuitEvent : public Events
{
public:
	QuitEvent();
	virtual ~QuitEvent();

};

class GameStartedEvent : public Events
{
public:
	GameStartedEvent(GameController *game);
	virtual ~GameStartedEvent();


private:
	GameController *m_game;
};

class PlayerMoveRequest : public Recursive
{
public:
	//le nom, premier mouvement, un pas par tick et une valeur finale et si il faut que la valeur soit supérieur
	PlayerMoveRequest(int name, sf::Vector2f move, sf::Vector2f add, sf::Vector2f finaleValue, bool sup);
	//Movement simple
	PlayerMoveRequest(sf::Vector2f move);
	virtual ~PlayerMoveRequest();
};


class PlayerMoveEvent : public Events
{
public:
	PlayerMoveEvent(sf::Vector2f direction);
	virtual ~PlayerMoveEvent();

	sf::Vector2f getDirection();

private:
	sf::Vector2f m_direction;

};

class BuildMapEvent : public Events
{
public:
	BuildMapEvent(std::string map);
	virtual ~BuildMapEvent();

	std::string getMap();

private:
	std::string m_map;
};


//Create Sprite of player and set position of real player
class CreatePlayerEvent : public Events
{
public:
	CreatePlayerEvent(std::string path, sf::Vector2f startPos);
	virtual ~CreatePlayerEvent();

	std::string getPath();
	sf::Vector2f getStartPos();

private:
	std::string m_path;
	sf::Vector2f m_startPos;
};


class ChangeSectorEvent : public Events
{
public:
	ChangeSectorEvent(int newSector);
	virtual ~ChangeSectorEvent();

	int getNewSector();
private:
	int m_newSector;
};


class ResizeEvent : public Events
{
public:
	ResizeEvent(sf::Event::SizeEvent newSize, sf::Vector2f oldSize);
	virtual ~ResizeEvent();

	sf::Event::SizeEvent getSize();
	sf::Vector2f getDifference();

private:
	sf::Event::SizeEvent m_newSize;
	sf::Vector2f m_oldSize;

};

class CreateGrabEvent : public Events
{
public:
	CreateGrabEvent(sf::Vector2f mousePos, sf::Vector2f playerPos);
	virtual ~CreateGrabEvent();

	sf::Vector2f getPos();

	sf::Vector2f getPlayerPos();
private:
	sf::Vector2f m_mousePos;
	sf::Vector2f m_playerPos;

};

class DeleteGrabEvent : public Events
{
public:
	DeleteGrabEvent();
	virtual ~DeleteGrabEvent();

private:

};


class MoveGrabEvent : public Recursive
{
public:
	//le nom, premier mouvement, un pas par tick et une valeur finale et si il faut que la valeur soit supérieur
	MoveGrabEvent(int name, sf::Vector2f move, sf::Vector2f add, sf::Vector2f finaleValue, bool sup);
	virtual ~MoveGrabEvent();

private:

};