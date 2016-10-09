#include "Ball.h"



Ball::Ball(float radius)
: SceneObject()
, m_Radius(radius)
, m_Rotation(0, 0, 0)
{
	//m_Sprite.setOrigin(BALL_TILE_SIZE / 2, BALL_TILE_SIZE / 2);
}


float Ball::getRadius() const
{
    return m_Radius;
}


void Ball::setTexture(const std::shared_ptr<sf::Texture>& texture)
{
    SceneObject::setTexture(texture);
    setSize(m_Radius, m_Radius);
	m_Sprite.setTextureRect(calcSpriteFrame());
}

void Ball::update(float dt)
{
	m_Sprite.setTextureRect(calcSpriteFrame());
}

sf::IntRect Ball::calcSpriteFrame() const
{
	sf::IntRect result(0, 0, BALL_TILE_SIZE, BALL_TILE_SIZE);

	int pitch_index	= static_cast<int>(roundf(static_cast<float>(m_Rotation.x) / BALL_ANIM_INCREMENT));
	int yaw_index		= static_cast<int>(roundf(static_cast<float>(m_Rotation.y) / BALL_ANIM_INCREMENT));
	int roll_index	= static_cast<int>(roundf(static_cast<float>(m_Rotation.z) / BALL_ANIM_INCREMENT));

	result.left = pitch_index * BALL_TILE_SIZE;
	result.top  = yaw_index * BALL_TILE_SIZE + roll_index * (360 / BALL_ANIM_INCREMENT) * BALL_TILE_SIZE;

	return result;
}
