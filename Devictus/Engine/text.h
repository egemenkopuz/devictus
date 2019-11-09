#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "manager.h"
#include "Shader.h"

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
	const char * shaderName;
	TextRenderer(const char * shaderName);
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
};


#endif