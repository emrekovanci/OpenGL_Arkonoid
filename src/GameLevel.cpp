#include "GameLevel.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "TileType.hpp"
#include "ResourceManager.hpp"

void GameLevel::Init(Tiles tiles, unsigned int levelWidth, unsigned int levelHeight)
{
	std::size_t height = tiles.size();
	std::size_t width = tiles[0].size();

	auto unitWidth = static_cast<float>(levelWidth) / width;
	auto unitHeight = static_cast<float>(levelHeight) / height;

	const Texture2D& solidBlockTexture = ResourceManager::GetTexture("block_solid");
	const Texture2D& regularBlockTexture = ResourceManager::GetTexture("block");

	for (std::size_t y = 0; y < height; ++y)
	{
		for (std::size_t x = 0; x < width; ++x)
		{
			glm::vec2 pos(unitWidth * x, unitHeight * y);
			glm::vec2 size(unitWidth, unitHeight);

			TileType tileType = static_cast<TileType>(tiles[y][x]);
			if (tileType == TileType::Solid)
			{
				GameObject& obj = Bricks.emplace_back(pos, size, solidBlockTexture, Color::Gray.Rgb);
				obj.Solid = true;
			}
			else if (tileType != TileType::Empty)
			{
				glm::vec3 color = getColorForTileType(tileType).Rgb;
				Bricks.emplace_back(pos, size, regularBlockTexture, color);
			}
		}
	}
}

void GameLevel::Load(const std::string& filePath, unsigned int levelWidth, unsigned int levelHeight)
{
	Bricks.clear();

	std::ifstream fstream(filePath);

	if (!fstream)
	{
		std::cout << "Failed to initialize level: " << filePath << '\n';
		return;
	}

	Tiles tiles;
	unsigned int tileCode;
	std::string line;

	while (std::getline(fstream, line))
	{
		std::istringstream sstream(line);
		std::vector<unsigned int> row;

		while (sstream >> tileCode)
		{
			row.push_back(tileCode);
		}

		tiles.push_back(row);
	}

	if (!tiles.empty())
	{
		Init(tiles, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : Bricks)
	{
		if (!tile.Destroyed)
		{
			tile.Draw(renderer);
		}
	}
}

bool GameLevel::IsCompleted() const
{
	return std::all_of(std::begin(Bricks), std::end(Bricks), [](const GameObject& tile) {
		return tile.Solid || tile.Destroyed;
	});
}
