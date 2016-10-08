#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector3.hpp>

class Player : public sf::Drawable
{
public:
	Player();

	void setTexture(sf::Texture& texture);

	void setPosition(const sf::Vector3f& pos);
	void move(const sf::Vector3f delta);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Sprite		m_Sprite;
	sf::Vector3f	m_Position;

};

#endif //PLAYER_H
