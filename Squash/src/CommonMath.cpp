#include "CommonMath.h"
#include "constants.h"
#include <set>
#include <cmath>

float dot(const sf::Vector2f& left, const sf::Vector2f& right)
{
	return left.x * right.x + left.y * right.y;
}

float dot(const sf::Vector3f& left, const sf::Vector3f& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

sf::Vector3f cross(const sf::Vector3f& left, const sf::Vector3f& right)
{
	sf::Vector3f result;
	result.x = left.y * right.z - left.z * right.y;
	result.y = left.z * right.x - left.x * right.z;
	result.z = left.x * right.y - left.y * right.x;
	return result;
}

float length2(const sf::Vector2f& v)
{
	return v.x * v.x + v.y * v.y;
}

float length2(const sf::Vector3f& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float length(const sf::Vector2f& v)
{
	return sqrtf(length2(v));
}

float length(const sf::Vector3f& v)
{
	return sqrtf(length2(v));
}

sf::Vector2f normalize(const sf::Vector2f& v)
{
	sf::Vector2f result = v / length(v);
	return result;
}

sf::Vector3f normalize(const sf::Vector3f& v)
{
	sf::Vector3f result = v / length(v);
	return result;
}

sf::Vector2f isometricProjection(const sf::Vector3f& v)
{
	sf::Vector2f result;

	result.x = (2.f * v.x + 2.f * v.y            );
	result.y = (	  v.x -		  v.y - 2.f * v.z);

	result *= Constants::TILE_SIZE;

	return result;
}
