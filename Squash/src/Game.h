#pragma once

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <SFML/Graphics/Texture.hpp>
#include "Tile.h"
#include "Player.h"
#include "Ball.h"

#include <memory>
#include "Stage.h"
#include "ForceArrow.h"

class Game
{
public:
	Game();
	~Game();

	void init();

	void run();

	void handleEvents(float dt);
	void update(float dt);
	void draw();

private:
    void swingRacquet(float angle, float dt);
    void handlePlayerMovement(float dt);

private:
	sf::RenderWindow	m_Window;
	float				m_Framerate;

	sf::CircleShape		m_Shape;	// A temporary test shape

	sf::View			m_BallView;
	sf::View            m_PlayerView;

	bool                m_IsBallView = false;
	bool                m_IsSandboxMode = true;
	unsigned int        m_NumConsecutiveFloorHits = 0;
	sf::Vector3f        m_CurrentPlayerDirection = sf::Vector3f(0.f, 0.f, 1.f);

	std::shared_ptr<sf::Texture>    m_TestTexture;
	std::shared_ptr<sf::Texture>    m_PlayerTexture;
	std::shared_ptr<sf::Texture>    m_BallTexture;
	std::shared_ptr<sf::Texture>    m_ArrowTexture;
	Tile				            m_Tile;
	Player                          m_Player;
	Ball							m_Ball;
	ForceArrow						m_ArrowTest;

	Stage m_Stage;

};




#endif // GAME_H
