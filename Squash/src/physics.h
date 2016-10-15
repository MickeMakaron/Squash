#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

#include "SFML/System/Vector3.hpp"

class Ball;
class ScenePlane;

void applyMagnusForce(Ball& ball, float dt);

bool handleContact(Ball& ball, const ScenePlane& plane, float dt);

bool handleCollision(Ball& ball, const ScenePlane& plane);



bool handleCollision2(Ball& ball, const ScenePlane& plane, const sf::Vector3f& planeCenter, float radius, float dt);
bool handleCollision2(Ball& ball, const ScenePlane& plane, float dt);

#endif //PHYSICS_H
