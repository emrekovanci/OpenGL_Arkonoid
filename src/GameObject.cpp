#include "GameObject.hpp"

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) :
	Position { position },
	Size { size },
	Sprite { sprite },
	Color { color },
	Velocity { velocity }
{
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(Sprite, Position, Size, Rotation, Color);
}
