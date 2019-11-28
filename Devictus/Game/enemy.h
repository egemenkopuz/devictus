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

	bool checkPhase();
public:
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