#include "Stage.h"

#include <fstream>
#include <cassert>
#include "constants.h"
#include "physics.h"
#include "CommonMath.h"

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
					tile.setType(Tile::Type::FLOOR);
					break;
				case 'W':
					tile.setTexture(getTexture("WallTest1"));
					tile.setType(Tile::Type::WALL);
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

std::vector<Tile::Type> Stage::collideWithStage(Ball& ball, float& dt)
{
	std::vector<size_t> collidingPlaneIndices;
	handleCollisions(ball, m_Planes, dt, collidingPlaneIndices);
	assert(m_Planes[0].getNormal() == sf::Vector3f(0.f, 0.f, 1.f) && m_Planes[0].getD() == 0.f);

	std::vector<Tile::Type> collidingPlaneTypes(collidingPlaneIndices.size());
	for(size_t i = 0; i < collidingPlaneIndices.size(); i++)
        if(collidingPlaneIndices[i] == 0) // If floor
            collidingPlaneTypes[i] = Tile::Type::FLOOR;
        else
            collidingPlaneTypes[i] = Tile::Type::WALL;

    return collidingPlaneTypes;
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

sf::Vector3f Stage::containPoint(const sf::Vector3f& p) const
{
    float dMinSqrd = std::numeric_limits<float>::max();
    sf::Vector3f containedPoint = p;
    for(const std::vector<Tile>& row : m_Tiles)
        for(const Tile& tile : row)
        {
            if(tile.getType() == Tile::Type::FLOOR)
            {
                float dSqrd = length2(p - tile.getPosition());
                if(dSqrd < dMinSqrd)
                {
                    static const float HALF_TILE_SIZE_SQRD = pow(2.f / 2.f, 2.f);
                    if(dSqrd < HALF_TILE_SIZE_SQRD)
                    {
                        return p;
                    }
                    dMinSqrd = dSqrd;
                    containedPoint = p;

                    static const float HALF_TILE_SIZE = 2.f / 2.f;
                    sf::Vector3f center = tile.getPosition();
                    float left = center.x - HALF_TILE_SIZE;
                    float right = center.x + HALF_TILE_SIZE;
                    float top = center.y + HALF_TILE_SIZE;
                    float bot = center.y - HALF_TILE_SIZE;

                    if(p.x < left)
                        containedPoint.x = left;
                    else if(p.x > right)
                        containedPoint.x = right;
                    if(p.y < bot)
                        containedPoint.y = bot;
                    else if(p.y > top)
                        containedPoint.y = top;
                }
            }
        }

    return containedPoint;
}
