#include "projectile.h"

void Projectile::move(float deltaTime)
{
	lifeTime -= deltaTime;

	if (lifeTime > 0.f) 
	{
		if (pType == BULLET || RANDOM)
		{
			increasePosition(glm::vec3(xSpeed * deltaTime, ySpeed * deltaTime, zSpeed * deltaTime));
		}
		else if (pType = AOE)
		{
			increasePosition(glm::vec3(0.f, ySpeed * deltaTime, 0.f));
		}
	}
	else available = false;
}

Projectile::Projectile(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model, 
	ProjectileType pType, ProjectileEffect pEffect, bool effectEnv, float xSpeed, float ySpeed, float zSpeed)
{
	this->type = "projectile";
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
	this->destructable = true;
	this->available = true;
	this->model = model;

	this->lifeTime = 3.f;
	this->pType = pType;
	this->pEffect = pEffect;

	this->rotX = 0.f;
	this->rotY = 1.f;
	this->rotZ = 0.f;

	this->envReactive = effectEnv;

	this->xSpeed = xSpeed;
	this->ySpeed = ySpeed;
	this->zSpeed = zSpeed;

	this->currentAABB = model->aabb;
	getTransform();
}

bool Projectile::increaseLife(float v)
{
	lifeTime += v;
	if (lifeTime <= 0.f)
		return false;
	else return true;
}

bool Projectile::decreaseLife(float v)
{
	//lifeTime -= (float)v;
	lifeTime = -1.f;
	if (lifeTime <= 0.f)
		return false;
	else return true;
}

bool Projectile::isAvailable()
{
	if (lifeTime <= 0.f || !available)
		return false;
	else return true;
}
