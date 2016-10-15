#pragma once

#ifndef STAGE_H
#define STAGE_H

#include <vector>
#include <map>
#include <SFML/Graphics/RenderTexture.hpp>

#include "Tile.h"
#include "ScenePlane.h"
#include "Ball.h"

class Stage : public sf::Drawable
{
public:
	Stage();
	~Stage();

	bool loadStageFromFile(const std::string& filename);

	bool collideWithStage(Ball& ball, float dt);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	static void initialize();
	static void loadTexture(const std::string& filename);
	static std::shared_ptr<sf::Texture> getTexture(const std::string& filename);

private:
	std::vector<std::vector<Tile>>	m_Tiles;
	std::vector<ScenePlane>			m_Planes;
	sf::RenderTexture				m_RenderTexture;

	static bool m_Initialized;
	static std::map<std::string, std::shared_ptr<sf::Texture>> m_Textures;
};

#endif //STAGE
