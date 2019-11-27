#include "enemy.h"
#include <GLFW/glfw3.h>

void Enemy::action(float deltaTime)
{
	// rotates according to the player

	glm::vec3 diff = player->getPosition() - position;
	rotationDegree = glm::orientedAngle(glm::vec2(diff.x,diff.z),glm::vec2(0.f, 1.f));
	transformed = true;


	// deciding attack type
	if (castType == -1) // deciding
	{
		castType = rand() % 3;
		effectType = rand() % 3;
		firstInit = true;
	}
	
	if (castType == 2) // random projectiles
	{
		if (firstInit) { cons = 20; firstInit = false; castBarrier = 0.f; }

		if (castBarrier < deltaTime)
		{
			dX = sin(glfwGetTime()) * 20.f;
			dZ = cos(glfwGetTime()) * 20.f;

			glm::vec3 v = glm::normalize(glm::vec3(dX, 0.f, dZ)) * 3.f;

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.3f), 
				projectileModel, RANDOM, (ProjectileEffect)effectType, true, v.x, 0.f, v.z));

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.3f),
				projectileModel, RANDOM, (ProjectileEffect)effectType, true, -v.x, 0.f, -v.z));

			castBarrier = deltaTime *2.f;
			cons--;
		}
		else castBarrier -= deltaTime;
	}
	else if (castType == 1) // projectile towards player
	{
		if (firstInit) { cons = 5; firstInit = false; castBarrier = 0.f; }

		if (castBarrier < deltaTime)
		{
			glm::vec3 t = player->currentAABB.getMaxExtent();
			//t.y += player->currentAABB.height;
			glm::vec3 target = glm::normalize(t - position) * 4.f;

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.3f),
				projectileModel, BULLET, (ProjectileEffect)effectType, true, target.x, target.y, target.z));

			castBarrier = deltaTime * 15.f;
			cons--;
		}
		else castBarrier -= deltaTime;
	}
	else if (castType == 0) // cast aoe on player
	{
		if (firstInit) { cons = 1; firstInit = false; castBarrier = 0.f; }

		if (castBarrier < deltaTime)
		{
			projectiles.push_back(new Projectile(glm::vec3(player->getPosition().x, player->getPosition().y + 3.f, player->getPosition().z), 0.f, glm::vec3(0.5f),
				projectileModel, AREA_OF_EFFECT, (ProjectileEffect)effectType, false, 0.f, -3.f, 0.f));

			castBarrier = deltaTime * 15.f;
			cons--;
		}
		else castBarrier -= deltaTime;
	}

	if (cons <= 0)
	{
		castType = -1;
	}
}

void Enemy::attachPlayer(Player * player)
{
	this->player = player;
}

void Enemy::attachProjectileModel(Model * model)
{
	this->projectileModel = model;
}

Enemy::Enemy(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model) {
	
	this->type = "block";
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
	this->health = 100.f;
	this->destructable = true;
	this->available = true;
	this->model = model;

	this->rotX = 0.f;
	this->rotY = 1.f;
	this->rotZ = 0.f;

	this->currentAABB = model->aabb;
}

bool Enemy::increaseLife(float v)
{
	health += v;
	if (health <= 0.f)
		return false;
	else return true;
}

bool Enemy::decreaseLife(float v)
{
	health -= v;
	if (health <= 0.f)
		return false;
	else return true;
}

bool Enemy::isAvailable()
{
	if (health <= 0.f || !available)
		return false;
	else return true;
}
