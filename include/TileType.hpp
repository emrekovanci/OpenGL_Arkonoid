#pragma once

#include "Graphics/Color.hpp"

enum class TileType
{
    Empty = 0,
    Solid = 1,
	Blue = 2,
	Green = 3,
	Magenta = 4,
	Yellow = 5
};

Color getColorForTileType(TileType tileType)
{
    switch (tileType)
    {
        case TileType::Blue: return Color::Blue;
        case TileType::Green: return Color::Green;
        case TileType::Magenta: return Color::Magenta;
        case TileType::Yellow: return Color::Yellow;
        default: return Color::White;
    }
}