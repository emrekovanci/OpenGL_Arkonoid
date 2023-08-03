#include "ResourceManager.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const std::string& name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const std::string& file, const std::string& name)
{
	Textures[name] = loadTextureFromFile(file.c_str());
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string& name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (const auto& shaderIterator : Shaders)
	{
		glDeleteProgram(shaderIterator.second.GetProgramId());
	}

	for (const auto& textureIterator : Textures)
	{
		glDeleteTextures(1, &textureIterator.second.Id);
	}
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode, fragmentCode, geometryCode;

    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files\n";
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // 2. now create shader object from source code
    Shader shader{};
    shader.Compile(vShaderCode, fShaderCode);
    return shader;
}

#include <iostream>

Texture2D ResourceManager::loadTextureFromFile(const char* file)
{
	Texture2D texture;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    if (nrChannels == 4)
    {
        texture.InternalFormat = GL_RGBA;
        texture.ImageFormat = GL_RGBA;
    }

	texture.Generate(width, height, data);
	stbi_image_free(data);

	return texture;
}
