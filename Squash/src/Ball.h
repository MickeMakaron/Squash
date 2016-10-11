#pragma once

#ifndef BALL_H
#define BALL_H

#include "SceneObject.h"

class Ball : public SceneObject
{
    public:
        explicit Ball(float radius);
        float getRadius() const;

        void setTexture(const std::shared_ptr<sf::Texture>& texture) override;

		void update();
		void rotate(float dt);

		// Mostly temporary. May be replaced by some other
		// "accelerateAngular" function or something.
		void rotate(sf::Vector3f angles);
		void accelerateAngular(sf::Vector3f angularAcceleration);
		sf::Vector3f getAngularVelocity() const;

	protected:
		void resetOrigin() override;

	private:
		sf::IntRect calcSpriteFrame() const;

    private:
        float m_Radius;

        sf::Vector3f m_AngularVelocity;
		sf::Vector3f m_Rotation;

		const unsigned int BALL_TILE_SIZE = 64U;
		const float BALL_ANIM_INCREMENT = 45.f / 2.f;
};

#endif //BALL_H
