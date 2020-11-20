#include "ViewSprite.h"
#include "SFML\Graphics.hpp"
#include <algorithm>





GrabSprite::GrabSprite(EventManager * evManager, std::string path) : Controller(evManager), m_visible(false), m_path(path)
{


	m_text.create(64, 64);

}

GrabSprite::~GrabSprite()
{
}

void GrabSprite::notify(Events * events)
{
	if (events->getEventType() == Events::CREATE_GRAB_EVENT)
	{
		m_visible = true;
		CreateGrabEvent *ev = (CreateGrabEvent*)events;
		//loadGrab(ev->getPos());
	}
	else if (events->getEventType() == Events::DELETE_GRAB_EVENT)
	{
		m_visible = false;
	}
	else if (events->getEventType() == Events::MOVE_GRAB_EVENT)
	{
		MoveGrabEvent *ev = (MoveGrabEvent*)events;
		move(ev->getCurrentMove());
	}
}

void GrabSprite::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	const sf::Texture& text = m_text.getTexture();

	sf::Sprite sprite(text);
	target.draw(sprite, states);
}

void GrabSprite::loadGrab(sf::Vector2f newPos)
{
	sf::Texture text;
	text.loadFromFile(m_path);
	sf::Sprite sprite;
	sprite.setTexture(text);
	std::cout << newPos.x << "/" << newPos.y << std::endl;

	m_text.clear(sf::Color(0, 0, 0, 0));

	m_text.draw(sprite);

	m_text.display();
	setPosition(newPos);
}


bool GrabSprite::isVisible()
{
	return m_visible;
}







PlayerSprite::PlayerSprite(EventManager * evManager) : Controller(evManager)
{
	m_evManager->registerListener(this);


	

}

PlayerSprite::~PlayerSprite()
{
}

void PlayerSprite::notify(Events * events)
{

	if (events->getEventType() == Events::CREATE_PLAYER_EVENT)
	{
		//Move sprite
		CreatePlayerEvent *ev = (CreatePlayerEvent*)events;
		load(ev->getPath());
	}
	else if (events->getEventType() == Events::RESIZE_EVENT)
	{
		ResizeEvent *ev = (ResizeEvent*)events;
		sf::Event::SizeEvent size = ev->getSize();

		setPosition(sf::Vector2f((size.width / 2) - (m_textPlayer.getSize().x / 2), size.height / 2));
	}
}




void PlayerSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// on applique la transformation de l'entité -- on la combine avec celle qui a été passée par l'appelant
	states.transform *= getTransform(); // getTransform() est définie par sf::Transformable

										// on applique la texture
	states.texture = &m_textPlayer;

	// on peut aussi surcharger states.shader ou states.blendMode si nécessaire

	target.draw(m_player, states);
}

//Charge le joueur (texture)
void PlayerSprite::load(std::string path)
{

	m_textPlayer.loadFromFile(path);


	m_player.setTexture(m_textPlayer);


	sf::Vector2f screenSize(window->getSize());

	setPosition(sf::Vector2f((screenSize.x / 2) - (m_textPlayer.getSize().x / 2), screenSize.y / 2));
}











Sector::Sector(int num, std::vector<int> neighbor, sf::Vector2f pos, sf::Color color ) : m_num(num), m_neighbor(neighbor), m_pos(pos)
{

	//Create texture
	m_sector.create(1000, 1000);

	//Set position
	setPosition(pos);

	//Init display
	m_sector.clear(color);
	m_sector.display();
}

Sector::~Sector()
{
}




void Sector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	states.transform *= getTransform();
	const sf::Texture& text = m_sector.getTexture();

	sf::Sprite sprite(text);
	target.draw(sprite, states);
}

//Retourne tous les voisins d'un secteur
std::vector<int> Sector::getNeighbor()
{
	return m_neighbor;
}












MapSprite::MapSprite(EventManager * evManager) : Controller(evManager), m_sector(0)
{
	m_map.create(3000, 3000);

}

MapSprite::~MapSprite()
{
}

void MapSprite::notify(Events * events)
{
	if (events->getEventType() == Events::BUILD_MAP_EVENT)
	{
		//Création de la map
		BuildMapEvent *ev = (BuildMapEvent*)events;
		load(ev->getMap());
	}
	else if (events->getEventType() == Events::RESIZE_EVENT)
	{
		//Changement de taille de la fenêtre
		ResizeEvent *ev = (ResizeEvent*)events;
		

		move(sf::Vector2f(ev->getDifference().x/2, ev->getDifference().y/2));
	}
	else if (events->getEventType() == Events::CHANGE_SECTOR_EVENT)
	{
		//Le joueur change de secteur, il faut donc changer les secteurs affiché
		ChangeSectorEvent *ev = (ChangeSectorEvent*)events;
		m_sector = ev->getNewSector();
		updateSector();
	}
	else if (events->getEventType() == Events::PLAYER_MOVE_EVENT)
	{
		//Déplacement de la map
		PlayerMoveEvent *ev = (PlayerMoveEvent*)events;
		moveP(ev->getDirection());
	}
	

}

//Retourne le secteur numéro num
Sector * MapSprite::getSector(unsigned int num)
{
	if (num < m_children.size() && num != -1)
		return m_children[num];
	else
		return nullptr;
}


//Lecture des fichiers de la map
void MapSprite::load(std::string map)
{
	//Lecture des secteurs
	m_children.push_back(new Sector(0, { 1, 9,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(0,0), sf::Color::Blue));
	m_children.push_back(new Sector(1, { 2, 0,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(1000, 0), sf::Color::Red));
	m_children.push_back(new Sector(2, { 5, 1,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(2000, 0), sf::Color::Green));
	m_children.push_back(new Sector(3, { 6, 4,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(0, 1000), sf::Color::Yellow));
	m_children.push_back(new Sector(4, { 3, 5,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(1000, 1000), sf::Color(255, 127, 0)));
	m_children.push_back(new Sector(5, { 4, 2,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(2000, 1000), sf::Color(169,1,5)));
	m_children.push_back(new Sector(6, { 7, 3,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(0, 2000), sf::Color::Magenta));
	m_children.push_back(new Sector(7, { 8, 6,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(1000, 2000), sf::Color(172,89,89)));
	m_children.push_back(new Sector(8, { 9, 7,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(2000, 2000), sf::Color(164,164,0)));
	m_children.push_back(new Sector(9, { 0, 8,-1,-1,-1,-1,-1,-1 }, sf::Vector2f(1000, 1000), sf::Color(255, 127, 0)));

	updateSector();

	//Lecture de la position de départ du joueur
	sf::Vector2f startPos(0, 0);

	//Création du joueur
	m_evManager->post(new CreatePlayerEvent("Ressources/Player/player.png", startPos));

	setPosition(sf::Vector2f(-startPos.x, -startPos.y));
}

void MapSprite::moveP(sf::Vector2f direction)
{
	move(direction);
}


//Charge les secteurs dans la map
void MapSprite::updateSector()
{
	std::vector<int> neighbor = m_children[m_sector]->getNeighbor();
	m_map.clear(sf::Color::Black);

	//draw all sector
	m_map.draw(*m_children[m_sector]);

	for (std::size_t i = 0; i < 8; ++i)
		if (neighbor[i] != -1)
			m_map.draw(*m_children[neighbor[i]]);

	m_map.display();
}

void MapSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	//On dessinne la map avec tous les secteurs chargés
	const sf::Texture& text = m_map.getTexture();

	sf::Sprite sprite(text);
	target.draw(sprite, states);
}












ViewController * ViewController::pview = nullptr;

ViewController::ViewController(EventManager *evManager) : Controller(evManager), m_player(evManager), m_map(evManager), m_grab(evManager, "Ressources/Grab/default.png")
{
	window = new sf::RenderWindow(sf::VideoMode(1080, 720), "GrappinMachin"/*, sf::Style::Fullscreen*/);

	pview = this;


	m_basicFont.loadFromFile("Ressources/Font/basic.otf");
	m_fps.setFont(m_basicFont);
	m_fps.setString("FPS: 0");
}

ViewController::~ViewController()
{
}

MapSprite *ViewController::getMap()
{
	return &m_map;
}

void ViewController::notify(Events *events)
{
	if (events->getEventType() == Events::RESIZE_EVENT)
	{
		ResizeEvent *ev = (ResizeEvent*)events;
		sf::Event::SizeEvent size = ev->getSize();
		// on met à jour la vue, avec la nouvelle taille de la fenêtre
		sf::FloatRect visibleArea(0, 0, size.width, size.height);
		window->setView(sf::View(visibleArea));
	}
	if (events->getEventType() == Events::TICK_EVENT)
	{
		//FPS STAT---------------------------
		m_frameTime = m_clock.getElapsedTime();
		
		if (m_frameTime.asMilliseconds() > 1000)
		{
			
			m_fps.setString("FPS: " + std::to_string((int)m_framerate));
			m_framerate = 0;
			m_clock.restart();
		}
		else
		{
			m_framerate++;
		}
		//----------------------------------

		//Affichage -------------------------------


		window->clear(sf::Color::Black);

		//Affichage de la amp
		window->draw(m_map);


		//Affichage du grappin
		if (m_grab.isVisible())
			window->draw(m_grab);

		//Affichage du joueur
		window->draw(m_player);

		//Affichage des fps
		window->draw(m_fps);
		window->display();

		//-----------------------------------------
	}
}


