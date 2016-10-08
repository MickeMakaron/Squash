#pragma once

#ifndef BALL_H
#define BALL_H

#include "SceneObject.h"

class Ball : public SceneObject
{
    public:
        explicit Ball(float radius);
        float getRadius() const;

        virtual void setTexture(const std::shared_ptr<sf::Texture>& texture) override;

    private:
        float m_Radius;
};

#endif //BALL_H
