#include "manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbImage/stb_image.h>

#include <iostream>
#include <sstream>
#include <fstream>

// Instantiate static variables
std::map<std::string, Texture>    Manager::textures;
std::map<std::string, Shader>       Manager::shaders;

Shader Manager::loadShader(const char * vertexPath, const char * fragmentPath, std::string name)
{
	shaders[name] = loadShaderFromFile(vertexPath, fragmentPath);
	return shaders[name];
}

Shader Manager::getShader(std::string name)
{
	return shaders[name];
}

Texture Manager::loadTexture(const char * texturePath, std::string name, std::string type)
{
	textures[name] = loadTextureFromFile(texturePath, type);
	return textures[name];
}

Texture Manager::getTexture(std::string name)
{
	return textures[name];
}

void Manager::clear()
{
	for (auto iter : shaders)
		glDeleteProgram(iter.second.ID);
	for (auto iter : textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader Manager::loadShaderFromFile(const char * vertexPath, const char * fragmentPath)
{
	std::string vertCode;
	std::string fragCode;

	try {
		std::ifstream vertShaderFile(vertexPath);
		std::ifstream fragShaderFile(fragmentPath);
		std::stringstream vertShaderStream, fragShaderStream;

		vertShaderStream << vertShaderFile.rdbuf();
		fragShaderStream << fragShaderFile.rdbuf();

		vertShaderFile.close();
		fragShaderFile.close();

		vertCode = vertShaderStream.str();
		fragCode = fragShaderStream.str();
	}
	catch (std::exception e) {
		std::cout << "ERROR, Shader failed to load at path: " << vertexPath << " or " << fragmentPath << std::endl;
	}

	const char *vShaderCode = vertCode.c_str();
	const char *fShaderCode = fragCode.c_str();

	Shader shader;
	shader.compile(vShaderCode, fShaderCode);
	return shader;
}

Texture Manager::loadTextureFromFile(const char * texturePath, std::string type)
{
	Texture texture;

	int width, height, nrComponents;
	unsigned char *data = stbi_load(texturePath, &width, &height, &nrComponents, 0);

	if (data) {
		if (nrComponents == 1)
			texture.format = GL_RED;
		else if (nrComponents == 3)
			texture.format = GL_RGB;
		else if (nrComponents == 4)
			texture.format = GL_RGBA;

		texture.generate(width, height, data, type);
	}
	else std::cout << "ERROR, Texture failed to load at path: " << texturePath << std::endl;

	stbi_image_free(data);

	return texture;
}
