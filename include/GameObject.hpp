#pragma once

#include "Graphics/Texture2D.hpp"
#include "Graphics/SpriteRenderer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class GameObject
{
public:
	GameObject() = default;
	GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));
	virtual ~GameObject() = default;

	virtual void Draw(SpriteRenderer& renderer);

	glm::vec2 Position { 0.0f, 0.0f };
	glm::vec2 Size { 1.0f, 1.0f };
	Texture2D Sprite { };
	glm::vec3 Color { 1.0f, 1.0f, 1.0f };
	glm::vec2 Velocity { 0.0f, 0.0f };

	float Rotation	{ 0.0f };
	bool Solid { false };
	bool Destroyed { false };
};