#include "SceneObject.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "CommonMath.h"

SceneObject::SceneObject()
{
	setPosition({ 0,0,0 });
}

void SceneObject::setTexture(const std::shared_ptr<sf::Texture>& texture)
{
    m_Texture = texture;
	m_Sprite.setTexture(*texture);

	resetOrigin();
}

void SceneObject::resetOrigin()
{
	float spriteWidth = m_Sprite.getGlobalBounds().width;
	float spriteHeight = m_Sprite.getGlobalBounds().height;
	m_Sprite.setOrigin(spriteWidth / 2.f, spriteHeight);
}

void SceneObject::setPosition(const sf::Vector3f& pos)
{
	m_Position = pos;
	m_Sprite.setPosition(isometricProjection(m_Position));
}

const sf::Vector3f& SceneObject::getPosition() const
{
    return m_Position;
}


void SceneObject::move(const sf::Vector3f& delta)
{
	m_Position += delta;
	m_Sprite.setPosition(isometricProjection(m_Position));
}

void SceneObject::move(float dt)
{
    move(m_Velocity * dt);
}

void SceneObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite, states);

}

void SceneObject::accelerate(const sf::Vector3f& acc)
{
    m_Velocity += acc;
}

const sf::Vector3f& SceneObject::getVelocity() const
{
    return m_Velocity;
}

void SceneObject::setSize(float width, float height)
{
	const float SPRITE_WIDTH = m_Sprite.getGlobalBounds().width;
	const float SPRITE_HEIGHT = m_Sprite.getGlobalBounds().height;

	m_Sprite.setScale(width / SPRITE_WIDTH, height / SPRITE_HEIGHT);
	resetOrigin();
}


void SceneObject::setMass(float mass)
{
    m_Mass = mass;
}

float SceneObject::getMass() const
{
    return m_Mass;
}
