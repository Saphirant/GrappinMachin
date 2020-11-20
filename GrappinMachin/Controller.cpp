#include "Controller.h"
#include <SFML\Graphics.hpp>
#include "ViewSprite.h"
#include <algorithm>



sf::RenderWindow *Controller::window = nullptr;

Controller::Controller(EventManager *evManager): m_evManager(evManager)
{
	m_evManager->registerListener(this);
}

Controller::~Controller()
{
}






KeyboardController::KeyboardController(EventManager *evManager): Controller(evManager)
{
}

KeyboardController::~KeyboardController()
{
}

void KeyboardController::notify(Events *events)
{
	if (events->getEventType() == Events::TICK_EVENT)
	{

		//Evènements de déplacement du joueur
		Events *ev(nullptr);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			ev = new PlayerMoveRequest(sf::Vector2f(-13, 0));
			m_evManager->post(ev);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			ev = new PlayerMoveRequest(sf::Vector2f(13, 0));
			m_evManager->post(ev);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ///TODO enlever cette touche
		{
			ev = new PlayerMoveRequest(sf::Vector2f(0,-13));
			m_evManager->post(ev);
		}


		sf::Event event;
		
		while (window->pollEvent(event))
		{
			switch (event.type)
			{
			//Fermé le jeu
			case sf::Event::Closed:
				ev = new QuitEvent();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					ev = new QuitEvent();
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					if (!Recursive::exist(Recursive::JUMP))
					{
						m_evManager->post(new PlayerMoveRequest(Recursive::JUMP, sf::Vector2f(0, 90), sf::Vector2f(0, -2), sf::Vector2f(0, 0), true));
					}
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					m_evManager->post(new CreateGrabEvent(sf::Vector2f(sf::Mouse::getPosition(*window)), ));
				}
				else if (event.key.code == sf::Keyboard::LAlt)
				{
					m_evManager->post(new DeleteGrabEvent());
				}
				break;

			

			default:
				break;
			}

			//Bouton gauche de la souris, lance le grab
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					m_evManager->post(new CreateGrabEvent(sf::Vector2f(sf::Mouse::getPosition(*window))));
				}
			}

			//La fenêtre est redimensionner
			if (event.type == sf::Event::Resized)
			{

				ev = new ResizeEvent(event.size, sf::Vector2f(window->getView().getSize()));
				
			}

			if (ev != nullptr)
			{
				m_evManager->post(ev);
			}


		}
	}
}





CPUSpinnerController::CPUSpinnerController(EventManager *evManager): Controller(evManager), m_keepGoing(true)
{
}

CPUSpinnerController::~CPUSpinnerController()
{
}

void CPUSpinnerController::notify(Events *events)
{
	if (events->getEventType() == Events::QUIT_EVENT)
	{
		m_keepGoing = false;
	}
}

void CPUSpinnerController::run()
{
	sf::Clock clock;
	while (m_keepGoing)
	{
		//Update de tous les éléments récursifs
		for (unsigned int i = 0; i < Recursive::recur.size(); i++)
		{

			if (!Recursive::recur[i]->nextStep())
			{
				delete Recursive::recur[i];
				Recursive::recur.erase(Recursive::recur.begin() + i);			
			}
			else
			{
				m_evManager->post(Recursive::recur[i]);
			}
		}


		//Mis à jour de la physique du jeu (selon les FPS)
		sf::Time frameTime = clock.restart();
		float framerate = 1 / (frameTime.asMilliseconds() * 0.001);
		TickEvent *ev = new TickEvent(framerate);
		m_evManager->post(ev);

		
	}
}





