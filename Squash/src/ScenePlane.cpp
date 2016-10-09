#include "ScenePlane.h"


ScenePlane::ScenePlane(const sf::Vector3f& normal, float d)
: Plane(normal, d)
{

}

ScenePlane::ScenePlane(const sf::Vector3f& normal, const sf::Vector3f& point)
: Plane(normal, point)
{
}
