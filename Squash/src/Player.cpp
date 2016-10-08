#include "Player.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "CommonMath.h"

Player::Player()
{
	setPosition({ 0,0,0 });
}

void Player::setTexture(sf::Texture& texture)
{
	m_Sprite.setTexture(texture);

	float spriteWidth = m_Sprite.getGlobalBounds().width;
	float spriteHeight = m_Sprite.getGlobalBounds().height;
	m_Sprite.setOrigin(spriteWidth / 2.f, spriteHeight);

}

void Player::setPosition(const sf::Vector3f& pos)
{
	m_Position = pos;
	m_Sprite.setPosition(isometricProjection(m_Position));
}

void Player::move(const sf::Vector3f delta)
{
	m_Position += delta;
	m_Sprite.setPosition(isometricProjection(m_Position));
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite, states);

}
