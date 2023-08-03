#pragma once

#include <map>
#include <string>

#include "Graphics/Shader.hpp"
#include "Graphics/Texture2D.hpp"

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const std::string& name);
	static Shader GetShader(const std::string& name);

	static Texture2D LoadTexture(const std::string& file, const std::string& name);
	static Texture2D GetTexture(const std::string& name);

	static void Clear();

private:
	ResourceManager() = default;

	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
	static Texture2D loadTextureFromFile(const char* file);
};