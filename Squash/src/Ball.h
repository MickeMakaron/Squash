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

		void update(float dt);

	private:
		sf::IntRect calcSpriteFrame() const;

    private:
        float m_Radius;

		sf::Vector3i m_Rotation;

		const unsigned int BALL_TILE_SIZE = 64U;
		const unsigned int BALL_ANIM_INCREMENT = 45U;
};

#endif //BALL_H
