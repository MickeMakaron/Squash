#include "Plane.h"

#include "CommonMath.h"

Plane::Plane(const sf::Vector3f& normal, float d)
	: m_Normal(normalize(normal)) // Do we want to instead assume that "normal" already is normalized?
	, m_D(d)
{

}

Plane::Plane(const sf::Vector3f& normal, const sf::Vector3f& point)
	: m_Normal(normalize(normal)) // Do we want to instead assume that "normal" already is normalized?
{
	m_D = dot(m_Normal, point);
	
}

Plane::~Plane()
{
}

const sf::Vector3f& Plane::getNormal() const
{
	return m_Normal;
}

float Plane::getD() const
{
	return m_D;
}
