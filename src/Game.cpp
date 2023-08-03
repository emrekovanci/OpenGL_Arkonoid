#include "Game.hpp"

#include "ResourceManager.hpp"

#include "Physics/Collision.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Game::Game(unsigned int width, unsigned int height) :
	_width { width },
	_height { height }
{ }

void Game::Init()
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(_width), static_cast<float>(_height), 0.0f, -1.0f, 1.0f);

	// in-game textures
	ResourceManager::LoadTexture("resources/textures/background.jpg", "background");
	ResourceManager::LoadTexture("resources/textures/awesomeface.png", "face");
	ResourceManager::LoadTexture("resources/textures/block.png", "block");
	ResourceManager::LoadTexture("resources/textures/block_solid.png", "block_solid");
	ResourceManager::LoadTexture("resources/textures/paddle.png", "paddle");
	ResourceManager::LoadTexture("resources/textures/particle.png", "particle");

	// renderer
	Shader& spriteShader = ResourceManager::LoadShader("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl", "sprite");
	spriteShader.Use();
	spriteShader.SetInt("image", 0);
	spriteShader.SetMat4("projection", projection);
	_renderer = std::make_unique<SpriteRenderer>(spriteShader);

	// particle shader
	Shader& particleShader = ResourceManager::LoadShader("resources/shaders/particle/vertex.glsl", "resources/shaders/particle/fragment.glsl", "particle");
	particleShader.Use();
	particleShader.SetInt("sprite", 0);
	particleShader.SetMat4("projection", projection);
	_particleGenerator = std::make_unique<ParticleGenerator>(particleShader, ResourceManager::GetTexture("particle"), 500);

	// post-process shader
	Shader& postProcessShader = ResourceManager::LoadShader("resources/shaders/postprocess/vertex.glsl", "resources/shaders/postprocess/fragment.glsl", "postprocessing");
	_postProcess = std::make_unique<PostProcessor>(postProcessShader, _width, _height);

	// levels
	for (int i = 0; i < _levelCount; ++i)
	{
		GameLevel level;
		level.Load("resources/levels/" + std::to_string(i + 1) + ".lvl", _width, _height / 2);
		_levels.emplace_back(level);
	}

	// game objects
	glm::vec2 playerPos = glm::vec2(_width * 0.5f - PlayerSize.x * 0.5f, _height - PlayerSize.y);
	_player = std::make_unique<GameObject>(playerPos, PlayerSize, ResourceManager::GetTexture("paddle"));

	glm::vec2 ballPos = playerPos + glm::vec2(PlayerSize.x * 0.5f - BallRadius, -BallRadius * 2.0f);
	_ball = std::make_unique<Ball>(ballPos, BallRadius, InitialBallVelocity, ResourceManager::GetTexture("face"));
}

void Game::ProcessInput(float deltaTime)
{
	if (_state != GameState::Active)
	{
		return;
	}

	float velocity = PlayerVelocity * deltaTime;

	if (_keys[GLFW_KEY_A] || _keys[GLFW_KEY_LEFT])
	{
		_player->Position.x = glm::max(_player->Position.x - velocity, 0.0f);
		if (_ball->Stuck)
		{
			_ball->Position.x = glm::max(_ball->Position.x - velocity, 0.0f);
		}
	}

	if (_keys[GLFW_KEY_D] || _keys[GLFW_KEY_RIGHT])
	{
		_player->Position.x = glm::min(_player->Position.x + velocity, _width - _player->Size.x);
		if (_ball->Stuck)
		{
			_ball->Position.x = glm::min(_ball->Position.x + velocity, _width - _ball->Radius * 2.0f);
		}
	}

	if (_keys[GLFW_KEY_SPACE])
	{
		_ball->Stuck = false;
	}
}

void Game::Update(float deltaTime)
{
	_ball->Move(deltaTime, _width);

	CheckCollisions();

	_particleGenerator->Update(deltaTime, *_ball, 2, glm::vec2(_ball->Radius * 0.5f));

	_shakeTime = glm::max(0.0f, _shakeTime - deltaTime);
	if (_shakeTime <= 0.0f)
	{
		_postProcess->Shake = false;
	}

	if (_ball->Position.y >= _height)
	{
		ResetLevel();
		ResetPlayer();
	}
}

void Game::Render()
{
	if (_state == GameState::Active)
	{
		_postProcess->BeginRender();
		_renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(_width, _height), 0.0f);
		_levels[_currentLevel].Draw(*_renderer);
		_player->Draw(*_renderer);
		_particleGenerator->Draw();
		_ball->Draw(*_renderer);
		_postProcess->EndRender();
		_postProcess->Render(static_cast<float>(glfwGetTime()));
	}
}

void Game::CheckCollisions()
{
	for (GameObject& brick : _levels[_currentLevel].Bricks)
	{
		if (!brick.Destroyed)
		{
			ProcessBallBrickCollision(brick);
		}
	}

	ProcessBallPlayerCollision();
}

void Game::ProcessBallBrickCollision(GameObject& brick)
{
	Collision collision = CheckCollision(*_ball, brick);

	if (collision.Collided)
	{
		if (!brick.Solid)
		{
			brick.Destroyed = true;
		}
		else
		{
			_shakeTime = 0.05f;
			_postProcess->Shake = true;
		}

		glm::vec2 normal = GetCollisionNormal(collision.Dir);
		_ball->Velocity = glm::reflect(_ball->Velocity, normal);

		glm::vec2 penetration = (_ball->Radius - glm::length(collision.Diff)) * normal;
		_ball->Position -= penetration;
	}
}

void Game::ProcessBallPlayerCollision()
{
	Collision result = CheckCollision(*_ball, *_player);
	if (!_ball->Stuck && result.Collided)
	{
		float centerBoard = _player->Position.x + _player->Size.x * 0.5f;
		float distance = (_ball->Position.x + _ball->Radius) - centerBoard;
		float percentage = distance / (_player->Size.x * 0.5f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = _ball->Velocity;
		_ball->Velocity.x = InitialBallVelocity.x * percentage * strength;
		_ball->Velocity = glm::normalize(_ball->Velocity) * glm::length(oldVelocity);
		_ball->Velocity.y = -1.0f * abs(_ball->Velocity.y);
	}
}

void Game::ResetLevel()
{
	_levels[_currentLevel].Load("resources/levels/" + std::to_string(_currentLevel + 1) + ".lvl", _width, _height / 2);
}

void Game::ResetPlayer()
{
	_player->Size = PlayerSize;
	_player->Position = glm::vec2(_width * 0.5f - PlayerSize.x, _height - PlayerSize.y);
	_ball->Reset(_player->Position + glm::vec2(PlayerSize.x * 0.5f - BallRadius, -BallRadius * 2.0f), InitialBallVelocity);
}

void Game::SetInput(int key, bool value)
{
	if (key >= 0 && key < 1024)
	{
		_keys[key] = value;
	}
}