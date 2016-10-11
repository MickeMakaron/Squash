#include "Game.h"
#include "CommonMath.h"
#include "constants.h"
#include "physics.h"
#include "ScenePlane.h"

#include <iostream>
#include <limits>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>




Game::Game()
	: m_Window()
	, m_Framerate(60.f)
	, m_Shape(20.f)
	, m_Tile()
	, m_TestTexture(new sf::Texture())
	, m_PlayerTexture(new sf::Texture())
	, m_BallTexture(new sf::Texture())
    , m_Ball(20.f)
{
    using namespace Constants;
	m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TITLE", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 8));

	m_Window.setView(sf::View({0, 0}, {static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)}));

	m_Shape.setFillColor(sf::Color::Red);
	m_Shape.setPosition(-float(WINDOW_WIDTH) / 2.f, 0.f);


	if (!m_TestTexture->loadFromFile("./res/FloorTest1.png"))
		std::cout << "Failed to load texture" << std::endl;

	m_Tile.setTexture(m_TestTexture);

	m_PlayerTexture->loadFromFile("./res/pimp.png");
	m_Player.setTexture(m_PlayerTexture);

	m_BallTexture->loadFromFile("./res/Ball_SpriteSheet.png");
	m_Ball.setTexture(m_BallTexture);

	m_Tile.setPosition({0 * TILE_SIZE, 0, 0});
	m_Player.setPosition({0, 0, 0});
	m_Ball.setPosition({0, -60, 40.f});

	m_Ball.setMass(100.f);
	m_Ball.accelerate({0.f, 100.f, 0.f});
	//m_Ball.accelerateAngular({0.f, -10.f, 0.f});
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

	while (m_Window.isOpen())
	{
		currentDelay += timer.restart().asSeconds();

		// Aging algorithm to smooth fps display
		m_Framerate = 0.5f / currentDelay + 0.5f * m_Framerate;

		bool worldHasUpdated = false;

		while(currentDelay > Constants::SECONDS_PER_FRAME)
		{
			worldHasUpdated = true;

			handleEvents();
			if(!m_Window.isOpen())
                return;
			update(Constants::SECONDS_PER_FRAME);
			currentDelay -= Constants::SECONDS_PER_FRAME;
		}

		if(worldHasUpdated)
			draw();
	}

}

bool paused = false;

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
			if (event.key.code == sf::Keyboard::P)
				paused = !paused;
			break;
			
		}

	}
}

void Game::update(float dt)
{
	if (paused)
		return;

	// Temporary : Just for testing vvvvv
	static bool moveRight = true;
	static bool isBallRolling = false;

	if (moveRight)
		m_Shape.move(150.f * dt, 0.f);
	else
		m_Shape.move(-150.f * dt, 0.f);

	if (m_Shape.getPosition().x <= -float(Constants::WINDOW_WIDTH) / 2.f && !moveRight)
		moveRight = true;
	else if (m_Shape.getPosition().x + 2.f * m_Shape.getRadius() >= Constants::WINDOW_WIDTH / 2.f && moveRight)
		moveRight = false;
	// Temporary : Just for testing ^^^^^


//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//		m_Tile.move({ 1.f * dt, 0.f, 0.f });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//		m_Tile.move({ -1.f * dt, 0.f, 0.f });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//		m_Tile.move({ 0.f, 1.f * dt, 0.f });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//		m_Tile.move({ 0.f, -1.f * dt, 0.f });

    static const sf::Vector3f DOWN(1.f, -1.f, 0.f);
    static const sf::Vector3f UP(-1.f, 1.f, 0.f);
    static const sf::Vector3f RIGHT(1.f, 1.f, 0.f);
    static const sf::Vector3f LEFT(-1.f, -1.f, 0.f);
    static const float SPEED = 1.f;
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



    ScenePlane plane({0.f, 0.f, 1.f}, 0);
    ScenePlane wall({0.f, -1.f, 0.f}, -Constants::TILE_SIZE * 2.f);
    plane.setMass(0.f);
    wall.setMass(0.f);

    if(handleCollision2(m_Ball, plane))
    {
        isBallRolling = std::fabs(m_Ball.getVelocity().z) < 10.f;
        if(isBallRolling)
            m_Ball.accelerate({0.f, 0.f, -m_Ball.getVelocity().z});
    }
    else //if(!isBallRolling)
        m_Ball.accelerate({0.f, 0.f, -9.82f * 20.f * dt});



//    handleCollision(m_Ball, wall);
    m_Ball.move(dt);
    m_Ball.rotate(dt);

//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//		m_Ball.rotate({ 5,0,0 });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//		m_Ball.rotate({ -5,0,0 });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//		m_Ball.rotate({ 0,-5,0 });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//		m_Ball.rotate({ 0,5,0 });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
//		m_Ball.rotate({ 0,0,-5 });
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
//		m_Ball.rotate({ 0,0,5 });


//    float ballZ = m_Ball.getPosition().z - m_Ball.getRadius();
//    if(ballZ < 0.f)
//    {
//        m_Ball.move({0, 0, -ballZ});
//        m_Ball.accelerate({0, 0, -m_Ball.getVelocity().z * 1.5f});
//    }
//    m_Player.accelerate({0.f, 0.f, -1.f * SPEED});
//    m_Player.move(dt);

	/*
		Insert updates here
	*/
}

void Game::draw()
{
	// Placeholder: FPS is displayed in the title-bar of the window
	// Should be replaced with actual text on the screen.
	m_Window.setTitle(std::string(Constants::WINDOW_TITLE) + " | fps: " + std::to_string(m_Framerate));

	// No draw calls before this.
	m_Window.clear();

	m_Window.draw(m_Shape);

	m_Window.draw(m_Tile);

	m_Window.draw(m_Ball);
	//m_Window.draw(m_Player);


	/*
		Insert draw calls here
	*/

	// No draw calls after this.
	m_Window.display();
}
