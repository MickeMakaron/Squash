#include "Plane.h"

Plane::Plane(const sf::Vector3f&normal, float d)
	: m_Normal(normal)
	, m_D(d)
{

}

Plane::Plane(const sf::Vector3f& normal, const sf::Vector3f& point)
	: m_Normal(normal)
{
	
}

Plane::~Plane()
{
}
