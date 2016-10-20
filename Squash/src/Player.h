#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector3.hpp>
#include "SceneObject.h"

#include <SFML/Window/Event.hpp>


class Player : public SceneObject
{
public:
	Player();
	~Player() override;

	void handleEvents(sf::Event& event);

	void update();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void resetOrigin() override;

private:
	sf::IntRect calcSpriteFrame();


private:
	sf::Vector2f m_FacingDirection;

	const int CHAR_TILE_WIDTH = 375;
	const int CHAR_TILE_HEIGHT = 480;
};

#endif //PLAYER_H
