#include "CommonMath.h"
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
	float l = length(v);
	if (l <= 1.0e-9f)
		return sf::Vector2f(0.f, 0.f);

	sf::Vector2f result = v / l;
	return result;
}

sf::Vector3f normalize(const sf::Vector3f& v)
{
	float l = length(v);
	if (l <= 1.0e-9f)
		return sf::Vector3f(0.f,0.f,0.f);

	sf::Vector3f result = v / l;
	return result;
}

sf::Vector2f isometricProjection(const sf::Vector3f& v)
{
	sf::Vector2f result;

	result.x = (2.f * v.x + 2.f * v.y            );
	result.y = (	  v.x -		  v.y - 2.f * v.z);

	return result;
}

Quaternion createRotationQuaternion(const sf::Vector3f& axis, float angle)
{
    Quaternion q;
    q.r = cosf(angle / 2.f);
    const float K = sinf(angle / 2.f);
    q.x = axis.x * K;
    q.y = axis.y * K;
    q.z = axis.z * K;
    return q;
}

sf::Vector3f rotate(const sf::Vector3f& p, const sf::Vector3f& axis, float angle)
{
    Quaternion q = createRotationQuaternion({axis.x, axis.y, axis.z}, angle);
    Quaternion pRot = q * Quaternion(p.x, p.y, p.z) * ~q;
    return {pRot.x, pRot.y, pRot.z};
}
