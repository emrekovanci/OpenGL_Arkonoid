#pragma once

#include <glad/glad.h>

struct Texture2D
{
	Texture2D();
	void Generate(unsigned int w, unsigned int h, unsigned char* data);
	void Bind() const;

	unsigned int Id { };

	unsigned int Width { 0 };
	unsigned int Height { 0 };

	unsigned int InternalFormat { GL_RGB };
	unsigned int ImageFormat { GL_RGB };

	unsigned int WrapS { GL_REPEAT };
	unsigned int WrapT { GL_REPEAT };
	unsigned int FilterMin { GL_LINEAR };
	unsigned int FilterMax { GL_LINEAR };
};