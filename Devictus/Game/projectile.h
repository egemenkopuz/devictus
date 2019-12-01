#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "object.h"

enum ProjectileType {
	BULLET,
	AOE,
	RANDOM,
	SCISSOR,
	HOMING
	// TODO MAYBE RAY, help from 3rdperson camera
};

enum ProjectileEffect {
	ATTACKER, 
	PUSHER, 
	PULLER,
};

class Projectile : public GameObject {
private:
	float xSpeed;
	float ySpeed;
	float zSpeed;

	float lifeTime;

	bool envReactive;

	ProjectileType pType;
	ProjectileEffect pEffect;

	glm::vec3 target;
public:
	void updateTarget(glm::vec3 t) { target = t; }
	bool isOver() { return lifeTime > 0.f ? false : true; }
	void end() { lifeTime = -1.f; }
	ProjectileEffect getEffect() { return pEffect; }
	ProjectileType getProjectileType() { return pType; }
	void setLifeTime(float newLifeTime) { lifeTime = newLifeTime; }
	bool isEnvReactive() { return envReactive; }

	void move(float deltaTime);

	Projectile(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model, 
		ProjectileType pType, ProjectileEffect pEffect, bool effectEnv, float xSpeed, float ySpeed, float zSpeed);
	bool increaseLife(float v);
	bool decreaseLife(float v);
	bool isAvailable();
	float getLife() { return lifeTime; }
};

#endif