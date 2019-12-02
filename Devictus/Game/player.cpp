#include "player.h"

void Player::attachProjectileModel(Model * model)
{
	this->projectileModel = model;
}

void Player::move(bool keys[], float deltaTime)
{
	prevDeltaTime = deltaTime;
	
	if (damagedTime < deltaTime)
	{
		damaged = false;
	}
	else damagedTime -= deltaTime;

	// stamina buffer
	stamina += 10 * deltaTime;
	if (stamina >= 100.0f)
		stamina = 100.f;

	// inits
	currentJumpSpeed = 0.f;
	currentWalkingSpeedX = 0.f;
	currentWalkingSpeedZ = 0.f;

	// jumping
	if (!jumping) CURRENT_JUMP = 0.f;

	if (keys[MOVE_UP] == true && !jumping && currentAABB.isCollided() && isOnGround == true)
	{
		currentJumpSpeed = JUMPING_SPEED;
		jumping = true;
	}
	if (keys[MOVE_DOWN])
	{
		//potentialY += -JUMPING_SPEED;
	}

	// dashing
	if (invisFrame < deltaTime)
	{
		invisibility = false;
		dashBoost = 1.f;
	}
	else
	{
		invisFrame -= deltaTime;
		Trail * trail = new Trail(transformMatrix, invisFrame);
		trails.push_back(trail);
	}

	if (dashCooldown < deltaTime)
	{
		dashed = false;
	}
	else dashCooldown -= deltaTime;

	if (keys[MOVE_SHIFT] && !invisibility && !dashed && !invisibility && stamina >= 30.f)
	{
		dashBoost = 3.f;
		invisFrame = 10.f * deltaTime;
		dashCooldown = 15.f * deltaTime;
		invisibility = true;
		dashed = true;
		stamina -= 30.f;
	}

	if (forcedMovTime < deltaTime)
	{
		beingPulled = false;
		beingPushed = false;
	}
	else forcedMovTime -= deltaTime;

	// movements
	if (beingPushed || beingPulled)
	{
		// forced movement
		if (beingPulled)
		{
			currentWalkingSpeedX = pullingV.x * 5.f;
			currentWalkingSpeedZ = pullingV.z * 5.f;
		}
		else
		{
			currentWalkingSpeedX = pushingV.x * 10.f;
			currentWalkingSpeedZ = pushingV.z * 10.f;
		}
	}
	else
	{
		// normal user's movement
		if (keys[MOVE_FORWARD] == true)
		{
			currentWalkingSpeedX = WALKING_SPEED * dashBoost;
		}
		if (keys[MOVE_BACKWARD] == true)
		{
			currentWalkingSpeedX = -WALKING_SPEED * dashBoost;
		}

		if (keys[MOVE_LEFT] == true)
		{
			currentWalkingSpeedZ = -WALKING_SPEED * dashBoost;
		}
		if (keys[MOVE_RIGHT] == true)
		{
			currentWalkingSpeedZ = WALKING_SPEED * dashBoost;
		}
	}

	// ranged attack
	if (castBarrier < deltaTime)
	{
		casting = false;
	}
	else castBarrier -= deltaTime;

	if (keys[MOVE_MOUSE_LEFT] == true && !casting)
	{
		if (aimAssist)
		{
			glm::vec3 diff = glm::normalize(target - position) *7.f;
			Projectile * p = new Projectile(position, 0.f, glm::vec3(0.1f), projectileModel, BULLET, ATTACKER, true, diff.x, diff.y, diff.z);
			p->makeTrails = true;
			projectiles.push_back(p);
		}
		else
		{
			glm::vec3 v = glm::normalize(target - position);
			float offsetX = 1.f * sin(rotationDegree);
			float offsetZ = 1.f * cos(rotationDegree);

			v.x = offsetX;
			v.z = offsetZ;
			
			v *= 7.f;
			
			Projectile * p = new Projectile(position, 0.f, glm::vec3(0.1f), projectileModel, BULLET, ATTACKER, true, v.x, v.y, v.z);
			p->makeTrails = true;
			projectiles.push_back(p);
		}

		castBarrier = deltaTime * 30.f;
		casting = true;
	}

	// melee attack
	if (meleeBarrier < deltaTime)
	{
		attacked = false;
	}
	else meleeBarrier -= deltaTime;
	
	if (keys[MOVE_MOUSE_RIGHT] == true && !attacked)
	{
		meleeBarrier = 60.f *deltaTime;
		attacked = true;
		melee = true;
	}
	else melee = false;

	// results
	float dX = 0.f, dZ = 0.f;

	float mX = currentWalkingSpeedX * deltaTime;
	float mZ = currentWalkingSpeedZ * deltaTime;

	if (beingPulled || beingPushed)
	{
		dX = mX;
		dZ = mZ;
	}
	else
	{
		dX += (float)(mX * sin(rotationDegree));
		dZ += (float)(mX * cos(rotationDegree));

		dX += (float)(mZ * -cos(rotationDegree));
		dZ += (float)(mZ * sin(rotationDegree));
	}

	float dY = 0.f;

	if (!jumping) dY = GRAVITY *deltaTime;
	else
	{
		CURRENT_JUMP += JUMPING_SPEED * deltaTime;
		if (CURRENT_JUMP < JUMP_MAX)
			dY = JUMPING_SPEED * deltaTime;
		else
			jumping = false;
	}

	increasePosition(glm::vec3(dX, dY, dZ));

	if (position.y < LIMIT)
		available = false;
}

void Player::rotate(float xoffset, float yoffset, float deltaTime)
{
	xoffset *= -MOUSE_SENSITIVITY;
	rotationDegree += xoffset * deltaTime;
}

Player::Player(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model) 
{
	this->type = "block";
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
	this->casting = false;
	this->destructable = true;
	this->available = true;
	this->model = model;

	this->currentWalkingSpeedZ = 0.f;
	this->currentTurnSpeed = 0.f;
	this->currentJumpSpeed = 0.f;
	this->currentWalkingSpeedX = 0.f;

	this->jumping = false;

	this->rotX = 0.f;
	this->rotY = 1.f;
	this->rotZ = 0.f;

	this->currentAABB = model->aabb;
}

bool Player::increaseLife(float v)
{
	health += v;
	if (health <= 0.f)
		return false;
	else return true;
}

bool Player::decreaseLife(float v)
{
	health -= v;
	damaged = true;
	damagedTime = prevDeltaTime * 10.f;
	if (health <= 0.f)
		return false;
	else return true;
}

bool Player::isAvailable()
{
	if (health <= 0.f || !available)
		return false;
	else return true;
}

bool Player::push(glm::vec3 v)
{
	if (!beingPushed && !beingPulled)
	{
		pushingV = v;
		beingPushed = true;
		forcedMovTime = prevDeltaTime * 15.f;
		return true;
	}
	else return false;
}

bool Player::pull(glm::vec3 v)
{
	if (!beingPushed && !beingPulled)
	{
		pullingV = v;
		beingPulled = true;
		forcedMovTime = prevDeltaTime * 15.f;
		return true;
	}
	else return false;
}
