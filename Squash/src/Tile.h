#pragma once

#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector3.hpp>

#include "SceneObject.h"

class Tile : public SceneObject
{
public:
	Tile();
	~Tile();

};

#endif //TILE
