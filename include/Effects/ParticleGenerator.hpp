#pragma once

#include "Particle.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/Texture2D.hpp"
#include "GameObject.hpp"

#include <vector>

class ParticleGenerator
{
public:
	ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount);
	void Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw() const;

private:
	std::vector<Particle> _particles;
	unsigned int _amount;
	Shader _shader;
	Texture2D _texture;
	unsigned int _vao;
	unsigned int _lastUsedParticle { 0 };

private:
	void init();
	unsigned int firstUnusedParticle();
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};