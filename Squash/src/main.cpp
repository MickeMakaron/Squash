//#include <iostream>
//#include <vector>
//#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include "Game.h"

#define TILE_SIZE 60.f
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "HELLO WORLD!"

/*
	Notice:
		Most things in this file at the moment are temporary
		and just for testing. Most globals, functions, macros 
		and classes are not meant to be in the final product.
*/

// Function prototypes

sf::Vector2f projectCoordinates(sf::Vector3f in);
//void init();
//void handleEvents(sf::RenderWindow& window);
//void update();
//void draw();

// TEMP classes

//class Tile : public sf::Transformable, public sf::Drawable
//{
//public:
//	Tile(sf::Vector3f position)
//		: m_globalPosition(position)
//		, m_shape(TILE_SIZE, 4)
//	{
//		m_shape.setOrigin(TILE_SIZE, TILE_SIZE);
//		m_shape.setScale(2.f, 1.f);
//
//		m_shape.setFillColor(sf::Color(rand(), rand(), rand()));
//
//		update();
//	}
//
//	void update()
//	{
//		setPosition(projectCoordinates(m_globalPosition));
//	}
//
//protected:
//	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
//	{
//		states.transform *= getTransform();
//
//		target.draw(m_shape, states);
//	}
//
//
//private:
//	sf::Vector3f m_globalPosition;
//
//	sf::CircleShape m_shape;
//};
//
//class Ball : public sf::Transformable, public sf::Drawable
//{
//public:
//	Ball()
//		: m_time(0.f)
//		, m_BallShape(20.f)
//		, m_ShadowShape(10.f)
//		, m_globalPosition(0.f, 0.f, 0.f)
//	{
//		m_BallShape.setOrigin(10.f, 10.f);
//
//		m_ShadowShape.setOrigin(5.f, 5.f);
//		m_ShadowShape.setScale(2.f, 1.f);
//		m_ShadowShape.setFillColor(sf::Color::Black);
//	}
//
//	void update()
//	{
//		m_globalPosition.x = sinf(m_time);
//		m_globalPosition.y = cosf(m_time);
//
//		float z = 1.f;// sinf(m_time);
//
//		setPosition(projectCoordinates(m_globalPosition));
//		m_BallShape.setPosition(projectCoordinates(sf::Vector3f(0.f, 0.f, z)));
//
//		m_time += 0.001f;
//	}
//
//protected:
//	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
//	{
//		states.transform *= getTransform();
//
//		target.draw(m_ShadowShape, states);
//		target.draw(m_BallShape, states);
//	}
//
//private:
//	float m_time;
//
//	sf::CircleShape m_BallShape;
//	sf::CircleShape m_ShadowShape;
//
//	sf::Vector3f m_globalPosition;
//};

// Globals
//sf::RenderWindow gWindow;
//std::vector<Tile> gTiles;
//Ball gBall;

int main()
{
	using namespace std;

	Game game;
	game.init();
	game.run();

	/*init();

	while (gWindow.isOpen())
	{
		handleEvents(gWindow);
		update();
		draw();
	}
*/
	return 0;
}

// Function definitions ----

sf::Vector2f projectCoordinates(sf::Vector3f in)
{
	sf::Vector2f result;

	result.x = (2.f * in.x + 2.f * in.y);
	result.y = (in.x - in.y - 2.f * in.z);

	result *= TILE_SIZE;

	return result;
}

//void init()
//{
//	srand(time(nullptr));
//
//	gWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default);
//
//	gTiles.push_back(Tile({ 0,0,0 }));
//	gTiles.push_back(Tile({ 1,0,0 }));
//	gTiles.push_back(Tile({ 0,1,0 }));
//	gTiles.push_back(Tile({ 1,1,0 }));
//	gTiles.push_back(Tile({ -1,0,0 }));
//	gTiles.push_back(Tile({ 0,-1,0 }));
//	gTiles.push_back(Tile({ -1,-1,0 }));
//
//	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
//	gWindow.setView(view);
//}
//
//void handleEvents(sf::RenderWindow& window)
//{
//	sf::Event event;
//	while (window.pollEvent(event))
//	{
//		if (event.type == sf::Event::Closed)
//			window.close();
//		if (event.type == sf::Event::KeyPressed)
//			if (event.key.code == sf::Keyboard::Escape)
//				window.close();
//	}
//}
//
//void update()
//{
//	gBall.update();
//}
//
//void draw()
//{
//	gWindow.clear();
//
//	for (auto& t : gTiles)
//		gWindow.draw(t);
//
//	gWindow.draw(gBall);
//
//	gWindow.display();
//}