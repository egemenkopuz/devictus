#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
	unsigned int ID;
	Shader() = default;
	Shader &use();
	void compile(const char *v, const char *f);
	void setBool(const std::string &name, bool value, bool useShader = false);
	void setInt(const std::string &name, int value, bool useShader = false);
	void setFloat(const std::string &name, float value, bool useShader = false);
	void setVec2(const std::string &name, const glm::vec2 &value, bool useShader = false);
	void setVec2(const std::string &name, float x, float y, bool useShader = false);
	void setVec3(const std::string &name, const glm::vec3 &value, bool useShader = false);
	void setVec3(const std::string &name, float x, float y, float z, bool useShader = false);
	void setVec4(const std::string &name, const glm::vec4 &value, bool useShader = false);
	void setVec4(const std::string &name, float x, float y, float z, float w, bool useShader = false);
	void setMat2(const std::string &name, const glm::mat2 &mat, bool useShader = false);
	void setMat3(const std::string &name, const glm::mat3 &mat, bool useShader = false);
	void setMat4(const std::string &name, const glm::mat4 &mat, bool useShader = false);
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif