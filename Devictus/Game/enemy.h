#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"
#include "projectile.h"
#include <cstdlib>
#include <ctime>

class Enemy : public GameObject {
private:
	float health = 100.f;

	bool invinsible = false;
	bool onlyMelee = false;

	bool randomFlag = false;

	Player * player;
	Model * projectileModel;

	float dX = 0.f;
	float dZ = 0.f;

	// cooldown between moves
	float firstCD = 0.f;
	float secondCD = 0.f;
	float thirdCD = 0.f;

	// probabilities
	unsigned int firstP = 0.f;
	unsigned int secondP = 0.f;
	unsigned int thirdP = 0.f;

	// time intervals between projectiles
	float firstMoveInterval = 0.f;
	float secondMoveInterval = 0.f;
	float thirdMoveInterval = 0.f;

	// index of attack type
	int firstMoveType = -1;
	int secondMoveType = -1;
	int thirdMoveType = -1;

	// remaining repetetive counts of attacks
	int firstMoveCount = 0;
	int secondMoveCount = 0;
	int thirdMoveCount = 0;

	// active attacks
	//bool moveSet[6];

	// current phase
	int phase = 4;

	// phase functions
	void phase1(float deltaTime);
	void phase2(float deltaTime);
	void phase3(float deltaTime);
	void phase4(float deltaTime);

	bool phaseChange();

	bool exploded = false;
public:

	void decreaseLifeWithMelee(float l) { if (!invinsible) health -= l; }
	int getPhase() { return phase; }
	int isExploded() { return exploded; }

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