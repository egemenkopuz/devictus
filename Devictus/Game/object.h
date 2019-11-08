#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <vector>

#include "../Engine/texture.h"
#include "../Engine/shader.h"
#include "../Engine/model.h"

class GameObject {
public:
	std::string type;
	glm::vec3 position;
	bool destructable;
	bool available;

	Model * model;
	~GameObject();

	virtual void transform() = 0;
};

class Player : public GameObject {
public:
	int health;
	int stamina;
	bool casting;

	Player(glm::vec3 position);
	void transform() override;
};

class Enemy : public GameObject {
public:
	int health;
	bool casting;

	Enemy(glm::vec3 position);
	void transform() override;
};

class Block : public GameObject {	
public:
	Block(glm::vec3 position, Model * model, bool indestructable);
	void transform() override;
};

#endif