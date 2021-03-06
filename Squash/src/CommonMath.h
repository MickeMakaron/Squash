﻿#pragma once

#ifndef COMMON_MATH_H
#define COMMON_MATH_H

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include "Quaternion.h"

const float PI = 3.14159f;

float dot(const sf::Vector2f& left, const sf::Vector2f& right);
float dot(const sf::Vector3f& left, const sf::Vector3f& right);

sf::Vector3f cross(const sf::Vector3f& left, const sf::Vector3f& right);

float length2(const sf::Vector2f& v);
float length2(const sf::Vector3f& v);

float length(const sf::Vector2f& v);
float length(const sf::Vector3f& v);

sf::Vector2f normalize(const sf::Vector2f& v);
sf::Vector3f normalize(const sf::Vector3f& v);

sf::Vector2f isometricProjection(const sf::Vector3f& v);

Quaternion createRotationQuaternion(const sf::Vector3f& axis, float angle);
sf::Vector3f rotate(const sf::Vector3f& p, const sf::Vector3f& axis, float angle);
sf::Vector3f rotate(const sf::Vector3f& p, const Quaternion& q);
Quaternion rotateTransform(const Quaternion& q, const sf::Vector3f& eulerAngle);
sf::Vector3f quatRotToPYR(const Quaternion& r);

float radiansToDegrees(float radians);



#endif //COMMON_MATH
