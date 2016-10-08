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
	virtual void move(const sf::Vector3f delta);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::shared_ptr<sf::Texture> m_Texture;
	sf::Sprite      m_Sprite;
	sf::Vector3f    m_Position;

};

#endif //SCENE_OBJECT_H
