#pragma once

#include "GameObject.hpp"

class Ball : public GameObject
{
public:
	Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
	glm::vec2 Move(float deltaTime, unsigned int windowWidth);
	void Reset(glm::vec2 position, glm::vec2 velocity);

	float Radius { 12.5f };
	bool Stuck { true };
};