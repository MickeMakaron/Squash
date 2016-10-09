#pragma once

#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector3.hpp>

#include <memory>

class SceneObject : public sf::Drawable
{
public:
	SceneObject();

	virtual void setTexture(const std::shared_ptr<sf::Texture>& texture);

	virtual void setPosition(const sf::Vector3f& pos);
	virtual const sf::Vector3f& getPosition() const;
	virtual void move(const sf::Vector3f& delta);
	virtual void move(float dt);
	virtual void accelerate(const sf::Vector3f& acc);
	virtual const sf::Vector3f& getVelocity() const;
	void setSize(float width, float height);

	void setMass(float mass);
	float getMass() const;


protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void resetOrigin();

protected:
	sf::Sprite      m_Sprite;

private:
    std::shared_ptr<sf::Texture> m_Texture;
	sf::Vector3f    m_Position;
	sf::Vector3f    m_Velocity;
	float           m_Mass = 0.f;

};

#endif //SCENE_OBJECT_H
