#include "Ball.hpp"

#include "Graphics/Color.hpp"

Ball::Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) :
	GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, Color::White.Rgb, velocity),
	Radius { radius },
	Stuck { true }
{

}

glm::vec2 Ball::Move(float deltaTime, unsigned int windowWidth)
{
	if (!Stuck)
	{
		Position += Velocity * deltaTime;

		if (Position.x <= 0.0f)
		{
			Velocity.x = -Velocity.x;
			Position.x = 0.0f;
		}
		else if (Position.x + Size.x >= windowWidth)
		{
			Velocity.x = -Velocity.x;
			Position.x = windowWidth - Size.x;
		}
		else if (Position.y <= 0.0f)
		{
			Velocity.y = -Velocity.y;
			Position.y = 0.0f;
		}
	}

	return Position;
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
	Position = position;
	Velocity = velocity;
}
