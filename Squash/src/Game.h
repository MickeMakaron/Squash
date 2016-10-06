#pragma once

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

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

};




#endif // GAME_H
