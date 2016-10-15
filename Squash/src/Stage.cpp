#include "Stage.h"

#include <fstream>
#include "constants.h"
#include "physics.h"

bool Stage::m_Initialized = false;
std::map<std::string, std::shared_ptr<sf::Texture>> Stage::m_Textures;

Stage::Stage()
{
	initialize();
}

Stage::~Stage()
{
}

bool Stage::loadStageFromFile(const std::string& filename)
{
	std::ifstream inFile;

	inFile.open("./res/" + filename);

	bool success = false;
	if (inFile.is_open())
	{
		int width;
		int height;
		inFile >> width;
		inFile >> height;

		// Floor
		m_Planes.push_back(ScenePlane({ 0,0,1 }, 0.f));

		// Walls
		m_Planes.push_back(ScenePlane({1,0,0}, -static_cast<float>(width-2)));
		m_Planes.push_back(ScenePlane({-1,0,0}, -static_cast<float>(width-2)));
		m_Planes.push_back(ScenePlane({0,1,0}, -static_cast<float>(height-2)));
		m_Planes.push_back(ScenePlane({0,-1,0}, -static_cast<float>(height-2)));

		int lineIndex = 0;
		std::string line;
		
		// Consume the \n to get to the next line with actual data.
		getline(inFile, line);

		while (getline(inFile, line))
		{
			m_Tiles.emplace_back();
			for (int i = 0; i < line.size(); i++)
			{
				Tile tile;
				switch (line[i])
				{
				case 'F':
					tile.setTexture(getTexture("FloorTest1"));
					break;
				case 'W':
					tile.setTexture(getTexture("WallTest1"));
					break;
				case 'w':
					//Invisible walls
					//tile.setTexture(getTexture("WallTest1"));
					break;
				}
				tile.setPosition({
					2.f * static_cast<float>(i - width / 2), 
					2.f * static_cast<float>(-(lineIndex - height / 2)), 
					0.f});

				m_Tiles.back().push_back(tile);
			}
			lineIndex++;
		}
		m_RenderTexture.clear();
		m_RenderTexture.create(
			(width + height) * Constants::MAX_TILE_WIDTH / 2, 
			(width + height - 1) * Constants::MIN_TILE_HEIGHT + Constants::MAX_TILE_HEIGHT);
		
		m_RenderTexture.setView(sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(m_RenderTexture.getTexture().getSize().x, m_RenderTexture.getTexture().getSize().y)));

		for (auto& row : m_Tiles)
		{
			for (auto& tile : row)
			{
				m_RenderTexture.draw(tile);
			}
		}
		m_RenderTexture.display();

		success = true;
	}

	inFile.close();

	return success;
}

bool Stage::collideWithStage(Ball& ball, float dt)
{
	bool anyCollision = false;
	for (auto& plane : m_Planes)
	{
		anyCollision = handleCollision2(ball, plane, dt);
	}
	return anyCollision;
}

void Stage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite stageSprite;
	stageSprite.setTexture(m_RenderTexture.getTexture());
	stageSprite.setOrigin(m_RenderTexture.getTexture().getSize().x / 2, m_RenderTexture.getTexture().getSize().y / 2);
	target.draw(stageSprite);
}

void Stage::initialize()
{
	if(!m_Initialized)
	{
		loadTexture("FloorTest1");
		loadTexture("WallTest1");

		m_Initialized = true;
	}
}

void Stage::loadTexture(const std::string& filename)
{
	if (m_Textures.find(filename) == m_Textures.end())
	{
		auto texture = m_Textures.insert(std::pair<std::string, std::shared_ptr<sf::Texture>>(filename, std::make_shared<sf::Texture>())).first;
		texture->second->loadFromFile("./res/" + filename + ".png");
	}
}

std::shared_ptr<sf::Texture> Stage::getTexture(const std::string& filename)
{
	return m_Textures.at(filename);
}
