#pragma once

#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector3.hpp>

#include "SceneObject.h"

class Tile : public SceneObject
{
    public:
        enum class Type : unsigned char
        {
            WALL,
            FLOOR,
            INVALID
        };
    public:
        Tile();
        ~Tile();

        void setType(Type type);
        Type getType() const;

    private:
        Type m_Type = Type::INVALID;
};

#endif //TILE
