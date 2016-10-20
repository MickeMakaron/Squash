#include "Game.h"
#include "CommonMath.h"
#include "constants.h"
#include "physics.h"
#include "ScenePlane.h"

#include <iostream>
#include <limits>
#include <cassert>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics/Text.hpp>



Game::Game()
	: m_Window()
	, m_Framerate(60.f)
	, m_Shape(20.f)
	, m_Tile()
    , m_Ball(Constants::SQUASHBALL_RADIUS)
	, m_TestTexture(new sf::Texture())
	, m_PlayerTexture(new sf::Texture())
	, m_BallTexture(new sf::Texture())
	, m_ArrowTexture(new sf::Texture())
	, m_Stage()
	, m_ArrowTest(sf::Color(255,0,0))
{
    using namespace Constants;
	m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TITLE", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 8));

	m_BallView.setCenter({ 0, 0 });
	m_BallView.setSize({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });
	m_Window.setView(sf::View({0, 0}, {static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)}));

	m_BallView.zoom(1.5f);
	m_PlayerView = m_BallView;

	m_Shape.setFillColor(sf::Color::Red);
	m_Shape.setPosition(-float(WINDOW_WIDTH) / 2.f, 0.f);

//	m_Stage.loadStageFromFile("stage01.txt");
	m_Stage.loadStageFromFile("stage02.txt");

	if (!m_TestTexture->loadFromFile("./res/FloorTest1.png"))
		std::cout << "Failed to load texture" << std::endl;

	m_Tile.setTexture(m_TestTexture);

	m_PlayerTexture->loadFromFile("./res/pimp.png");
	m_Player.setTexture(m_PlayerTexture);

	m_BallTexture->loadFromFile("./res/Ball_SpriteSheet.png");
	m_Ball.setTexture(m_BallTexture);

	m_ArrowTexture->loadFromFile("./res/Arrow_SpriteSheet.png");
	m_ArrowTest.setTexture(m_ArrowTexture);
	m_Ball.setArrowTexture((m_ArrowTexture));

	m_Tile.setPosition({0, 0, 0});
	m_Player.setPosition({0, 0, 0});

	resetBall();

	m_Ball.setMass(Constants::SQUASHBALL_MASS);
//	m_Ball.accelerate({0.f, 9999.f, 0.f});
//	m_Ball.accelerateAngular({0.f, 100.f, 0.f});

	std::cout << "-----------\nBALL STARTING CONDITIONS!\nVel: (" <<
		m_Ball.getVelocity().x << ", " << m_Ball.getVelocity().y << ", " << m_Ball.getVelocity().z << ")\nRot: (" <<
		m_Ball.getAngularVelocity().x << ", " << m_Ball.getAngularVelocity().y << ", " << m_Ball.getAngularVelocity().z << ")" << std::endl;
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

			handleEvents(Constants::SECONDS_PER_FRAME);
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

void Game::handleEvents(float dt)
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
                switch(event.key.code)
                {
                    case sf::Keyboard::Escape:
                        m_Window.close();
                        break;

                    case sf::Keyboard::P:
                        paused = !paused;
                        break;

                    case sf::Keyboard::B:
                        m_IsBallView = !m_IsBallView;
                        break;

                    case sf::Keyboard::Space:
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                            swingRacquet(PI * 0.2f, dt);
                        else
                            swingRacquet(PI * 0.4f, dt);
                        break;

                    case sf::Keyboard::M:
                        m_IsSandboxMode = !m_IsSandboxMode;
                        break;

					case sf::Keyboard::K:
						m_Ball.toggleForces();
						break;

                    case sf::Keyboard::Num1:
						m_Ball.setRadius(Constants::SQUASHBALL_RADIUS);
						m_Ball.setMass(Constants::SQUASHBALL_MASS);
                        resetBall();
						break;

                    case sf::Keyboard::Num2:
						m_Ball.setRadius(Constants::FOOTBALL_RADIUS);
						m_Ball.setMass(Constants::SQUASHBALL_MASS);
						resetBall();
						break;

                    case sf::Keyboard::Num3:
						m_Ball.setRadius(Constants::FOOTBALL_RADIUS);
						m_Ball.setMass(Constants::FOOTBALL_MASS);
						resetBall();
						break;

                    default:
                        break;
                }

                break;

            case sf::Event::MouseWheelScrolled:
            {
                std::cout << event.mouseWheelScroll.delta << std::endl;
                if(event.mouseWheelScroll.delta > 10.f)
                    event.mouseWheelScroll.delta = 10.f;
                else if(event.mouseWheelScroll.delta < -10.f)
                    event.mouseWheelScroll.delta = -10.f;

                float zoomFactor = 1.f - event.mouseWheelScroll.delta / 20.f;
                m_BallView.zoom(zoomFactor);
                m_PlayerView.zoom(zoomFactor);
                break;
            }
            default:
                break;
		}

	}
}

void Game::swingRacquet(float angle, float dt)
{
    float racquetZ = 0.1f;

    float hitBoxRadius = 2.f;
    if(length(m_Player.getPosition() + sf::Vector3f(0.f, 0.f, racquetZ) - m_Ball.getPosition()) <= hitBoxRadius + m_Ball.getRadius())
    {
        sf::Vector3f racquetDirection = rotate(m_CurrentPlayerDirection, cross(m_CurrentPlayerDirection, {0.f, 0.f, 1.f}), angle);
        racquetDirection = normalize(racquetDirection);
        ScenePlane racquet(racquetDirection, 0);
        racquet.setMass(0.1f);
    //            racquet.accelerate({0.f, 0.f, 5.f});
        racquet.accelerate(racquetDirection * 3.5f);

        if(handleCollision(m_Ball, racquet, dt))
        {
            m_NumConsecutiveFloorHits = 0;
        }
    }
}

void Game::handlePlayerMovement(float dt)
{
    static const sf::Vector3f DOWN(1.f, -1.f, 0.f);
    static const sf::Vector3f UP(-1.f, 1.f, 0.f);
    static const sf::Vector3f RIGHT(1.f, 1.f, 0.f);
    static const sf::Vector3f LEFT(-1.f, -1.f, 0.f);
    static const float SPEED = 2.f * dt;
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

    if(length2(direction) > 0.f)
    {
        direction = normalize(direction);
        m_CurrentPlayerDirection = direction;
        m_Player.move(direction * SPEED);
        m_Player.move(m_Stage.containPoint(m_Player.getPosition()) - m_Player.getPosition());
        m_PlayerView.setCenter(isometricProjection(m_Player.getPosition()));
    }
}

void Game::resetBall()
{
	m_Ball.setPosition({0, 0, 2.f});
    m_Ball.accelerate(-m_Ball.getVelocity());
    m_Ball.accelerateAngular(-m_Ball.getAngularVelocity());
    m_NumConsecutiveFloorHits = 0;
}

void Game::update(float dt)
{
	if (paused)
		return;

    handlePlayerMovement(dt);

	m_Ball.clearAllForceVectors();

    float ballDt = dt;

    std::vector<Tile::Type> stageCollisions = m_Stage.collideWithStage(m_Ball, ballDt);
    for(const Tile::Type& t : stageCollisions)
    {
        if(t == Tile::Type::FLOOR)
        {
            m_NumConsecutiveFloorHits++;
            if(m_NumConsecutiveFloorHits > 1 && !m_IsSandboxMode)
                resetBall();
        }
        else
            m_NumConsecutiveFloorHits = 0;
    }

    m_Ball.move(ballDt);
	m_Ball.rotate(ballDt);

//	if (!m_Ball.isGrounded())
	{

		m_Ball.accelerate({ 0.f, 0.f, -9.82f * dt });
		m_Ball.addForceVector({ 0,0,-1 }, sf::Color(255, 255, 0));

		if(!m_Ball.isGrounded())
			applyMagnusForce(m_Ball, ballDt);

		applyDragForce(m_Ball, ballDt);
	}

	m_BallView.setCenter(isometricProjection(m_Ball.getPosition()));

	m_ArrowTest.setPosition(m_Ball.getPosition());
	m_ArrowTest.setVector(normalize(m_Ball.getVelocity()));
}

void Game::draw()
{
	// Placeholder: FPS is displayed in the title-bar of the window
	// Should be replaced with actual text on the screen.
	m_Window.setTitle(std::string(Constants::WINDOW_TITLE) + " | fps: " + std::to_string(m_Framerate));

	if(m_IsBallView)
        m_Window.setView(m_BallView);
    else
        m_Window.setView(m_PlayerView);
	// No draw calls before this.
	m_Window.clear();

	m_Window.draw(m_Shape);

	m_Window.draw(m_Tile);
	m_Window.draw(m_Stage);

	m_Window.draw(m_Ball);
	m_Window.draw(m_Player);

	sf::Text text;
	sf::Font font;
	font.loadFromFile("./res/arial.ttf");
	text.setFont(font);
	text.setString("Hello");
	text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
	text.setPosition(m_Window.getView().getCenter());
	//text.setColor(sf::Color::Blue);
	m_Window.draw(text);

	//m_Window.draw(m_ArrowTest);

	/*
		Insert draw calls here
	*/

	// No draw calls after this.
	m_Window.display();
}
