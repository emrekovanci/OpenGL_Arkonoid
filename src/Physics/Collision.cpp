#include "Physics/Collision.hpp"

#include <vector>

Collision::Direction GetCollisionDirection(glm::vec2 target)
{
	std::vector<glm::vec2> compass
	{
		{ 0.0f, 1.0f },  // up
		{ 1.0f, 0.0f },  // right
		{ 0.0f, -1.0f }, // down
		{ -1.0f, 0.0f }  // left
	};

	auto dotProductComparer = [&target](const glm::vec2& a, const glm::vec2& b) {
		return glm::dot(glm::normalize(target), a) < glm::dot(glm::normalize(target), b);
	};

	auto maxDirection = std::max_element(std::begin(compass), std::end(compass), dotProductComparer);
	std::size_t index = std::distance(std::begin(compass), maxDirection);

	return static_cast<Collision::Direction>(index);
}

glm::vec2 GetCollisionNormal(Collision::Direction direction)
{
	switch (direction)
	{
		case Collision::Direction::Up: return glm::vec2(0.0f, 1.0f); break;
		case Collision::Direction::Right: return glm::vec2(1.0f, 0.0f); break;
		case Collision::Direction::Down: return glm::vec2(0.0f, -1.0f); break;
		case Collision::Direction::Left: return glm::vec2(-1.0f, 0.0f); break;
	}

	return glm::vec2(0.0f, 0.0f);
}

Collision CheckCollision(Ball& ball, GameObject& other)
{
	glm::vec2 center(ball.Position + ball.Radius);

	glm::vec2 aabb_half_extents(other.Size.x * 0.5f, other.Size.y * 0.5f);
	glm::vec2 aabb_center(other.Position.x + aabb_half_extents.x, other.Position.y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;
	glm::vec2 closestDiff = closest - center;

	return glm::length(closestDiff) <= ball.Radius
		? Collision { true, GetCollisionDirection(closestDiff), closestDiff }
		: Collision { false, Collision::Direction::Up, glm::vec2(0.0f, 0.0f) };
}
