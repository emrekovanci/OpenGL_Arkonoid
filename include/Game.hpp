#pragma once

#include "GameLevel.hpp"
#include "Ball.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Effects/ParticleGenerator.hpp"
#include "Effects/PostProcessor.hpp"

#include <memory>

enum class GameState
{
	Active,
	Menu,
	Win
};

constexpr glm::vec2 PlayerSize { 100.0f, 20.0f };
constexpr float PlayerVelocity { 500.0f };

constexpr glm::vec2 InitialBallVelocity { 100.0f, -350.0f };
constexpr float BallRadius { 12.5f };

class Game
{
private:
	void CheckCollisions();
	void ProcessBallBrickCollision(GameObject& brick);
	void ProcessBallPlayerCollision();

public:
	Game(unsigned int width, unsigned int height);

	void Init();
	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();

	void ResetLevel();
	void ResetPlayer();

	void SetInput(int key, bool value);

private:
	GameState _state { GameState::Active };
	bool _keys[1024] { false };
	unsigned int _width { 800 };
	unsigned int _height { 600 };

	std::vector<GameLevel> _levels;
	std::size_t _currentLevel { 0 };
	const std::size_t _levelCount { 4 };

	std::unique_ptr<GameObject> _player;
	std::unique_ptr<Ball> _ball;
	std::unique_ptr<SpriteRenderer> _renderer;
	std::unique_ptr<ParticleGenerator> _particleGenerator;
	std::unique_ptr<PostProcessor> _postProcess;

	float _shakeTime { 0.0f };
};