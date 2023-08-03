#include "Effects/ParticleGenerator.hpp"

#include <random>

ParticleGenerator::ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount) :
	_shader { shader },
	_texture { texture },
	_amount { amount }
{
	init();
}

void ParticleGenerator::Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset)
{
	for (unsigned int i = 0; i < newParticles; ++i)
	{
		int unusedParticle = firstUnusedParticle();
		respawnParticle(_particles[unusedParticle], object, offset);
	}

	for (unsigned int i = 0; i < _amount; ++i)
	{
		Particle& p = _particles[i];
		p.Life -= deltaTime;
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * deltaTime;
			p.Color.a -= deltaTime * 2.5f;
		}
	}
}

void ParticleGenerator::Draw() const
{
	// additive blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	_shader.Use();

	for (const Particle& particle : _particles)
	{
		if (particle.Life > 0.0f)
		{
			_shader.SetVec2("offset", particle.Position);
			_shader.SetVec4("color", particle.Color);
			_texture.Bind();

			glBindVertexArray(_vao);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	unsigned int vbo;

	float particleQuad[] =
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < _amount; ++i)
	{
		_particles.push_back(Particle());
	}
}

unsigned int ParticleGenerator::firstUnusedParticle()
{
	for (unsigned int i = _lastUsedParticle; i < _amount; ++i)
	{
		if (_particles[i].Life <= 0.0f)
		{
			_lastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < _lastUsedParticle; ++i)
	{
		if (_particles[i].Life <= 0.0f)
		{
			_lastUsedParticle = i;
			return i;
		}
	}

	_lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
	static std::default_random_engine generator;
	std::uniform_real_distribution<float> distPosition(-5.0f, 5.0f);
	std::uniform_real_distribution<float> distColor(0.5f, 1.0f);

	float randomPosition = distPosition(generator);
	float randomColor = distColor(generator);

	particle.Position = object.Position + randomPosition + offset;
	particle.Color = glm::vec4(randomColor, randomColor, randomColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}
