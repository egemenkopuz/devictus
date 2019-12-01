#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "projectile.h"

struct Trail {
	glm::mat4 model;
	float lifeTime;

	Trail(glm::mat4 m, float t) : model(m), lifeTime(t) {}
};

class Player : public GameObject {
private:
	float health = 100.f;
	float stamina = 100.f;

	bool damaged = false;
	float damagedTime = 0.f;

	float forcedMovTime;
	bool beingPulled;
	bool beingPushed;
	glm::vec3 pushingV;
	glm::vec3 pullingV;

	bool melee = false;
	bool attacked = false;
	float meleeBarrier = 0.f;

	bool casting = false;
	float castBarrier = 0.f;

	bool jumping = false;
	bool dashed = false;

	float dashBoost = 1.f;
	float dashCooldown = 0.f;

	bool invisibility = false; 
	float invisFrame = 0.f;

	float prevDeltaTime = 0.f;

	float currentWalkingSpeedZ;
	float currentWalkingSpeedX;
	float currentJumpSpeed;
	float currentTurnSpeed;

	const float WALKING_SPEED = 1.2f;
	const float TURNNING_SPEED = 5.f;
	const float GRAVITY = -1.f;
	const float JUMPING_SPEED = 2.5f;

	const float JUMP_MAX = 1.f;
	float CURRENT_JUMP = 0.f;
	
	Model * projectileModel;
public:
	std::vector<Trail*> trails;
	bool isOnGround = false;

	std::vector<Projectile*> projectiles;
	void attachProjectileModel(Model * model);
	glm::vec3 target;

	float getCurrentWalkingSpeedZ() { return currentWalkingSpeedZ; }
	float getCurrentWalkingSpeedX() { return currentWalkingSpeedX; }
	float getCurrentJumpSpeed() { return currentJumpSpeed; }

	float LIMIT = -2.f;

	void move(bool keys[], float deltaTime);

	void rotate(float xoffset, float yoffset, float deltaTime);

	Player(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model);

	bool isInvisible() { return invisibility; }
 
	bool increaseLife(float v);
	bool decreaseLife(float v);
	bool isAvailable();
	float getLife() { return health; }
	float getStamina() { return stamina; }
	bool doingMeleeAttack() { return melee; }

	bool canMeleeAttack() { return !attacked; }
	bool canRangedAttack() { return !casting; }
	bool canDash() { return !dashed; }

	bool isPushed() { return beingPushed; }
	bool isPulled() { return beingPulled; }
		
	bool push(glm::vec3 v);
	bool pull(glm::vec3 v);

	bool isDamaged() { return damaged; }
 };


#endif