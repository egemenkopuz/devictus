#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <vector>

#include "../Engine/texture.h"
#include "../Engine/shader.h"
#include "../Engine/mesh.h"

class GameObject {
public:
	glm::vec3 position;
	bool gravityApplied;
	bool destroyed;
	int health;

private:
	std::vector<Mesh> meshes;
	std::vector<GameObject> children;

	virtual void transform() = 0;
};

class Player : public GameObject {
public:
	bool casting;
	int stamina;

	Player();

private:
	virtual void transform();
};

class Enemy : public GameObject {
public:
	bool casting;
	int stamina;

	Enemy();
private:
	virtual void transform();
};

class Block : public GameObject {
public:

	Block();
private:
	virtual void transform();
};

#endif