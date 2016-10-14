﻿#pragma once

#ifndef PHYSICS_H
#define PHYSICS_H

class Ball;
class ScenePlane;

void applyMagnusForce(Ball& ball);

bool handleContact(Ball& ball, const ScenePlane& plane, float dt);

bool handleCollision(Ball& ball, const ScenePlane& plane);
bool handleCollision2(Ball& ball, const ScenePlane& plane, float dt);

#endif //PHYSICS_H
