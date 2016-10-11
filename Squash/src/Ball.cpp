#include "Ball.h"



Ball::Ball(float radius)
: SceneObject()
, m_Radius(radius)
, m_Rotation(0, 0, 0)
{
	m_Sprite.setOrigin(BALL_TILE_SIZE / 2, BALL_TILE_SIZE / 2);
}


float Ball::getRadius() const
{
    return m_Radius;
}


void Ball::setTexture(const std::shared_ptr<sf::Texture>& texture)
{
    SceneObject::setTexture(texture);

	//m_Sprite.setTexture(*texture);
	update();
    //setSize(m_Radius, m_Radius);
}

void Ball::update()
{
	m_Sprite.setTextureRect(calcSpriteFrame());
}

void Ball::rotate(sf::Vector3f angles)
{
	m_Rotation += angles;
	while(m_Rotation.x > 360.f)
        m_Rotation.x -= 360.f;
	while(m_Rotation.x < 0)
        m_Rotation.x += 360.f;

	while(m_Rotation.y > 360.f)
        m_Rotation.y -= 360.f;
	while(m_Rotation.y < 0)
        m_Rotation.y += 360.f;

	while(m_Rotation.z > 360.f)
        m_Rotation.z -= 360.f;
	while(m_Rotation.z < 0)
        m_Rotation.z += 360.f;
//	if (m_Rotation.x >= 360)
//		m_Rotation.x %= 360;
//	if (m_Rotation.x < 0)
//		m_Rotation.x += 360;
//
//	if (m_Rotation.y >= 360)
//		m_Rotation.y %= 360;
//	if (m_Rotation.y < 0)
//		m_Rotation.y += 360;
//
//	if (m_Rotation.z >= 360)
//		m_Rotation.z %= 360;
//	if (m_Rotation.z < 0)
//		m_Rotation.z += 360;


	update();
}

void Ball::rotate(float dt)
{
//    sf::Vector3f angV(0.f, 0.f, 10.f);
//    sf::Vector3f rotation(100.f * dt, 0, 0);
    rotate(m_AngularVelocity * dt * 100.f);

}

void Ball::accelerateAngular(sf::Vector3f angularAcceleration)
{
    m_AngularVelocity += angularAcceleration;
}

sf::Vector3f Ball::getAngularVelocity() const
{
    return m_AngularVelocity;
}

void Ball::resetOrigin()
{
	float x = m_Sprite.getScale().x;
	float y = m_Sprite.getScale().y;
	m_Sprite.setOrigin(BALL_TILE_SIZE * x / 2, BALL_TILE_SIZE * y / 2);
}

sf::IntRect Ball::calcSpriteFrame() const
{
	sf::IntRect result(0, 0, BALL_TILE_SIZE, BALL_TILE_SIZE);

	int pitch_index	= static_cast<int>(roundf(static_cast<float>(m_Rotation.x) / BALL_ANIM_INCREMENT));
	int yaw_index	= static_cast<int>(roundf(static_cast<float>(m_Rotation.y) / BALL_ANIM_INCREMENT));
	int roll_index	= static_cast<int>(roundf(static_cast<float>(m_Rotation.z) / BALL_ANIM_INCREMENT));

	// The amount of tiles per "block", as in, if block_size is 8, then there are 8 blocks of 8x8 tiles
	int block_size = 360 / BALL_ANIM_INCREMENT;

	// All indices must be 0..7, if any equals 8, "modulo" it back to 0
	pitch_index = pitch_index != block_size ? pitch_index : 0;
	yaw_index	= yaw_index != block_size ? yaw_index : 0;
	roll_index	= roll_index != block_size ? roll_index : 0;

	result.left = pitch_index * BALL_TILE_SIZE;
	result.top  = yaw_index * BALL_TILE_SIZE + roll_index * block_size * BALL_TILE_SIZE;

	return result;
}
