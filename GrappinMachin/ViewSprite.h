#pragma once
#include "Controller.h"
#include "SFML\Graphics.hpp"
#include <vector>





class GrabSprite : Controller, public sf::Drawable, public sf::Transformable
{
public:
	GrabSprite(EventManager *evManager, std::string path);
	virtual ~GrabSprite();

	virtual void notify(Events *events);

	//Le grappin doit etre visible
	bool isVisible();

	//Affichage du grappin
	void loadGrab(sf::Vector2f newPos);

private:
	//Draw grappin
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	

	//Texture de rendue du grappin
	sf::RenderTexture m_text;

	//Chemin d'accès de l'image du  grappin
	std::string m_path;

	//Le grappin est visible
	bool m_visible;


};





class PlayerSprite : Controller, public sf::Drawable, public sf::Transformable
{
public:
	PlayerSprite(EventManager *evManager);
	virtual ~PlayerSprite();

	virtual void notify(Events *events);

private:
	//Draw player
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//load player Sprite 
	void load(std::string path);

	sf::Sprite m_player;
	sf::Texture m_textPlayer;
};








class Sector : public sf::Drawable, public sf::Transformable
{
public:

	Sector(int num, std::vector<int> neighbor, sf::Vector2f pos, sf::Color color);

	~Sector();

	//get all neighbor
	 std::vector<int> getNeighbor();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:

	//Number of current sector
	int m_num;

	//Pos of the sector
	sf::Vector2f m_pos;

	//Texture rendered
	sf::RenderTexture m_sector;

	//Num of his neighbor
	std::vector<int> m_neighbor;
};

class MapSprite : public Controller, public sf::Drawable, public sf::Transformable
{
public:
	MapSprite(EventManager *evManager);
	virtual ~MapSprite();

	virtual void notify(Events *events);

	//Renvoie un secteur selon son numéro
	Sector * getSector(unsigned int num);

private:
	//Draw Map
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	//Load map
	void load(std::string map);

	//Move map (when player move)
	void moveP(sf::Vector2f direction);

	//Edit rendered sectors
	void updateSector();



	sf::RenderTexture m_map;


	//current sector of the player
	int m_sector;

	//Collections of sectors
	std::vector<Sector*> m_children;
};




class ViewController : Controller
{
public:
	ViewController(EventManager *evManager);
	virtual ~ViewController();


	//Renvoie la map
	MapSprite *getMap();

	virtual void notify(Events *events);

	//Lui même
	static ViewController* pview;
private:
	//Map actuelle
	MapSprite m_map;

	//Joueur
	PlayerSprite m_player;

	//Grappin du joueur (toujours existant, juste invisible si le grappin n'est pas lancée
	GrabSprite m_grab;


	sf::Font m_basicFont;

	//FPS stats
	sf::Clock m_clock;
	double m_framerate;
	sf::Time m_frameTime;
	sf::Text m_fps;
};

