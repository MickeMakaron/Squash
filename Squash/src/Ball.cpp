#include "Ball.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "CommonMath.h"


Ball::Ball(float radius)
: SceneObject()
, m_Radius(radius)
, m_AngularVelocity(0.f, 0.f, 0.f)
, m_Rotation(0.f, 0.f, 0.f)
, m_IsGrounded(false)
, m_RotationTransform(createRotationQuaternion({0.f, 0.f, 1.f}, 0))
{
	m_Sprite.setOrigin(BALL_TILE_SIZE / 2, BALL_TILE_SIZE / 2);

	m_Shadow.setRadius(BALL_TILE_SIZE / 2);
	m_Shadow.setOrigin(BALL_TILE_SIZE / 2, BALL_TILE_SIZE / 2);
	m_Shadow.setScale(1.f, 0.5f);
	m_Shadow.setFillColor(sf::Color(0, 0, 0, 100));
}


float Ball::getRadius() const
{
    return m_Radius;
}


void Ball::setTexture(const std::shared_ptr<sf::Texture>& texture)
{
    SceneObject::setTexture(texture);
	update();
}

void Ball::update()
{
	m_Sprite.setTextureRect(calcSpriteFrame());
}

void Ball::rotate(sf::Vector3f angles)
{
    m_RotationTransform = rotateTransform(m_RotationTransform, angles);
    m_Rotation = quatRotToPYR(m_RotationTransform);
    update();
}

void Ball::rotate(float dt)
{
    rotate(m_AngularVelocity * dt);
}

void Ball::accelerateAngular(sf::Vector3f angularAcceleration)
{
    m_AngularVelocity += angularAcceleration;
}

sf::Vector3f Ball::getAngularVelocity() const
{
    return m_AngularVelocity;
}

void Ball::move(const sf::Vector3f& delta)
{
	SceneObject::move(delta);

	sf::Vector3f shadowPos = this->getPosition();
	shadowPos.z = 0;
	m_Shadow.setPosition(isometricProjection(shadowPos));
}

void Ball::move(float dt)
{
	SceneObject::move(dt);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Shadow);

	SceneObject::draw(target, states);
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

bool Ball::isGrounded() const
{
    return m_IsGrounded;
}

void Ball::setGrounded(bool flag)
{
    m_IsGrounded = flag;
}
