#pragma once

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <SFML/Graphics/Texture.hpp>
#include "Tile.h"
#include "Player.h"

#include <memory>

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

	std::shared_ptr<sf::Texture>    m_TestTexture;
	std::shared_ptr<sf::Texture>    m_PlayerTexture;
	std::shared_ptr<sf::Texture>    m_BallTexture;
	Tile				            m_Tile;
	Player                          m_Player;
	SceneObject                     m_Ball;

};




#endif // GAME_H
