#pragma once

#ifndef FORCE_ARROW_H
#define FORCE_ARROW_H
#include "SceneObject.h"

class ForceArrow : public SceneObject
{
public:
	ForceArrow(sf::Color color);
	virtual ~ForceArrow();

	void setTexture(const std::shared_ptr<sf::Texture>& texture) override;

	void setColor(sf::Color color);
	void setVector(sf::Vector3f vec);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void resetOrigin() override;

private:
	sf::IntRect calcSpriteFrame() const;
	void update();

private:
	sf::Vector3f	m_Direction;

	const unsigned int ARROW_TILE_SIZE = 128;
	const float ARROW_ANIM_INCREMENT = 45.f / 2.f;
};


#endif
