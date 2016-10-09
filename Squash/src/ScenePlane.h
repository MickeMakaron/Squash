#pragma once

#ifndef SCENE_PLANE_H
#define SCENE_PLANE_H

#include "SceneObject.h"
#include "Plane.h"


class ScenePlane : public SceneObject, public Plane
{
    public:
        ScenePlane(const sf::Vector3f& normal, float d);
        ScenePlane(const sf::Vector3f& normal, const sf::Vector3f& point);
};

#endif //SCENE_PLANE_H
