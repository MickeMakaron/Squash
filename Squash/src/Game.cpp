#include "Game.h"
#include "CommonMath.h"

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>




Game::Game()
	: m_Window()
	, m_Framerate(60.f)
	, m_Shape(20.f)
	, m_TestTexture()
	, m_Tile()
{
	m_Window.create(sf::VideoMode(1280, 720), "TITLE", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 8));

	m_Shape.setFillColor(sf::Color::Red);
	m_Shape.setPosition(0.f, 400.f - m_Shape.getRadius() / 2.f);


	if (!m_TestTexture.loadFromFile("./res/FloorTest1.png"))
		std::cout << "Failed to load texture" << std::endl;

	m_Tile.setTexture(m_TestTexture);

	m_TestTexture.loadFromFile("./res/pimp.png");
	m_Player.setTexture(m_TestTexture);

	m_Tile.setPosition({ 2,0,0 });
	m_Player.setPosition({2, 0, 0});
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


	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_Tile.move({ 1.f * dt, 0.f, 0.f });
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_Tile.move({ -1.f * dt, 0.f, 0.f });
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_Tile.move({ 0.f, 1.f * dt, 0.f });
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_Tile.move({ 0.f, -1.f * dt, 0.f });

    static const sf::Vector3f DOWN(1.f, -1.f, 0.f);
    static const sf::Vector3f UP(-1.f, 1.f, 0.f);
    static const sf::Vector3f RIGHT(1.f, 1.f, 0.f);
    static const sf::Vector3f LEFT(-1.f, -1.f, 0.f);
    static const float SPEED = 0.05f;
    sf::Vector3f direction(0.f, 0.f, 0.f);

    typedef sf::Keyboard K;
	if(K::isKeyPressed(K::D))
		direction += RIGHT;
	if(K::isKeyPressed(K::A))
		direction += LEFT;
	if(K::isKeyPressed(K::W))
		direction += UP;
	if(K::isKeyPressed(K::S))
		direction += DOWN;

    if(length(direction) > 0.f)
    {
        direction = normalize(direction);
        m_Player.move(direction * SPEED);
    }

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

	m_Window.draw(m_Tile);

	m_Window.draw(m_Player);

	/*
		Insert draw calls here
	*/

	// No draw calls after this.
	m_Window.display();
}