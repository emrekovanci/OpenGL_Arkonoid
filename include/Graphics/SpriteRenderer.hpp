#pragma once

#include "Graphics/Texture2D.hpp"
#include "Graphics/Shader.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class SpriteRenderer
{
public:
	explicit SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader _shader;
	unsigned int _vao;
	void initRenderData();
};