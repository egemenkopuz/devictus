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
		else player->rotate(x,y, deltaTime);
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

		if (key == KEY_ESCAPE) {
			terminated = true;
		}

		if (key == KEY_SPACE) {
			if (paused) freeCamera->ProcessKeyboard(UP, deltaTime);
			else movementKeys[MOVE_UP] = true;
		}

		if (key == KEY_LEFT_SHIFT) {
			if (paused) freeCamera->ProcessKeyboard(DOWN, deltaTime);
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
			// light attack
		}
		if (key == KEY_RIGHT) {
			// heavy attack
		}
	}
	else if (key == KEY_ESCAPE || key == KEY_ENTER) terminated = true;
}

void Game::init()
{
	std::cout << "GAME:INITILIZATION" << std::endl;
	Manager::loadShader("./Shaders/block.vert", "./Shaders/block.frag", "block");
	Manager::loadShader("./Shaders/lightblock.vert", "./Shaders/lightblock.frag", "lightblock");
	Manager::loadShader("./Shaders/text.vert", "./Shaders/text.frag", "text");

	Shader blockShader = Manager::getShader("block");
	blockShader.use();
	blockShader.setFloat("material.shininess", 32.0f);
	blockShader.setFloat("material.diffuse", 1.0f);
	blockShader.setFloat("material.specular", 1.0f);

	// directional light
	//blockShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//blockShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//blockShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//blockShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

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
				player->move(movementKeys, deltaTime);
				for (int b = 0; b < NUM_MOVE_KEYS; b++) movementKeys[b] = false;
				playerCamera->processMovement();
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

		// Shader modification
		Shader blockShader = Manager::getShader("block");
		blockShader.use();
		blockShader.setVec3("viewPos", position);
		blockShader.setMat4("projection", projection);
		blockShader.setMat4("view", view);

		// Rendering the objects
		this->scene.draw();

		// Text rendering
		if (paused) textRenderer->renderText("PAUSED", 870.f, 512.f, 0.5f, glm::vec3(0.0, 0.0f, 1.0f));
		textRenderer->renderText("Mephisto", 870.f, 1000.f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f));
		textRenderer->renderText("FPS:" + std::to_string((int)ceil(1.0f / deltaTime)), 15.f, 1050.f, 0.35f, glm::vec3(1.0, 0.0f, 0.0f));
		
		break;
	case GAME_WIN:
		break;
	case GAME_LOSE:
		break;
	}
}
void Game::checkCollisions()
{

}
bool Game::isTerminated()
{
	//return terminated;
	return false;
}

