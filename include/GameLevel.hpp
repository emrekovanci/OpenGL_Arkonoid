#pragma once

#include <vector>

#include "Graphics/SpriteRenderer.hpp"
#include "GameObject.hpp"

class GameLevel
{
	using Tiles = std::vector<std::vector<unsigned int>>;

private:
	void Init(Tiles tiles, unsigned int levelWidth, unsigned int levelHeight);

public:
	void Load(const std::string& filePath, unsigned int levelWidth, unsigned int levelHeight);
	void Draw(SpriteRenderer& renderer);
	bool IsCompleted() const;

	std::vector<GameObject> Bricks;
};