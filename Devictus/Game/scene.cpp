#include "scene.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

Scene::Scene()
{
}

Scene::~Scene()
{
	delete this->freeCamera;
	delete this->player;
	delete this->enemy;

	//for (auto iter : sceneGraph)
	//	delete &iter;
	sceneGraph.clear();

	//for (auto iter : projectiles)
	//	delete &iter;

	delete this->blockModel;
	delete this->enemyModel;
	delete this->playerModel;
	delete this->projectileModel;
}

void Scene::init(LevelDifficulty levelDifficulty, const char *levelPath)
{
	this->blockModel = new Model("./Objects/Block/block.obj");
	this->enemyModel = new Model("./Objects/Enemy/Earth_Golem_OBJ.obj");
	this->playerModel = new Model("./Objects/Enemy/Earth_Golem_OBJ.obj");
	this->projectileModel = new Model("./Objects/Projectile/sphere.obj");

	LevelInfo levelInfo = readLevelFromFile(levelPath);
	this->levelDifficulty = levelDifficulty;

	bool destructable = true;
	if (levelDifficulty == PEASANT)
		destructable = false;

	for (auto iter : sceneGraph)
		delete iter;
	sceneGraph.clear();

	unsigned int arenaSize0 = levelInfo.level0.size();
	unsigned int center0 = floor(arenaSize0 / 2);

	unsigned int arenaSize1 = levelInfo.level1.size();
	unsigned int center1 = floor(arenaSize1 / 2);

	blockLength = levelInfo.blockLength;

	// level0

	for (int m = 0; m < arenaSize0; m++) {
		for (int n = 0; n < arenaSize0; n++) {
			float xCoord = ((float)m - (float)center0) * blockLength;
			float zCoord = ((float)n - (float)center0) * blockLength;
			int height = levelInfo.level0[m][n];
			if (height != 0) {
				for (int k = 0; k < height; k++) {
					float yCoord = -((float)k * blockLength);
					sceneGraph.push_back(new Block(glm::vec3(xCoord, yCoord, zCoord), 0.f, glm::vec3((float)blockLength), blockModel, destructable, "floor"));
				}
			}
		}
	}

	// level1

	for (int m = 0; m < arenaSize1; m++) {
		for (int n = 0; n < arenaSize1; n++) {
			float xCoord = ((float)m - (float)center1) * blockLength;
			float zCoord = ((float)n - (float)center1) * blockLength;
			int height = levelInfo.level1[m][n];
			if (height != 0) {
				for (int k = 0; k < height; k++) {
					float yCoord = ((float)k * blockLength) + blockLength;
					sceneGraph.push_back(new Block(glm::vec3(xCoord, yCoord, zCoord), 0.f, glm::vec3((float)blockLength), blockModel, destructable, "wall"));
				}
			}
		}
	}

	this->player = new Player(glm::vec3(0.0f, 3.f * blockLength, -6.0f * blockLength), 0.f, glm::vec3(0.5f), playerModel);
	this->player->attachProjectileModel(this->projectileModel);

	this->enemy = new Enemy(glm::vec3(0.0f, enemyModel->aabb.getMaxExtent().y, 0.0f), 0.f, glm::vec3(1.0f), enemyModel);
	this->enemy->attachPlayer(this->player);
	this->enemy->attachProjectileModel(this->projectileModel);

	this->freeCamera = new Camera(glm::vec3(0.0f, 2.0f * blockLength, 5.0f * blockLength));
	this->playerCamera = new Camera3rdPerson(this->player);
}

//void Scene::draw(bool aabbDebug)
//{
//	Shader shader;
//	for (auto iter : sceneGraph)
//	{
//		if (iter->isAvailable())
//		{
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//			shader = Manager::getShader(iter->getType());
//			shader.use();
//			float life = iter->getLife();
//			shader.setFloat("durability", life);
//			shader.setMat4("model", iter->getTransform());
//			iter->drawModel(shader);
//
//			if (aabbDebug)
//			{
//				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//				shader = Manager::getShader("aabb");
//				shader.use();
//				if (iter->currentAABB.isCollided())
//					shader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
//				else
//					shader.setVec3("color", glm::vec3(0.f, 1.f, 0.f));
//				shader.setMat4("model", iter->getAABBTransform());
//				iter->drawAABB(shader);
//			}
//		}
//	}
//
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	for (Projectile * iter : enemy->projectiles)
//	{
//		if (iter->isAvailable())
//		{
//			ProjectileEffect effect = iter->getEffect();
//			glm::vec3 color;
//
//			if (effect == ATTACKER) color = glm::vec3(1.f, 0.f, 0.f);
//			else if (effect == PULLER) color = glm::vec3(0.f, 0.f, 1.f);
//			else if (effect == PUSHER) color = glm::vec3(1.f, 1.f, 0.f);
//			else color = glm::vec3(0.f);
//
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//			shader = Manager::getShader("projectile");
//			shader.use();
//			shader.setVec3("color", color);
//			shader.setMat4("model", iter->getTransform());
//			iter->drawModel(shader);
//
//			if (aabbDebug)
//			{
//				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//				shader = Manager::getShader("aabb");
//				shader.use();
//				if (iter->currentAABB.isCollided())
//					shader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
//				else
//					shader.setVec3("color", glm::vec3(0.f, 0.f, 1.f));
//				shader.setMat4("model", iter->getAABBTransform());
//				iter->drawAABB(shader);
//			}
//		}
//	}
//
//
//	for (Projectile * iter : player->projectiles)
//	{
//		if (iter->isAvailable())
//		{
//			ProjectileEffect effect = iter->getEffect();
//			glm::vec3 color(0.f);
//			if (effect == ATTACKER) color = glm::vec3(1.f, 0.f, 0.f);
//			else if (effect == PULLER) color = glm::vec3(0.3f, 0.f, 1.f);
//			else if (effect == PUSHER) color = glm::vec3(1.f, 1.f, 0.f);
//
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//			shader = Manager::getShader("projectile");
//			shader.use();
//			shader.setVec3("color", color);
//			shader.setMat4("model", iter->getTransform());
//			iter->drawModel(shader);
//
//			if (aabbDebug)
//			{
//				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//				shader = Manager::getShader("aabb");
//				shader.use();
//				if (iter->currentAABB.isCollided())
//					shader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
//				else
//					shader.setVec3("color", glm::vec3(0.f, 0.f, 1.f));
//				shader.setMat4("model", iter->getAABBTransform());
//				iter->drawAABB(shader);
//			}
//		}
//	}
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	if (player->isAvailable())
//	{
//		Shader shader;
//
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		shader = Manager::getShader(player->getType());
//		shader.use();
//		shader.setFloat("durability", 1.f);
//		shader.setMat4("model", player->getTransform());
//		player->drawModel(shader);
//
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//		shader = Manager::getShader("aabb");
//		shader.use();
//		if (player->currentAABB.isCollided())
//			shader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
//		else
//			shader.setVec3("color", glm::vec3(0.f, 0.f, 1.f));
//		shader.setMat4("model", player->getAABBTransform());
//		if (aabbDebug)	player->drawAABB(shader);
//	}
//
//	if (enemy->isAvailable())
//	{
//		Shader shader;
//
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		shader = Manager::getShader(enemy->getType());
//		shader.use();
//		shader.setFloat("durability", 1.f);
//		shader.setMat4("model", enemy->getTransform());
//		enemy->drawModel(shader);
//
//		if (aabbDebug)
//		{
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			shader = Manager::getShader("aabb");
//			shader.use();
//			if (enemy->currentAABB.isCollided())
//				shader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
//			else
//				shader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));
//			shader.setMat4("model", enemy->getAABBTransform());
//			enemy->drawAABB(shader);
//		}
//	}
//}

void Scene::drawShadowMap(Shader shader)
{
	Texture wallTexture = Manager::getTexture("wall");
	Texture objectTexture = Manager::getTexture("object");
	Texture projectileTexture = Manager::getTexture("projectile");

	for (auto iter : sceneGraph)
	{
		if (iter->isAvailable())
		{
			shader.use();
			shader.setMat4("model", iter->getTransform());
			iter->drawModel(shader);
		}
	}

	for (Projectile * iter : enemy->projectiles)
	{
		if (iter->isAvailable())
		{
			shader.use();
			shader.setMat4("model", iter->getTransform());
			iter->drawModel(shader);
		}
	}

	for (Projectile * iter : player->projectiles)
	{
		if (iter->isAvailable())
		{
			shader.use();
			shader.setMat4("model", iter->getTransform());
			iter->drawModel(shader);
		}
	}

	if (player->isAvailable())
	{
		shader.use();
		shader.setMat4("model", player->getTransform());
		player->drawModel(shader);
	}

	if (enemy->isAvailable())
	{
		shader.use();
		shader.setMat4("model", enemy->getTransform());
		enemy->drawModel(shader);
	}
}

void Scene::drawScene(Shader &shader, bool aabbDebug)
{
	Texture wallTexture = Manager::getTexture("wall");
	Texture objectTexture = Manager::getTexture("object");

	Texture red = Manager::getTexture("red");
	Texture yellow = Manager::getTexture("yellow");
	Texture blue = Manager::getTexture("blue");
	Texture black = Manager::getTexture("black");
	Texture playerT = Manager::getTexture("player");
	Texture enemyT = Manager::getTexture("enemy");

	Shader aabbShader = Manager::getShader("aabb");
	Shader projectileShader = Manager::getShader("projectile");

	glActiveTexture(GL_TEXTURE0);

	for (auto iter : sceneGraph)
	{
		if (iter->isAvailable())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			shader.use();
			shader.setMat4("model", iter->getTransform());

			if (iter->getType() == "wall") wallTexture.bind();
			else if (iter->getType() == "floor") objectTexture.bind();
			// TODO ADD MORE VARIETY
			iter->drawModel(shader);

			if (aabbDebug)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				aabbShader.use();
				if (iter->currentAABB.isCollided())
					aabbShader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
				else
					aabbShader.setVec3("color", glm::vec3(0.f, 1.f, 0.f));
				aabbShader.setMat4("model", iter->getAABBTransform());
				iter->drawAABB(aabbShader);
			}
		}
	}

	for (Projectile * iter : enemy->projectiles)
	{
		if (iter->isAvailable())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			projectileShader.use();

			if (iter->getEffect() == ATTACKER) red.bind();
			else if (iter->getEffect() == PULLER) blue.bind();
			else if (iter->getEffect() == PUSHER) yellow.bind();
		
			projectileShader.setMat4("model", iter->getTransform());
			iter->drawModel(projectileShader);
			if (aabbDebug)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				aabbShader.use();
				if (iter->currentAABB.isCollided())
					aabbShader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
				else
					aabbShader.setVec3("color", glm::vec3(0.f, 1.f, 0.f));
				aabbShader.setMat4("model", iter->getAABBTransform());
				iter->drawAABB(aabbShader);
			}
		}
	}

	black.bind();
	for (Projectile * iter : player->projectiles)
	{
		if (iter->isAvailable())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			projectileShader.use();
			projectileShader.setMat4("model", iter->getTransform());
			iter->drawModel(projectileShader);

			if (aabbDebug)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				aabbShader.use();
				if (iter->currentAABB.isCollided())
					aabbShader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
				else
					aabbShader.setVec3("color", glm::vec3(0.f, 1.f, 0.f));
				aabbShader.setMat4("model", iter->getAABBTransform());
				iter->drawAABB(aabbShader);
			}

			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			for (ProjectTrail trail : iter->trails)
			{
				if (trail.lifeTime > 0.f)
				{
					objectTexture.bind();
					shader.use();
					shader.setMat4("model", trail.model);
					iter->drawModel(shader);
				}
			}
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	if (player->isAvailable())
	{
		playerT.bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader.use();

		if (!player->canMeleeAttack())
		{
			// TODO MORE EFFECTS
			glm::mat4 m = player->getTransform();
			m = glm::rotate(m, 45.f, glm::vec3(1.f, 0.f, 0.f));
			shader.setMat4("model", m);
			player->drawModel(shader);
		}
		else
		{
			shader.setMat4("model", player->getTransform());
			player->drawModel(shader);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		for (Trail * trail : player->trails)
		{
			objectTexture.bind();
			shader.use();
			shader.setMat4("model", trail->model);
			player->drawModel(shader);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (aabbDebug)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			aabbShader.use();
			if (player->currentAABB.isCollided())
				aabbShader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
			else
				aabbShader.setVec3("color", glm::vec3(0.f, 1.f, 0.f));
			aabbShader.setMat4("model", player->getAABBTransform());
			player->drawAABB(aabbShader);
		}
	}

	enemyT.bind();
	if (enemy->isAvailable())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader.use();
		shader.setMat4("model", enemy->getTransform());
		enemy->drawModel(shader);

		if (aabbDebug)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			aabbShader.use();
			if (enemy->currentAABB.isCollided())
				aabbShader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
			else
				aabbShader.setVec3("color", glm::vec3(0.f, 1.f, 0.f));
			aabbShader.setMat4("model", enemy->getAABBTransform());
			enemy->drawAABB(aabbShader);
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Scene::drawGUI(Shader &shader)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glm::mat4 model;
	unsigned int x, y;

	// PLAYER HEALTH BAR
	
	unsigned int currentBarCount = (int)ceil(player->getLife() / 5.f);
	if (currentBarCount < 0) currentBarCount = 0;
	unsigned int damagedBarCount = (int)floor((100.f - player->getLife()) / 5.f);

	x = (WINDOW_WIDTH / 7) - 15 * 15;
	y = 100;

	shader.use();
	glActiveTexture(GL_TEXTURE0);
	Manager::getTexture("green").bind();
	for (int k = 0; k < currentBarCount + damagedBarCount; k++)
	{
		if (k >= currentBarCount) Manager::getTexture("black").bind();
		x += 15;
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(glm::vec2(static_cast<GLfloat>(x), static_cast<GLfloat>(y)), 0.0f));
		model = glm::scale(model, glm::vec3(glm::vec2(10.f), 1.0f));

		shader.setMat4("model", model);
		shader.setVec3("color", glm::vec3(0.6f));

		drawQuad();
	}

	// PLAYER STAMINA BAR

	currentBarCount = (int)ceil(player->getStamina() / 5.f);
	if (currentBarCount < 0) currentBarCount = 0;
	damagedBarCount = (int)floor((100.f - player->getStamina()) / 5.f);

	x = (WINDOW_WIDTH / 7) - 15 * 15;
	y = 80;

	shader.use();
	glActiveTexture(GL_TEXTURE0);
	Manager::getTexture("yellow").bind();
	for (int k = 0; k < currentBarCount + damagedBarCount; k++)
	{
		if (k >= currentBarCount) Manager::getTexture("black").bind();
		x += 15;
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(glm::vec2(static_cast<GLfloat>(x), static_cast<GLfloat>(y)), 0.0f));
		model = glm::scale(model, glm::vec3(glm::vec2(10.f), 1.0f));

		shader.setMat4("model", model);
		shader.setVec3("color", glm::vec3(0.6f));

		drawQuad();
	}

	
	// ENEMY HEALTH BAR

	currentBarCount = (int)ceil(enemy->getLife() / 5.f);
	if (currentBarCount < 0) currentBarCount = 0;
	damagedBarCount = (int)floor((100.f - enemy->getLife()) / 5.f);

	x = (WINDOW_WIDTH / 2) - 30 * 10;
	y = 980;

	glActiveTexture(GL_TEXTURE0);
	Manager::getTexture("red").bind();
	for (int k = 0; k < currentBarCount + damagedBarCount; k++)
	{
		if (k >= currentBarCount) Manager::getTexture("black").bind();
		x += 30;
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(glm::vec2(static_cast<GLfloat>(x), static_cast<GLfloat>(y)), 0.0f));
		model = glm::scale(model, glm::vec3(glm::vec2(15.f), 1.0f));

		shader.setMat4("model", model);
		shader.setVec3("color", glm::vec3(0.6f));

		drawQuad();
	}
}

void Scene::drawQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Scene::drawCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

LevelInfo Scene::readLevelFromFile(const char * levelPath)
{
	LevelInfo info;

	try {
		std::ifstream file(levelPath);
		std::string str;
		std::string word;

		unsigned int col;
		unsigned int row;

		std::getline(file, str);	// reads block length
		float blockLength = std::atof(str.c_str());

		std::getline(file, str);	// reads the level0 size
		int levelSize0 = std::stoi(str);

		std::vector<std::vector<char>> level0(levelSize0, std::vector<char>(levelSize0));

		row = 0;
		while (levelSize0 > 0 && std::getline(file, str)) {
			std::istringstream iss(str);
			col = 0;
			while (iss >> word) {
				level0[row][col] = std::stoi(word);
				col++;
			}

			row++;
			levelSize0--;
		}

		std::getline(file, str);	// reads the level1 size
		int levelSize1 = std::stoi(str);

		std::vector<std::vector<char>> level1(levelSize1, std::vector<char>(levelSize1));

		row = 0;
		while (levelSize1 > 0 && std::getline(file, str)) {
			std::istringstream iss(str);
			col = 0;
			while (iss >> word) {
				level1[row][col] = std::stoi(word);
				col++;
			}
			row++;
			levelSize1--;
		}

		info.level0 = level0;
		info.level1 = level1;
		info.blockLength = blockLength;
	}
	catch (std::exception e) {
		std::cout << "ERROR, Level failed to load at path: " << levelPath << std::endl;
	}

	return info;
}