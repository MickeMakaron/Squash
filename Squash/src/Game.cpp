#include "Game.h"

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

Game::Game()
	: m_Window()
	, m_Framerate(60.f)
	, m_Shape(20.f)
{
	m_Window.create(sf::VideoMode(1280, 720), "TITLE", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 8));

	m_Shape.setFillColor(sf::Color::Red);
	m_Shape.setPosition(0.f, 400.f - m_Shape.getRadius() / 2.f);
}

Game::~Game()
{
}

void Game::init()
{
	/*
		Not sure what specifically should be put here, but from experience it tends
		to be useful...

		We can remove it if we never have a need for it.
	*/
}

void Game::run()
{
	/*
		The game-loop. The heart of the beast.

		Current setup:
			- Static timestep
				Good for physics. Slow downs do not affect results.
				Bad for framerates. Sacrifice frames for more updates.
	*/

	sf::Clock timer;

	float currentDelay = timer.restart().asSeconds();
	float step = 1.f/60.f;

	while (m_Window.isOpen())
	{
		currentDelay += timer.restart().asSeconds();

		// Aging algorithm to smooth fps display
		m_Framerate = 0.5f / currentDelay + 0.5f * m_Framerate; 

		bool worldHasUpdated = false;

		while(currentDelay > step)
		{
			worldHasUpdated = true;

			handleEvents();
			update(step);
			currentDelay -= step;
		}

		if(worldHasUpdated)
			draw();
	}

}

void Game::handleEvents()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_Window.close(); 
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				m_Window.close(); 
			break;
		}

	}
}

void Game::update(float dt)
{
	// Temporary : Just for testing vvvvv
	static bool moveRight = true;
	
	if (moveRight)
		m_Shape.move(150.f * dt, 0.f);
	else
		m_Shape.move(-150.f * dt, 0.f);
	
	if (m_Shape.getPosition().x <= 0.f && !moveRight)
		moveRight = true;
	else if (m_Shape.getPosition().x + 2.f * m_Shape.getRadius() >= 1280 && moveRight)
		moveRight = false;
	// Temporary : Just for testing ^^^^^


	/*
		Insert updates here
	*/
}

void Game::draw()
{
	// Placeholder: FPS is displayed in the title-bar of the window
	// Should be replaced with actual text on the screen.
	m_Window.setTitle("fps: " + std::to_string(m_Framerate));

	// No draw calls before this.
	m_Window.clear();

	m_Window.draw(m_Shape);

	/*
		Insert draw calls here	
	*/

	// No draw calls after this.
	m_Window.display();
}
