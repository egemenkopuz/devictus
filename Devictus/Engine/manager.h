#ifndef MANAGER_H
#define MANAGER_H

#include <map>
#include <string>

#include "../Engine/shader.h"
#include "../Engine/texture.h"

class Manager {
public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture> textures;

	static Shader loadShader(const char *vertexPath, const char *fragmentPath, std::string name);
	static Shader getShader(std::string name);

	static Texture loadTexture(const char *texturePath, std::string name, std::string type);
	static Texture getTexture(std::string name);

	static void clear();
private:
	Manager() = default;
	
	static Shader loadShaderFromFile(const char *vertexPath, const char *fragmentPath);
	static Texture loadTextureFromFile(const char *texturePath, std::string type);
};

#endif