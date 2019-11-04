#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <vector>

class Texture {
public:
	unsigned int ID;
	unsigned int width, height;
	std::string path;
	std::string type;
	GLenum format;
	Texture();
	void generate(unsigned int width, unsigned int height, unsigned char *data, std::string type);
	void bind() const;
};

#endif