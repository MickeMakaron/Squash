#include "ForceArrow.h"
#include "CommonMath.h"

ForceArrow::ForceArrow(sf::Color color)
	: SceneObject()
	, m_Direction(normalize({0,1,0}))
{
	m_Sprite.setColor(color);
	update();
}

ForceArrow::~ForceArrow()
{
}

void ForceArrow::setTexture(const std::shared_ptr<sf::Texture>& texture)
{
	SceneObject::setTexture(texture);
	update();
}

void ForceArrow::setColor(sf::Color color)
{
	m_Sprite.setColor(color);
}

void ForceArrow::setVector(sf::Vector3f vec)
{
	if(length2(vec) > 0.f)
	{
		m_Direction = normalize(vec);
		update();
	}
}

void ForceArrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	SceneObject::draw(target, states);
}

void ForceArrow::resetOrigin()
{
	float x = m_Sprite.getScale().x;
	float y = m_Sprite.getScale().y;
	m_Sprite.setOrigin(ARROW_TILE_SIZE * x / 2, ARROW_TILE_SIZE * y / 2);
}

sf::IntRect ForceArrow::calcSpriteFrame() const
{
	sf::IntRect result(0, 0, ARROW_TILE_SIZE, ARROW_TILE_SIZE);

	//Quaternion q = createRotationQuaternion(m_Direction, 90.f);

	//sf::Vector3f rotation = quatRotToPYR(q);
	sf::Vector3f rotation;
	rotation.y = (180 / PI) * acos(dot(m_Direction, {0,0,1}));
	rotation.z = (180 / PI) * acos(dot(m_Direction, {1,0,0}));

	if(m_Direction.y <= 0.f)
	{
		rotation.z = 2 * PI - rotation.z;
	}

	int pitch_index = static_cast<int>(roundf(static_cast<float>(rotation.z) / ARROW_ANIM_INCREMENT));
	int yaw_index = static_cast<int>(roundf(static_cast<float>(rotation.y) / ARROW_ANIM_INCREMENT));

	// The amount of tiles per "block", as in, if block_size is 8, then there are 8 blocks of 8x8 tiles
	int block_size = 360 / ARROW_ANIM_INCREMENT;

	// All indices must be 0..7, if any equals 8, "modulo" it back to 0
	pitch_index = pitch_index != block_size ? pitch_index : 0;
	yaw_index = yaw_index != block_size / 2 + 1 ? yaw_index : 0;

	result.left = pitch_index * ARROW_TILE_SIZE;
	result.top = yaw_index * ARROW_TILE_SIZE;

	return result;

}

void ForceArrow::update()
{
	m_Sprite.setTextureRect(calcSpriteFrame());
}
