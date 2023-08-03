#pragma once

#include <glm/vec2.hpp>

#include "Ball.hpp"
#include "GameObject.hpp"

struct Collision
{
	enum class Direction { Up, Right, Down, Left };

	bool Collided;
	Direction Dir;
	glm::vec2 Diff;
};

Collision::Direction GetCollisionDirection(glm::vec2 target);
glm::vec2 GetCollisionNormal(Collision::Direction direction);
Collision CheckCollision(Ball& ball, GameObject& other);
