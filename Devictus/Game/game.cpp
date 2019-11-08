#include "game.h"

Game::Game()
{
	this->terminated = false;
	this->currentDiff = PEASANT;
	this->state = GAME_MENU;
}

Game::~Game()
{

}

void Game::processMouse(float x, float y)
{
	if (state == GAME_ACTIVE)
		sceneCamera->ProcessMouseMovement(x, y);
}

void Game::processKey(KEY key, float deltaTime)
{
	if (state == GAME_MENU) {
		if (key == KEY_ESCAPE) {
#ifdef DEBUG
			std::cout << "GAME_MENU:PRESSED:KEY_ESCAPE, terminating..." << std::endl;
#endif 
			terminated = true;
		}
		else if (key == KEY_ENTER) {
			if (currentDiff == PEASANT) {
#ifdef DEBUG
				std::cout << "GAME_MENU:PRESSED:KEY_ENTER, Peasent Level Scene is initializing from path: \"./Levels/peasant.lvl\"" << std::endl;
#endif 
				scene.init(currentDiff, "./Levels/peasant.lvl");
			}
			else if (currentDiff == CHALLENGER) {
#ifdef DEBUG
				std::cout << "GAME_MENU:PRESSED:KEY_ENTER, Challenger Level Scene is initializing from path: \"./Levels/challenger.lvl\"" << std::endl;
#endif 
				scene.init(currentDiff, "./Levels/challenger.lvl");
			}
			else {
#ifdef DEBUG
				std::cout << "GAME_MENU:PRESSED:KEY_ENTER, God Level Scene is initializing from path: \"./Levels/god.lvl\"" << std::endl;
#endif 
				scene.init(currentDiff, "./Levels/god.lvl");
			}
			sceneCamera = scene.sceneCamera;
			uiCamera = scene.uiCamera;
			player = scene.player;
			enemy = scene.enemy;

			state = GAME_ACTIVE;
		}
		else if (key == KEY_W) {
			if (currentDiff == CHALLENGER) {
#ifdef DEBUG
				std::cout << "GAME_MENU:PRESSED:KEY_W, Selected PEASANT level difficulty" << std::endl;
#endif 
				currentDiff = PEASANT;
			}
			else if (currentDiff == GOD) {
#ifdef DEBUG
				std::cout << "GAME_MENU:PRESSED:KEY_W, Selected CHALLENGER level difficulty" << std::endl;
#endif 
				currentDiff = CHALLENGER;
			}
		}
		else if (key == KEY_S) {
			if (currentDiff == PEASANT) {
#ifdef DEBUG
				std::cout << "GAME_MENU:PRESSED:KEY_S, Selected CHALLENGER level difficulty" << std::endl;
#endif 
				currentDiff = CHALLENGER;
			}
			else if (currentDiff == CHALLENGER) {
#ifdef DEBUG
				std::cout << "GAME_MENU:PRESSED:KEY_S, Selected GOD level difficulty" << std::endl;
#endif 
				currentDiff = GOD;
			}
		}
	}
	else if (state == GAME_ACTIVE) {
		if (key == KEY_ESCAPE) 
			state = GAME_MENU;
		else if (key == KEY_W)
			sceneCamera->ProcessKeyboard(FORWARD, deltaTime);
		else if (key == KEY_A) 
			sceneCamera->ProcessKeyboard(LEFT, deltaTime);
		else if (key == KEY_S)
			sceneCamera->ProcessKeyboard(BACKWARD, deltaTime);
		else if (key == KEY_D)
			sceneCamera->ProcessKeyboard(RIGHT, deltaTime);
		else if (key == KEY_LEFT);

		else if (key == KEY_RIGHT);

		else if (key == KEY_SPACE) 
			sceneCamera->ProcessKeyboard(UP, deltaTime);
		else if (key == KEY_LEFT_SHIFT) 
			sceneCamera->ProcessKeyboard(DOWN, deltaTime);

	}
	if (state == GAME_WIN) {
		if (key == KEY_ESCAPE || key == KEY_ENTER)
			state = GAME_MENU;

	}
	if (state == GAME_LOSE) {
		if (key == KEY_ESCAPE || key == KEY_ENTER)
			state = GAME_MENU;
	}
}



void Game::init()
{
	std::cout << "GAME:INITILIZATION" << std::endl;
	Manager::loadShader("./Shaders/block.vert", "./Shaders/block.frag", "block");
	Manager::loadShader("./Shaders/lightblock.vert", "./Shaders/lightblock.frag", "lightblock");

	Shader blockShader = Manager::getShader("block");
	blockShader.use();
	blockShader.setFloat("material.shininess", 32.0f);
	blockShader.setFloat("material.diffuse", 0.5f);
	blockShader.setFloat("material.specular", 1.0f);

	// directional light
	blockShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	blockShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	blockShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	blockShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	blockShader.setVec3("pointLights[0].position", glm::vec3(0.0f, 5.0f, 0.0f));
	blockShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	blockShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	blockShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	blockShader.setFloat("pointLights[0].constant", 1.0f);
	blockShader.setFloat("pointLights[0].linear", 0.09);
	blockShader.setFloat("pointLights[0].quadratic", 0.032);

	this->state = GAME_MENU;
}

void Game::update(float deltaTime)
{
	if (this->state == GAME_MENU) {

	}
	if (this->state == GAME_ACTIVE) {

	}
	if (this->state == GAME_WIN) {

	}
	if (this->state == GAME_LOSE) {

	}
}

void Game::render(float deltaTime)
{
	if (this->state == GAME_MENU) {


	}
	if (this->state == GAME_ACTIVE) {
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(sceneCamera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = sceneCamera->GetViewMatrix();

		Shader blockShader = Manager::getShader("block");
		blockShader.use();
		blockShader.setVec3("viewPos", sceneCamera->Position);
		blockShader.setMat4("projection", projection);
		blockShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		blockShader.setMat4("model", model);
		this->scene.draw();
	}
	if (this->state == GAME_WIN) {


	}
	if (this->state == GAME_LOSE) {

	}
}
void Game::checkCollisions()
{

}
bool Game::isTerminated()
{
	return terminated;
}

