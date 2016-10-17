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

class Game
{
public:
	Game();
	~Game();

	void init();

	void run();

	void handleEvents();
	void update(float dt);
	void draw();

private:
	sf::RenderWindow	m_Window;
	float				m_Framerate;

	sf::CircleShape		m_Shape;	// A temporary test shape

	sf::View			m_BallView;

	Tile				            m_Tile;
	Player                          m_Player;
	Ball							m_Ball;
	std::shared_ptr<sf::Texture>    m_TestTexture;
	std::shared_ptr<sf::Texture>    m_PlayerTexture;
	std::shared_ptr<sf::Texture>    m_BallTexture;

	Stage m_Stage;

};




#endif // GAME_H
