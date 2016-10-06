﻿#pragma once

#ifndef PLANE_H
#define PLANE_H

#include <SFML/System/Vector3.hpp>

class Plane
{
public:
	Plane(const sf::Vector3f& normal, float d);
	Plane(const sf::Vector3f& normal, const sf::Vector3f& point);
	~Plane();



private:
	sf::Vector3f	m_Normal;
	float			m_D;


};

#endif //PLANE
