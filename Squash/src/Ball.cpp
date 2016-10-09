#include "Ball.h"



Ball::Ball(float radius)
: SceneObject()
, m_Radius(radius)
{
}


float Ball::getRadius() const
{
    return m_Radius;
}


void Ball::setTexture(const std::shared_ptr<sf::Texture>& texture)
{
    SceneObject::setTexture(texture);
    setSize(m_Radius, m_Radius);
}
