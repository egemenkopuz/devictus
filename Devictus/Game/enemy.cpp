#include "enemy.h"
#include <GLFW/glfw3.h>

void Enemy::action(float deltaTime)
{
	// rotates according to the player
	float offsetX = -1.f * sin(player->getRotationDegree());
	float offsetZ = -1.f * cos(player->getRotationDegree());
	rotationDegree = glm::orientedAngle(glm::vec2(offsetX, offsetZ), glm::vec2(0.f, 1.f));
	transformed = true;

	// PHASE CALLS
	switch (phase)
	{
	case 1:
		phase1(deltaTime);
		break;
	case 2:
		phase2(deltaTime);
		break;
	case 3:
		phase3(deltaTime);
		break;
	case 4:
		phase4(deltaTime);
		break;
	}

	// NEXT PHASE AND FULL RESET
	if (phaseChange())
	{
		firstMoveType = -1; firstMoveCount = 0; firstCD = 0.f; firstMoveInterval = 0.f;
		secondMoveType = -1; secondMoveCount = 0; secondCD = 0.f; secondMoveInterval = 0.f;
		thirdMoveType = -1; thirdMoveCount = 0; thirdCD = 0.f; thirdMoveInterval = 0.f;
	}
}

unsigned int randomArea = 0;
glm::vec3 v1, v2;

void Enemy::phase1(float deltaTime)
{
	// FIRST & SECOND MOVES ONLY
	if (firstMoveType == -1 && firstCD < deltaTime)
		firstP = rand() % 100;

	if (secondMoveType == -1 && secondCD < deltaTime)
		secondP = rand() % 100;

	//// FIRST MOVE

	// [0] BULLET | %70 
	if (firstCD < deltaTime && (firstMoveType == 0 || firstP < 70))
	{
		if (firstMoveType == -1)
		{
			firstMoveCount = 5;
			firstMoveInterval = 0.f;
			firstMoveType = 0;
		}

		if (firstMoveInterval < deltaTime)
		{
			glm::vec3 t = player->currentAABB.getMaxExtent();
			//t.y += player->currentAABB.height;
			glm::vec3 target = glm::normalize(t - position) * 4.f;

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.1f), projectileModel,
				BULLET, (ProjectileEffect)(firstP % 3), true, target.x, target.y, target.z));

			firstMoveInterval = deltaTime * 15.f;
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstCD = 15.f * deltaTime;
				firstMoveType = -1;
			}
		}
		else firstMoveInterval -= deltaTime;
	}
	// [1] SPIRAL | %30
	else if (firstCD < deltaTime && (firstMoveCount == 1 || firstP >= 70))
	{
		if (firstMoveType == -1)
		{
			firstMoveCount = 20;
			firstMoveInterval = 0.f;
			firstMoveType = 1;
			firstP % 2 == 0 ? randomFlag = false : randomFlag = true;
		}

		if (firstMoveInterval < deltaTime)
		{
			if (randomFlag)
			{
				dX = cos(glfwGetTime()) * 30.f;
				dZ = sin(glfwGetTime()) * 30.f;
			}
			else
			{
				dX = sin(glfwGetTime()) * 30.f;
				dZ = cos(glfwGetTime()) * 30.f;
			}

			glm::vec3 v = glm::normalize(glm::vec3(dX, 0.f, dZ)) * 4.f;
			glm::vec3 p = position;

			p.x = p.x + dX * deltaTime;
			p.z = p.z + dZ * deltaTime;

			projectiles.push_back(new Projectile(p, 0.f, glm::vec3(0.3f),
				projectileModel, SPIRAL, (ProjectileEffect)(firstP % 3), true, v.x, 0.f, v.z));

			p = position;
			p.x = p.x - dX * deltaTime;
			p.z = p.z - dZ * deltaTime;

			projectiles.push_back(new Projectile(p, 0.f, glm::vec3(0.3f),
				projectileModel, SPIRAL, (ProjectileEffect)(firstP % 3), true, -v.x, 0.f, -v.z));

			firstMoveInterval = deltaTime * 5.f;
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstCD = 15.f * deltaTime;
				firstMoveType = -1;
			}
		}
		else firstMoveInterval -= deltaTime;
	}
	else firstCD -= deltaTime;

	//// SECOND MOVE

	// [0] | AOE | %100
	if (secondCD < deltaTime && (secondMoveType == 0 || secondP < 100))
	{
		if (secondMoveType == -1)
		{
			secondMoveCount = 1;
			secondMoveType = 0;
			secondMoveInterval = 0.f;
		}

		if (secondMoveInterval < deltaTime)
		{
			projectiles.push_back(new Projectile(glm::vec3(player->getPosition().x, player->getPosition().y + 3.f, player->getPosition().z), 0.f, glm::vec3(0.5f),
				projectileModel, AOE, ATTACKER, true, 0.f, -1.f, 0.f, 5.f));

			secondMoveInterval = deltaTime * 200.f;
			secondMoveCount--;

			if (secondMoveCount <= 0)
			{
				secondCD = 400.f * deltaTime;
				secondMoveType = -1;
			}
		}
		else secondMoveInterval -= deltaTime;
	}
	else secondCD -= deltaTime;
}

void Enemy::phase2(float deltaTime)
{
	if (firstMoveType == -1 && firstCD < deltaTime)
		firstP = rand() % 100;

	if (secondMoveType == -1 && secondCD < deltaTime)
		secondP = rand() % 100;

	//// FIRST MOVE

	// [0] HOMING | %30
	if (firstCD < deltaTime && (firstMoveType == 0 || firstP > 70))
	{
		if (firstMoveType == -1)
		{
			firstMoveCount = 2;
			firstMoveInterval = 0.f;
			firstMoveType = 0;
		}

		if (firstMoveInterval < deltaTime)
		{
			Projectile * p = new Projectile(glm::vec3(position.x, position.y + 1.f, position.z), 0.f,
				glm::vec3(0.2f), projectileModel, HOMING, (ProjectileEffect)(firstP % 3), true, 0.f, 0.f, 0.f, 50.f);

			p->updateTarget(player->getPosition());
			p->updateDest(false);

			projectiles.push_back(p);

			firstMoveInterval = deltaTime * 60.f;
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstCD = 0.f * deltaTime;
				firstMoveType = -1;
			}
		}
		else firstMoveInterval -= deltaTime;
	}
	// [1] SPIRAL_MAX | %40
	else if (firstCD < deltaTime && (firstMoveType == 1 || (firstP <= 70 && firstP > 30)))
	{
		if (firstMoveType == -1)
		{
			firstMoveCount = 25;
			firstMoveInterval = 0.f;
			firstMoveType = 1;

			randomArea = firstP % 4;

			// +x +z
			if (randomArea == 0)
			{
				v1 = glm::vec3(1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, 1.f);
			}
			// -x +z
			else if (randomArea == 1)
			{
				v1 = glm::vec3(-1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, 1.f);
			}
			// -x -z
			else if (randomArea == 2)
			{
				v1 = glm::vec3(-1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, -1.f);
			}
			// +x -z
			else
			{
				v1 = glm::vec3(1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, -1.f);
			}
		}

		if (firstMoveInterval < deltaTime)
		{
			// +x +z
			if (randomArea == 0)
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}
			// -x +z
			else if (randomArea == 1)
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}
			// -x -z
			else if (randomArea == 2)
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}
			// +x -z
			else
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}

			v1 = glm::normalize(v1) * 2.f;
			v2 = glm::normalize(v2) * 2.f;

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.15f),
				projectileModel, SPIRAL_MAX, (ProjectileEffect)(firstP % 3), true, v1.x, 0.f, v1.z, 4.f));


			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.15f),
				projectileModel, SPIRAL_MAX, (ProjectileEffect)(firstP % 3), true, v2.x, 0.f, v2.z, 4.f));


			firstMoveInterval = deltaTime * 2.f;
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstCD = 15.f * deltaTime;
				firstMoveType = -1;
			}
		}
		else firstMoveInterval -= deltaTime;
	}
	// [2] BULLET | %30
	else if (firstCD < deltaTime && (firstMoveType == 2 || firstP <= 30))
	{
		if (firstMoveType == -1)
		{
			firstMoveCount = 3;
			firstMoveInterval = 0.f;
			firstMoveType = 2;
		}

		if (firstMoveInterval < deltaTime)
		{
			glm::vec3 t = player->currentAABB.getMaxExtent();
			glm::vec3 target = glm::normalize(t - position) * 5.f;	// faster & bigger

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.2f), projectileModel,
				BULLET, (ProjectileEffect)(firstP % 3), true, target.x, target.y, target.z));

			firstMoveInterval = deltaTime * 15.f;
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstCD = 15.f * deltaTime;
				firstMoveType = -1;
			}
		}
		else firstMoveInterval -= deltaTime;

	}
	else firstCD -= deltaTime;

	//// SECOND MOVE

	// [0] | AOE | %100
	if (secondCD < deltaTime && (secondMoveType == 0 || secondP < 100))
	{
		if (secondMoveType == -1)
		{
			secondMoveCount = 1;
			secondMoveType = 0;
			secondMoveInterval = 0.f;
		}

		if (secondMoveInterval < deltaTime)
		{
			projectiles.push_back(new Projectile(glm::vec3(player->getPosition().x, player->getPosition().y + 3.f, player->getPosition().z), 0.f, glm::vec3(0.5f),
				projectileModel, AOE, ATTACKER, true, 0.f, -1.f, 0.f, 5.f));

			secondMoveInterval = deltaTime * 200.f;
			secondMoveCount--;

			if (secondMoveCount <= 0)
			{
				secondCD = 500.f * deltaTime;
				secondMoveType = -1;
			}
		}
		else secondMoveInterval -= deltaTime;
	}
	else secondCD -= deltaTime;
}

void Enemy::phase3(float deltaTime)
{
	if (firstMoveType == -1 && firstCD < deltaTime)
		firstP = rand() % 100;

	if (secondMoveType == -1 && secondCD < deltaTime)
		secondP = rand() % 100;

	if (thirdMoveType == -1 && thirdCD < deltaTime)
		thirdP = rand() % 100;

	// FIRST MOVE

	// [0] HOMING | %30
	if (firstCD < deltaTime && (firstMoveType == 0 || firstP > 70))
	{
		if (firstMoveType == -1)
		{
			firstMoveCount = 2;
			firstMoveInterval = 0.f;
			firstMoveType = 0;
		}

		if (firstMoveInterval < deltaTime)
		{
			Projectile * p = new Projectile(glm::vec3(position.x, position.y + 1.f, position.z), 0.f,
				glm::vec3(0.2f), projectileModel, HOMING, (ProjectileEffect)(firstP % 3), true, 0.f, 0.f, 0.f, 50.f);

			p->updateTarget(player->getPosition());
			p->updateDest(false);

			projectiles.push_back(p);

			firstMoveInterval = deltaTime * 60.f;
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstCD = 60.f * deltaTime;
				firstMoveType = -1;
			}
		}
		else firstMoveInterval -= deltaTime;
	}
	// [1] BULLET | %70
	else if (firstCD < deltaTime && (firstMoveType == 1 || firstP <= 70))
	{
		if (firstMoveType == -1)
		{
			firstMoveCount = 3;
			firstMoveInterval = 0.f;
			firstMoveType = 1;
		}

		if (firstMoveInterval < deltaTime)
		{
			glm::vec3 t = player->currentAABB.getMaxExtent();
			glm::vec3 target = glm::normalize(t - position) * 5.f;	// faster & bigger

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.2f), projectileModel,
				BULLET, (ProjectileEffect)(firstP % 3), true, target.x, target.y, target.z));

			firstMoveInterval = deltaTime * 15.f;
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstCD = 60.f * deltaTime;
				firstMoveType = -1;
			}
		}
		else firstMoveInterval -= deltaTime;

	}
	else firstCD -= deltaTime;

	// SECOND MOVE

	// [0] SPIRAL | %50
	if (secondCD < deltaTime && (secondMoveType == 0 || secondP >= 50))
	{
		if (secondMoveType == -1)
		{
			secondMoveCount = 20;
			secondMoveInterval = 0.f;
			secondMoveType = 1;
			secondP % 2 == 0 ? randomFlag = false : randomFlag = true;
		}

		if (secondMoveInterval < deltaTime)
		{
			if (randomFlag)
			{
				dX = cos(glfwGetTime()) * 30.f;
				dZ = sin(glfwGetTime()) * 30.f;
			}
			else
			{
				dX = sin(glfwGetTime()) * 30.f;
				dZ = cos(glfwGetTime()) * 30.f;
			}

			glm::vec3 v = glm::normalize(glm::vec3(dX, 0.f, dZ)) * 4.f;
			glm::vec3 p = position;

			p.x = p.x + dX * deltaTime;
			p.z = p.z + dZ * deltaTime;

			projectiles.push_back(new Projectile(p, 0.f, glm::vec3(0.3f),
				projectileModel, SPIRAL, (ProjectileEffect)(secondP % 3), true, v.x, 0.f, v.z));


			p = position;
			p.x = p.x - dX * deltaTime;
			p.z = p.z - dZ * deltaTime;

			projectiles.push_back(new Projectile(p, 0.f, glm::vec3(0.3f),
				projectileModel, SPIRAL, (ProjectileEffect)(secondP % 3), true, -v.x, 0.f, -v.z));


			secondMoveInterval = deltaTime * 5.f;
			secondMoveCount--;

			if (secondMoveCount <= 0)
			{
				secondCD = 60.f * deltaTime;
				secondMoveType = -1;
			}
		}
		else secondMoveInterval -= deltaTime;
	}
	// [1] SPIRAL_MAX | %50
	else if (secondCD < deltaTime && (secondMoveType == 1 || (secondP < 50)))
	{
		if (secondMoveType == -1)
		{
			secondMoveCount = 25;
			secondMoveInterval = 0.f;
			secondMoveType = 1;

			randomArea = secondP % 4;

			// +x +z
			if (randomArea == 0)
			{
				v1 = glm::vec3(1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, 1.f);
			}
			// -x +z
			else if (randomArea == 1)
			{
				v1 = glm::vec3(-1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, 1.f);
			}
			// -x -z
			else if (randomArea == 2)
			{
				v1 = glm::vec3(-1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, -1.f);
			}
			// +x -z
			else
			{
				v1 = glm::vec3(1.f, 0.f, 0.f);
				v2 = glm::vec3(0.f, 0.f, -1.f);
			}
		}

		if (secondMoveInterval < deltaTime)
		{
			// +x +z
			if (randomArea == 0)
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}
			// -x +z
			else if (randomArea == 1)
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}
			// -x -z
			else if (randomArea == 2)
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}
			// +x -z
			else
			{
				v1 = glm::rotate(v1, 3.f, glm::vec3(0.f, 1.f, 0.f));
				v2 = glm::rotate(v2, 3.f, glm::vec3(0.f, 1.f, 0.f));
			}

			v1 = glm::normalize(v1) * 2.f;
			v2 = glm::normalize(v2) * 2.f;

			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.15f),
				projectileModel, SPIRAL_MAX, (ProjectileEffect)(secondP % 3), true, v1.x, 0.f, v1.z, 4.f));


			projectiles.push_back(new Projectile(position, 0.f, glm::vec3(0.15f),
				projectileModel, SPIRAL_MAX, (ProjectileEffect)(secondP % 3), true, v2.x, 0.f, v2.z, 4.f));


			secondMoveInterval = deltaTime * 2.f;
			secondMoveCount--;

			if (secondMoveCount <= 0)
			{
				secondCD = 60.f * deltaTime;
				secondMoveType = -1;
			}
		}
		else secondMoveInterval -= deltaTime;
	}
	else secondCD -= deltaTime;

	// THIRD MOVE

	// [0] | AOE | %100
	if (thirdCD < deltaTime && (thirdMoveType == 0 || thirdP < 100))
	{
		if (thirdMoveType == -1)
		{
			thirdMoveCount = 1;
			thirdMoveType = 0;
			thirdMoveInterval = 0.f;
		}

		if (thirdMoveInterval < deltaTime)
		{
			projectiles.push_back(new Projectile(glm::vec3(player->getPosition().x, player->getPosition().y + 3.f, player->getPosition().z), 0.f, glm::vec3(0.5f),
				projectileModel, AOE, ATTACKER, true, 0.f, -1.f, 0.f, 5.f));

			thirdMoveInterval = deltaTime * 200.f;
			thirdMoveCount--;

			if (thirdMoveCount <= 0)
			{
				thirdCD = 300.f * deltaTime;
				thirdMoveType = -1;
			}
		}
		else thirdMoveInterval -= deltaTime;
	}
	else thirdCD -= deltaTime;
}

glm::vec3 ray;
void Enemy::phase4(float deltaTime)
{
	// [-1]..[0]..[1]..[2]..[3] | BOMB | %100 AT START
	if (firstMoveType == -1 || firstMoveType == 0)
	{
		// MOVE UP
		if (firstMoveType == -1)
		{

			firstMoveCount = 240.f;
			firstMoveInterval = 0.f;
			firstMoveType = 0;
			invinsible = true;
			onlyMelee = true;
		}

		if (firstMoveInterval < deltaTime)
		{
			increasePosition(glm::vec3(0.f, deltaTime/4.f, 0.f));
			firstMoveCount--;

			health += 1.f;
			if (health >= 100.f) health = 100.f;

			if (firstMoveCount <= 0)
			{
				firstMoveType = 1; // NEXT TO EXPLOSION
			}
		}
		else firstMoveInterval -= deltaTime;
	}
	else if (firstMoveType == 1)
	{
		// EXPLODE
		Projectile * p = new Projectile(position, 0.f, glm::vec3(0.1f), projectileModel,
			BOMB, FATAL, true, 0.f, 0.f, 0.f, 30.f * deltaTime);
		p->makeTrails = true;
		p->updateDest(false);

		projectiles.push_back(p);

		scale += 0.7f;
		increasePosition(glm::vec3(0.f, 0.5f, 0.f));

		exploded = true;

		firstMoveType = 2;
	}
	else if (firstMoveType == 2 || firstMoveType == 3)
	{
		// MOVE DOWN
		if (firstMoveType == 2)
		{
			firstMoveCount = 60.f;
			firstMoveInterval = 0.f;
			firstMoveType = 3;
		}

		if (firstMoveInterval < deltaTime)
		{
			increasePosition(glm::vec3(0.f, -deltaTime, 0.f));
			firstMoveCount--;

			if (firstMoveCount <= 0)
			{
				firstMoveType = 4; 
				invinsible = false;
			}
		}
		else firstMoveInterval -= deltaTime;
	}
	else
	{
		// RAY ATTACK
		/*if (secondMoveType == -1 && secondCD < deltaTime)
			secondP = rand() % 100;*/

		if (secondCD < deltaTime)
		{
			if (secondMoveType == -1)
			{
				secondMoveCount = 10;
				secondMoveInterval = 0.f;
				secondMoveType = 0;

				double r1 = rand() % 100;
				double r2 = rand() % 100;

				ray = glm::normalize(glm::vec3(r1,0.f,r2));
				ray.y = .5f;
				//ray = (glm::normalize(glm::vec3(r1, position.y, r2)) - position) * deltaTime;
				//ray = glm::normalize(player->getPosition() - position) * deltaTime;
			}

			if (secondMoveInterval < deltaTime)
			{

				ray.x += (ray.x * (deltaTime * 2.f));
				ray.z += (ray.z * (deltaTime * 2.f));

				Projectile * p = new Projectile(ray, 0.f, glm::vec3(0.4f), projectileModel,
						RAY, FATAL, true, 0.f, 0.f, 0.f, 2.f);

				p->updateDest(false);

				projectiles.push_back(p);

				secondMoveInterval = deltaTime * 10.f;
				secondMoveCount--;

				if (secondMoveCount <= 0)
				{
					secondCD = 10.f * deltaTime;
					secondMoveType = -1;
				}
			}
			else secondMoveInterval -= deltaTime;
		}
		else secondCD -= deltaTime;
	}
}

bool Enemy::phaseChange()
{
	if (phase == 1 && health < 70.f)
	{
		phase = 2;
		return true;
	}
	else if (phase == 2 && health < 30.f)
	{
		phase = 3;
		return true;
	}
	else if (phase == 3 && health < 10.f)
	{
		phase = 4;
		return true;
	}
	else return false;
}

void Enemy::attachPlayer(Player * player)
{
	this->player = player;
}

void Enemy::attachProjectileModel(Model * model)
{
	this->projectileModel = model;
}

Enemy::Enemy(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model)
{
	srand(time(NULL));

	this->type = "block";
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
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
	if (invinsible || onlyMelee) return true;
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