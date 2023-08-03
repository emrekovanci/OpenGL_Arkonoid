#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

struct Particle
{
	glm::vec2 Position { 0.0f, 0.0f };
	glm::vec2 Velocity { 0.0f, 0.0f };
	glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
	float Life { 0.0f };
};