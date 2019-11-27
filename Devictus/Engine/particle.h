#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "texture.h"
#include "../Game/object.h"

struct Particle {
	glm::vec3 position, velocity;
	glm::vec4 color;
	float lifeTime;
	Particle() : position(0.f), velocity(0.f), color(1.f), lifeTime(0.f) {}
};

class ParticleGenerator {
private:
	std::vector<Particle> particles;
	unsigned int amount;
	Shader shader;
	Texture texture;

	unsigned int  VAO;

	void init();
	unsigned int lastUsedParticle = 0;
	unsigned int getFirst();
	void update(float dt, GameObject &object, unsigned nAmount, glm::vec3 offset);
	void respawn(Particle &particle, GameObject &object, glm::vec3 offset = glm::vec3(0.f));
public:
	ParticleGenerator(Shader shader, Texture texture, unsigned int amount);
	void draw();
};

#endif