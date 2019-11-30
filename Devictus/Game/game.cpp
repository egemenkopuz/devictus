#include "game.h"

Game::Game()
{
	this->terminated = false;
	this->currentDiff = PEASANT;
	this->state = GAME_MENU;
	this->menuSelectionBarrierVal = 0.0f;
	this->menuSelectionBarrier = true;
	this->gameWidth = (float)WINDOW_WIDTH;
	this->gameHeight = (float)WINDOW_HEIGHT;
	this->aspecRatio = this->gameWidth / this->gameHeight;
}

Game::~Game()
{
	delete textRenderer;
	delete freeCamera;
	delete playerCamera;
	delete player;
	delete enemy;
}

void Game::processMouse(float x, float y, float deltaTime)
{
	if (state == GAME_ACTIVE) {
		if (paused) freeCamera->ProcessMouseMovement(x, y);
		else player->rotate(x, y, deltaTime);
	}
}

void Game::processKey(KEY key, float deltaTime)
{
	if (state == GAME_MENU) {
		if (!menuSelectionBarrier) return;
		switch (key) {
		case KEY_ESCAPE:
			terminated = true;
			break;
		case KEY_ENTER:
			if (currentDiff == PEASANT) scene.init(currentDiff, "./Levels/peasant.lvl");
			else if (currentDiff == CHALLENGER) scene.init(currentDiff, "./Levels/challenger.lvl");
			else scene.init(currentDiff, "./Levels/god.lvl");

			this->player = scene.player;
			this->enemy = scene.enemy;
			this->freeCamera = scene.freeCamera;
			this->playerCamera = scene.playerCamera;

			state = GAME_ACTIVE;
			paused = true;
			break;
		case KEY_W:
			if (currentDiff == GOD)
				currentDiff = CHALLENGER;
			else currentDiff = PEASANT;

			break;
		case KEY_S:
			if (currentDiff == PEASANT)
				currentDiff = CHALLENGER;
			else currentDiff = GOD;

			break;
		}
		// to have a delay when navigating the game menu
		menuSelectionBarrier = false;
		menuSelectionBarrierVal = 15.0f * deltaTime;
	}
	else if (state == GAME_ACTIVE) {
		if (key == KEY_PAUSE) {
			if (!menuSelectionBarrier) return;
			paused = !paused;
			menuSelectionBarrier = false;
			menuSelectionBarrierVal = 50.f * deltaTime;
		}

		if (key == KEY_DEBUG) {
			if (!menuSelectionBarrier) return;
			aabbDebug = !aabbDebug;
			menuSelectionBarrier = false;
			menuSelectionBarrierVal = 15.f * deltaTime;
		}

		if (key == KEY_ESCAPE) {
			terminated = true;
		}

		if (key == KEY_SPACE) {
			if (paused) freeCamera->ProcessKeyboard(UP, deltaTime);
			else movementKeys[MOVE_UP] = true;
		}

		if (key == KEY_LEFT_SHIFT) {
			if (paused) freeCamera->ProcessKeyboard(DOWN, deltaTime);
			else movementKeys[MOVE_SHIFT] = true;
		}

		if (key == KEY_W) {
			if (paused) freeCamera->ProcessKeyboard(FORWARD, deltaTime);
			else movementKeys[MOVE_FORWARD] = true;
		}

		if (key == KEY_A) {
			if (paused) freeCamera->ProcessKeyboard(LEFT, deltaTime);
			else movementKeys[MOVE_LEFT] = true;
		}

		if (key == KEY_S) {
			if (paused) freeCamera->ProcessKeyboard(BACKWARD, deltaTime);
			else movementKeys[MOVE_BACKWARD] = true;
		}

		if (key == KEY_D) {
			if (paused) freeCamera->ProcessKeyboard(RIGHT, deltaTime);
			else movementKeys[MOVE_RIGHT] = true;
		}

		if (key == KEY_LEFT) {
			if (!paused) movementKeys[MOVE_MOUSE_LEFT] = true;
		}
		if (key == KEY_RIGHT) {
			if (!paused) movementKeys[MOVE_MOUSE_RIGHT] = true;
		}
	}
	else if (key == KEY_ESCAPE || key == KEY_ENTER) terminated = true;
}

void Game::init()
{
	Manager::loadTexture("./Textures/object.jpg", "object");
	Manager::loadTexture("./Textures/wall.jpg", "wall");
	Manager::loadTexture("./Textures/container.jpg", "projectile");

	Manager::loadShader("./Shaders/block.vert", "./Shaders/block.frag", "block");
	Manager::loadShader("./Shaders/projectile.vert", "./Shaders/projectile.frag", "projectile");
	Manager::loadShader("./Shaders/text.vert", "./Shaders/text.frag", "text");
	Manager::loadShader("./Shaders/aabb.vert", "./Shaders/aabb.frag", "aabb");
	
	// exp
	Manager::loadShader("./Shaders/depth.vert", "./Shaders/depth.frag", "depth");
	Shader objectShader = Manager::loadShader("./Shaders/object.vert", "./Shaders/object.frag", "object");
	objectShader.use();
	objectShader.setInt("diffuseTexture", 0);
	objectShader.setInt("shadowMap", 1);

	Shader quadShader = Manager::loadShader("./Shaders/debugQuad.vert", "./Shaders/debugQuad.frag", "quad");
	quadShader.use();
	quadShader.setInt("depthMap", 0);
	// exp
	
	Shader blockShader = Manager::getShader("block");
	blockShader.use();
	blockShader.setFloat("material.shininess", 32.0f);
	blockShader.setFloat("material.diffuse", 1.0f);
	blockShader.setFloat("material.specular", 1.0f);

	// directional light
	blockShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	blockShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	blockShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	blockShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	// point light 1
	blockShader.setVec3("pointLights[0].position", glm::vec3(0.0f, 3.0f, 0.0f));
	blockShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	blockShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	blockShader.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.5f);
	blockShader.setFloat("pointLights[0].constant", 1.0f);
	blockShader.setFloat("pointLights[0].linear", 0.09);
	blockShader.setFloat("pointLights[0].quadratic", 0.032);

	Shader textShader = Manager::getShader("text");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WINDOW_WIDTH), 0.0f, static_cast<GLfloat>(WINDOW_HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	shadowMap = ShadowMap();
	shadowMap.init();

	textRenderer = new TextRenderer("text");

	this->state = GAME_MENU;
}

void Game::update(float deltaTime)
{
	switch (state) {
	case GAME_MENU:
		if (menuSelectionBarrier == false && deltaTime < menuSelectionBarrierVal) {
			menuSelectionBarrierVal -= deltaTime;
		}
		else menuSelectionBarrier = true;
		break;
	case GAME_ACTIVE:
		if (menuSelectionBarrier == false && deltaTime < menuSelectionBarrierVal) {
			menuSelectionBarrierVal -= deltaTime;
		}
		else
		{
			if (!paused)
			{
				player->target = enemy->getPosition();

				player->move(movementKeys, deltaTime);

				enemy->action(deltaTime);

				for (int b = 0; b < NUM_MOVE_KEYS; b++) movementKeys[b] = false;

				std::vector<Projectile*>::iterator iter = enemy->projectiles.begin();
				while (iter != enemy->projectiles.end())
				{
					if ((*iter)->isAvailable()) 
					{
						(*iter)->move(deltaTime);
						iter++;
					}
					else if (iter != enemy->projectiles.end())
						iter = enemy->projectiles.erase(iter);
					else {
						enemy->projectiles.erase(iter);
						break;
					}
				}

				iter = player->projectiles.begin();
				while (iter != player->projectiles.end())
				{
					if ((*iter)->isAvailable())
					{
						(*iter)->move(deltaTime);
						iter++;
					}
					else if (iter != player->projectiles.end())
						iter = player->projectiles.erase(iter);
					else {
						player->projectiles.erase(iter);
						break;
					}
				}

				checkCollisions(deltaTime);

				playerCamera->processMovement();

				if (!enemy->isAvailable())
					state = GAME_WIN;

				if (!player->isAvailable())
					state = GAME_LOSE;
			}
			menuSelectionBarrier = true;
		}

		break;
	case GAME_WIN:

		break;

	case GAME_LOSE:

		break;
	}
}

void Game::render(float deltaTime)
{
	switch (state) {
	case GAME_MENU:
		// Text rendering
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		textRenderer->renderText("DEVICTUS", 650.f, 800.f, 2.0f, glm::vec3(1.0, 0.0f, 0.0f));
		textRenderer->renderText("Choose Difficulty", 800.f, gameHeight / 2.5f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		if (currentDiff == PEASANT)
			textRenderer->renderText("Peasant", 900.f, gameHeight / 3.f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
		else if (currentDiff == CHALLENGER)
			textRenderer->renderText("Challenger", 870.f, gameHeight / 3.f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
		else
			textRenderer->renderText("GOD", 930.f, gameHeight / 3.f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
		break;
	case GAME_ACTIVE:
		// view/projection transformations
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec3 position;

		if (paused)
		{
			// free camera view
			projection = glm::perspective(glm::radians(freeCamera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
			view = freeCamera->GetViewMatrix();
			position = freeCamera->Position;
		}
		else
		{
			projection = glm::perspective(glm::radians(45.f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
			view = playerCamera->getViewMatrix();
			position = playerCamera->getPosition();
		}

		////////////////////// exp
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
	
		lightProjection = glm::perspective(glm::radians(120.f), (GLfloat)1024 / (GLfloat)1024, shadowMap.near_plane, shadowMap.far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, shadowMap.near_plane, shadowMap.far_plane);
		lightView = glm::lookAt(glm::vec3(0.f,4.f,0.f), glm::vec3(0.f,0.f,0.f), glm::vec3(1.f, 0.0f, 1.0f));
		lightSpaceMatrix = lightProjection * lightView;

		Shader depthShader = Manager::getShader("depth");
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glDisable(GL_CULL_FACE);
		glViewport(0, 0, 1024, 1024);
		shadowMap.bindForWriting();
		glClear(GL_DEPTH_BUFFER_BIT);
		/*glActiveTexture(GL_TEXTURE0);
		Manager::getTexture("object").bind();*/
		this->scene.drawShadowMap(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_CULL_FACE);
		glViewport(0, 0, 1920, 1080);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader objectShader = Manager::getShader("object");
		objectShader.use();
		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);
		objectShader.setVec3("viewPos", position);
		objectShader.setVec3("lightPos", glm::vec3(0.f, 3.5f, 0.f));
		objectShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shadowMap.bindForReading(GL_TEXTURE1);

		if (aabbDebug)
		{
			Shader aabbShader = Manager::getShader("aabb");
			aabbShader.use();
			aabbShader.setMat4("projection", projection);
			aabbShader.setMat4("view", view);
		}
		
		Shader projectileShader = Manager::getShader("projectile");
		projectileShader.use();
		projectileShader.setMat4("projection", projection);
		projectileShader.setMat4("view", view);

		this->scene.drawScene(objectShader,aabbDebug);

		// DEBUG

		/*Shader quadShader = Manager::getShader("quad");
		quadShader.use();
		quadShader.setFloat("near_plane", shadowMap.near_plane);
		quadShader.setFloat("far_plane", shadowMap.far_plane);
		shadowMap.bindForReading(GL_TEXTURE0);

		this->scene.drawDebugQuad();*/

		// Shader modification
		/*Shader blockShader = Manager::getShader("block");
		blockShader.use();
		blockShader.setVec3("viewPos", position);
		blockShader.setMat4("projection", projection);
		blockShader.setMat4("view", view);

		Shader aabbShader = Manager::getShader("aabb");
		aabbShader.use();
		aabbShader.setMat4("projection", projection);
		aabbShader.setMat4("view", view);

		*/

		// Rendering the objects
		//this->scene.draw(aabbDebug);

		// Text rendering
		if (paused) textRenderer->renderText("PAUSED", 15.f, 60.f, 0.5f, glm::vec3(0.0, 0.0f, 1.0f));
		textRenderer->renderText("Mephisto %" + std::to_string((int)enemy->getLife()), 870.f, 1000.f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f));
		textRenderer->renderText("FPS:" + std::to_string((int)ceil(1.0f / deltaTime)), 15.f, 1050.f, 0.35f, glm::vec3(1.0, 0.0f, 0.0f));
		textRenderer->renderText("HEALTH: %" + std::to_string((int)player->getLife()), 15.f, 25.f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

		break;
	case GAME_WIN:
		/*glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
		textRenderer->renderText("YOU WIN", 800.f, 500.f, 1.f, glm::vec3(1.0, 0.0f, 0.0f));
		break;
	case GAME_LOSE:
	/*	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
		textRenderer->renderText("YOU LOSE", 800.f, 500.f, 1.f, glm::vec3(1.0, 0.0f, 0.0f));
		break;
	}
}
void Game::checkCollisions(float deltaTime)
{
	bool intersectedFaces[6] = { 0,0,0,0,0,0 };
	
	// player melee attack vs enemy

	if (player->doingMeleeAttack())
	{
		Collision playerMeleeCollision = player->currentAABB.intersectAABB(enemy->currentAABB);
		if (std::get<0>(playerMeleeCollision))
		{
			enemy->decreaseLife(20.f);
		}
	}

	// enemy vs players's projectiles
	Collision enemyProjectileCollision;
	for (Projectile * projectile : player->projectiles)
	{
		if (projectile->isAvailable())
		{
			enemyProjectileCollision = enemy->currentAABB.intersectAABB(projectile->currentAABB);
			if (std::get<0>(enemyProjectileCollision))
			{
				projectile->decreaseLife(1.f);

				ProjectileEffect effect = projectile->getEffect();
				enemy->decreaseLife(2.f);
			}
		}
	}

	// player vs enemy's projectiles

	Collision playerProjectileCollision;
	if (!player->isInvisible())
	{
		for (Projectile * projectile : enemy->projectiles)
		{
			if (projectile->isAvailable())
			{
				playerProjectileCollision = player->currentAABB.intersectAABB(projectile->currentAABB);
				if (std::get<0>(playerProjectileCollision))
				{
					projectile->decreaseLife(1.f);

					ProjectileType t = projectile->getProjectileType();
					ProjectileEffect e = projectile->getEffect();
					if (t == BULLET && e == ATTACKER)
					{
						player->decreaseLife(20.f);
					}
					else if (t == RANDOM && e == ATTACKER)
					{
						player->decreaseLife(10.f);
					}
					else if (e == PUSHER)
					{
						glm::vec3 diff = -glm::normalize(enemy->getPosition() - player->getPosition()) * 0.8f;

						player->increasePosition(glm::vec3(diff.x, 0.f, diff.z));
					}
					else if (e == PULLER)
					{
						glm::vec3 diff = glm::normalize(enemy->getPosition() - player->getPosition()) * 0.4f;

						player->increasePosition(glm::vec3(diff.x, 0.f, diff.z));
					}
				}
			}
		}
	}

	// player & projectiles vs environment

	for (auto& block : scene.sceneGraph)
	{
		if (block->isAvailable())
		{
			// PROJECTILES VS ENV
			bool flag0 = true;
			Collision projectileCollision;

			for (Projectile * projectile : enemy->projectiles)
			{
				if (projectile->isAvailable() && projectile->isEnvReactive())
				{
					projectileCollision = projectile->currentAABB.intersectAABB(block->currentAABB);
					if (std::get<0>(projectileCollision))
					{
						float damage;
						ProjectileType t = projectile->getProjectileType();
						ProjectileEffect e = projectile->getEffect();
						if (t == BULLET && e == ATTACKER) damage = 50.f;
						else if (t == AREA_OF_EFFECT) damage = 100.f;
						else if (t == RANDOM && e == ATTACKER) damage = 2.f;
						else damage = 0.f;

						projectile->decreaseLife(1.f);
						if (!block->decreaseLife(damage)) flag0 = false;
					}
				}
			}

			for (Projectile * projectile : player->projectiles)
			{
				if (projectile->isAvailable() && projectile->isEnvReactive())
				{
					projectileCollision = projectile->currentAABB.intersectAABB(block->currentAABB);
					if (std::get<0>(projectileCollision))
					{
						float damage;
						ProjectileType t = projectile->getProjectileType();
						ProjectileEffect e = projectile->getEffect();
						if (t == BULLET && e == ATTACKER) damage = 20.f;
						else if (t == AREA_OF_EFFECT && e == ATTACKER) damage = 10.f;
						else if (t == RANDOM && e == ATTACKER) damage = 2.f;
						else damage = 0.f;

						projectile->decreaseLife(1.f);
						if (!block->decreaseLife(damage)) flag0 = false;
					}
				}
			}

			// PLAYER VS ENV
			if (flag0)
			{
				Collision playerCollision = player->currentAABB.intersectAABB(block->currentAABB);
				if (std::get<0>(playerCollision))
				{

					CollisionDirection dir = std::get<1>(playerCollision);
					glm::vec3 normal = std::get<2>(playerCollision);
					if (intersectedFaces[0] == false && dir == C_LEFT) // +x reaction
					{
						if (player->currentAABB.getMinExtent().y < block->getPosition().y + block->currentAABB.height * 0.8f)
						{
							float difference = block->currentAABB.getMaxExtent().x - player->currentAABB.getMinExtent().x;
							player->increasePosition(glm::vec3(difference + 0.01f, 0.f, 0.f));
							intersectedFaces[0] = true;
						}
						/*else if (player->currentAABB.getMinExtent().x < block->getPosition().x + block->currentAABB.widthX * 0.8f)
							dir = C_BOTTOM;*/
					}
					else if (intersectedFaces[1] == false && dir == C_RIGHT) // -x reaction
					{
						if (player->currentAABB.getMinExtent().y < block->getPosition().y + block->currentAABB.height * 0.8f)
						{
							float difference = block->currentAABB.getMinExtent().x - player->currentAABB.getMaxExtent().x;
							player->increasePosition(glm::vec3(difference - 0.01f, 0.f, 0.f));
							intersectedFaces[1] = true;
						}
						/*else if (player->currentAABB.getMaxExtent().x < block->getPosition().x - block->currentAABB.widthX * 0.8f)
							dir = C_BOTTOM;*/
					}

					if (intersectedFaces[4] == false && dir == C_FAR) // +z reaction
					{
						if (player->currentAABB.getMinExtent().y < block->getPosition().y + block->currentAABB.height * 0.8f)
						{
							float difference = block->currentAABB.getMaxExtent().z - player->currentAABB.getMinExtent().z;
							player->increasePosition(glm::vec3(0.f, 0.f, difference + 0.01f));
							intersectedFaces[4] = true;
						}
						/*else if (player->currentAABB.getMinExtent().z > block->getPosition().z + block->currentAABB.widthZ)
							dir = C_BOTTOM;*/
					}
					else if (intersectedFaces[5] == false && dir == C_CLOSE) // -z reaction
					{
						if (player->currentAABB.getMinExtent().y < block->getPosition().y + block->currentAABB.height * 0.8f)
						{
							float difference = block->currentAABB.getMinExtent().z - player->currentAABB.getMaxExtent().z;
							player->increasePosition(glm::vec3(0.f, 0.f, difference - 0.01f));
							intersectedFaces[5] = true;
						}
						/*else if (player->currentAABB.getMaxExtent().z > block->getPosition().z - block->currentAABB.widthZ)
							dir = C_BOTTOM;*/
					}

					if (intersectedFaces[2] == false && dir == C_BOTTOM) // +y reaction
					{
						float difference = block->currentAABB.getMaxExtent().y - player->currentAABB.getMinExtent().y;
						player->increasePosition(glm::vec3(0.f, difference, 0.f));
						/*player->LIMIT = block->currentAABB.getMaxExtent().y + 0.001f;*/
						intersectedFaces[2] = true;
					}
					else if (intersectedFaces[3] == false && dir == C_TOP) // -y reaction
					{
						float difference = block->currentAABB.getMinExtent().y - player->currentAABB.getMaxExtent().y;
						player->increasePosition(glm::vec3(0.f, difference, 0.f));
						intersectedFaces[3] = true;
					}
				}
			}
		}
	}
	if (intersectedFaces[0] == true || intersectedFaces[1] == true || intersectedFaces[2] == true || intersectedFaces[3] == true || intersectedFaces[4] == true || intersectedFaces[5] == true)
		player->currentAABB.setCollision(true);
	//if (enemyHit) enemy->currentAABB.setCollision(true);
}
bool Game::isTerminated()
{
	return terminated;
}

