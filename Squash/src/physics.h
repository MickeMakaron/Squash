#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

#include "SFML/System/Vector3.hpp"

#include <vector>
#include <cstring>

class Ball;
class ScenePlane;

void applyMagnusForce(Ball& ball, float dt);
void applyDragForce(Ball& ball, float dt);

void handleCollisions(Ball& ball, const std::vector<ScenePlane>& planes, float& dt, std::vector<size_t>& collidingPlaneIndices);

bool findCollisionPoint(const Ball& ball, const ScenePlane& plane, float dt, sf::Vector3f& distanceVector, float& timeUntilCollision);

bool handleCollision(Ball& ball, const ScenePlane& plane, float& dt);

#endif //PHYSICS_H
