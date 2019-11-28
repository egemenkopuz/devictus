#include "player.h"

void Player::attachProjectileModel(Model * model)
{
	this->projectileModel = model;
}

void Player::move(bool keys[], float deltaTime)
{
	prevDeltaTime = deltaTime;

	currentJumpSpeed = 0.f;
	currentWalkingSpeedX = 0.f;
	currentWalkingSpeedZ = 0.f;

	if (!jumping) CURRENT_JUMP = 0.f;

	if (keys[MOVE_UP] == true && !jumping && currentAABB.isCollided())
	{
		currentJumpSpeed = JUMPING_SPEED;
		jumping = true;
	}
	if (keys[MOVE_DOWN])
	{
		//potentialY += -JUMPING_SPEED;
	}

	if (invisFrame < deltaTime)
	{
		invisibility = false;
		dashBoost = 1.f;
	}
	else invisFrame -= deltaTime;

	if (dashCooldown < deltaTime)
	{
		dashed = false;
	}
	else dashCooldown -= deltaTime;

	if (keys[MOVE_SHIFT] && !invisibility && !dashed)
	{
		dashBoost = 2.f;
		invisFrame = 20.f * deltaTime;
		dashCooldown = 60.f * deltaTime;
		invisibility = true;
		dashed = true;
	}

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

	if (castBarrier < deltaTime)
	{
		casting = false;
	}
	else castBarrier -= deltaTime;

	if (keys[MOVE_MOUSE_LEFT] == true && !casting)
	{
		glm::vec3 diff = glm::normalize(target - position) * 4.f;

		projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.1f),
			projectileModel, BULLET, ATTACKER, true, diff.x, diff.y, diff.z));
		castBarrier = deltaTime * 30.f;
		casting = true;
	}

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

	float dX = 0.f, dZ = 0.f;

	float mX = currentWalkingSpeedX * deltaTime;
	float mZ = currentWalkingSpeedZ * deltaTime;

	dX += (float)(mX * sin(rotationDegree));
	dZ += (float)(mX * cos(rotationDegree));

	dX += (float)(mZ * -cos(rotationDegree));
	dZ += (float)(mZ * sin(rotationDegree));

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
	this->health = 100.f;
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
