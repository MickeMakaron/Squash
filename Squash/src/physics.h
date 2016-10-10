#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

class Ball;
class ScenePlane;


bool handleCollision(Ball& ball, const ScenePlane& planes);
bool handleCollision2(Ball& ball, const ScenePlane& planes);

#endif //PHYSICS_H
