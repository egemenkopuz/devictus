#ifndef SHADOW_H
#define SHADOW_H
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class ShadowMap {
private:
	unsigned int FBO;
	unsigned int depthMap;
public:
	float near_plane = 1.0f;
	float far_plane = 5.f;

	ShadowMap() = default;
	~ShadowMap() = default;

	void init(unsigned int width = 1024, unsigned int height = 1024);
	void bindForWriting();
	void bindForReading(GLenum textureUnit);
};


#endif