#include "particle.h"

void ParticleGenerator::init()
{
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (unsigned int i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

unsigned int ParticleGenerator::getFirst()
{
	for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].lifeTime <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].lifeTime <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::update(float dt, GameObject & object, unsigned nAmount, glm::vec3 offset)
{
	// Add new particles 
	for (GLuint i = 0; i < nAmount; ++i)
	{
		int unusedParticle = this->getFirst();
		this->respawn(this->particles[unusedParticle], object, offset);
	}
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle &p = this->particles[i];
		p.lifeTime -= dt; // reduce life
		if (p.lifeTime > 0.0f)
		{	// particle is alive, thus update
			p.position -= p.velocity * dt;
			p.color.a -= dt * 2.5;
		}
	}
}

void ParticleGenerator::respawn(Particle & particle, GameObject & object, glm::vec3 offset)
{
	unsigned int random = ((rand() % 100) - 50) / 10.0f;
	float randomColor = 0.5 + ((rand() % 100) / 100.0f);

	particle.position = object.getPosition() + glm::vec3(random) + offset;
	particle.color = glm::vec4(glm::vec3(randomColor), 1.f);
	particle.lifeTime = 1.f;
	particle.velocity = glm::vec3(0.f);
}

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount)
	: shader(shader), texture(texture), amount(amount)
{
	init();
}

void ParticleGenerator::draw()
{
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.use();
	for (Particle particle : this->particles)
	{
		if (particle.lifeTime > 0.0f)
		{
			this->shader.setVec3("offset", particle.position);
			this->shader.setVec4("color", particle.color);
			this->texture.bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
