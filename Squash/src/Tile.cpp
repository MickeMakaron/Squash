#include "Tile.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "CommonMath.h"

Tile::Tile()
{
	setPosition({ 0,0,0 });
}

Tile::~Tile()
{
}

void Tile::setTexture(sf::Texture& texture)
{
	m_Sprite.setTexture(texture);

	float spriteWidth = m_Sprite.getGlobalBounds().width;
	float spriteHeight = m_Sprite.getGlobalBounds().height;
	m_Sprite.setOrigin(spriteWidth / 2.f, spriteHeight);

}

void Tile::setPosition(const sf::Vector3f& pos)
{
	m_Position = pos;
	m_Sprite.setPosition(isometricProjection(m_Position));
}

void Tile::move(const sf::Vector3f delta)
{
	m_Position += delta;
	m_Sprite.setPosition(isometricProjection(m_Position));
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite, states);

}
