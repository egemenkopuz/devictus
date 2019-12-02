#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"
#include "projectile.h"
#include <cstdlib>
#include <ctime>

class Enemy : public GameObject {
private:
	float health;
	int castType = -1;
	int effectType = -1;

	bool randomFlag = false;

	Player * player;
	Model * projectileModel;

	float castBarrier = 0.f;

	float dX = 0.f;
	float dZ = 0.f;

	bool firstInit = true;
	int cons = 0;

	int phase = 1;

	void phase1(float deltaTime);
	void phase2(float deltaTime);
	void phase3(float deltaTime);
	void phase4(float deltaTime);

public:
	int getPhase() { return phase; }

	std::vector<Projectile*> projectiles;
	void attachProjectileModel(Model * model);

	void action(float deltaTime);
	void attachPlayer(Player * player);
	Enemy(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model);
	
	bool increaseLife(float v);
	bool decreaseLife(float v);
	bool isAvailable();
	float getLife() { return health; }
};

#endif