#pragma once

#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector3.hpp>

class Tile : public sf::Drawable
{
public:
	Tile();
	~Tile();

	void setTexture(sf::Texture& texture);

	void setPosition(const sf::Vector3f& pos);
	void move(const sf::Vector3f delta);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Sprite		m_Sprite;
	sf::Vector3f	m_Position;

};

#endif //TILE
