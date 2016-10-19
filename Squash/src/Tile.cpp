#include "Tile.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include "CommonMath.h"

Tile::Tile()
{
	setPosition({ 0,0,0 });
}

Tile::~Tile()
{
}

void Tile::setType(Type type)
{
    m_Type = type;
}

Tile::Type Tile::getType() const
{
    return m_Type;
}
