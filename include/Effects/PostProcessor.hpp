#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Texture2D.hpp"

class PostProcessor
{
public:
	PostProcessor(const Shader& shader, unsigned int width, unsigned int height);
	void BeginRender();
	void EndRender();
	void Render(float time);

	bool Confuse { false };
	bool Chaos { false };
	bool Shake { false } ;

private:
	Shader _shader;
	unsigned int _width;
	unsigned int _height;

	Texture2D _fboTexture;
	unsigned int _msfbo;
	unsigned int _fbo;
	unsigned int _rbo;
	unsigned int _vao;

	void initRenderData();
};