#include "shader.h"

#include <iostream>

Shader &Shader::use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::compile(const char * vertShaderCode, const char * fragShaderCode)
{
	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);

	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::setBool(const std::string & name, bool value, bool useShader) 
{
	if (useShader) this->use();
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value, bool useShader) 
{
	if (useShader) this->use();
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string & name, float value, bool useShader) 
{
	if (useShader) this->use();
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value, bool useShader) 
{
	if (useShader) this->use();
	glUniform2fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y, bool useShader) 
{
	if (useShader) this->use();
	glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value, bool useShader) 
{
	if (useShader) this->use();
	glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z, bool useShader) 
{
	if (useShader) this->use();
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value, bool useShader) 
{
	if (useShader) this->use();
	glUniform4fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w, bool useShader) 
{
	if (useShader) this->use();
	glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat, bool useShader) 
{
	if (useShader) this->use();
	glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat, bool useShader) 
{
	if (useShader) this->use();
	glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat, bool useShader) 
{
	if (useShader) this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
}