#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

#include "Plane.h"
#include <vector>
class Ball;


void handleCollision(Ball& ball, const std::vector<Plane>& planes);

#endif //PHYSICS_H
