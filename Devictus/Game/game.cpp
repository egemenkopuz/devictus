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
		camera->ProcessMouseMovement(x, y);
}

void Game::processKey(KEY key, float deltaTime)
{
	if (state == GAME_MENU) {
		if (key == KEY_ESCAPE)
			terminated = true;
		if (key == KEY_ENTER) {
			if (currentDiff == PEASANT) {
				scene.init(currentDiff, "./Levels/peasant.lvl");
			}
			else if (currentDiff == CHALLENGER)
				scene.init(currentDiff, "./Levels/challenger.lvl");
			else
				scene.init(currentDiff, "./Levels/god.lvl");
			state = GAME_ACTIVE;
		}
		if (key == KEY_W) {
			if (currentDiff == CHALLENGER) {
				std::cout << "GAME_MENU:PRESSED:KEY_W, Peasant Difficulty is selected" << std::endl;
				currentDiff = PEASANT;
			}
			else if (currentDiff == GOD) {
				std::cout << "GAME_MENU:PRESSED:KEY_W, Challenger Difficulty is selected" << std::endl;
				currentDiff = CHALLENGER;
			}
		}
		if (key == KEY_S) {
			if (currentDiff == PEASANT) {
				std::cout << "GAME_MENU:PRESSED:KEY_S, Challenger Difficulty is selected" << std::endl;
				currentDiff = CHALLENGER;
			}
			else if (currentDiff == CHALLENGER) {
				std::cout << "GAME_MENU:PRESSED:KEY_S, God Difficulty is selected" << std::endl;
				currentDiff = GOD;
			}
		}
	}
	else if (state == GAME_ACTIVE) {
		if (key == KEY_ESCAPE)
			state = GAME_MENU;
		if (key == KEY_W)
			camera->ProcessKeyboard(FORWARD, deltaTime);
		if (key == KEY_A)
			camera->ProcessKeyboard(LEFT, deltaTime);
		if (key == KEY_S)
			camera->ProcessKeyboard(BACKWARD, deltaTime);
		if (key == KEY_D)
			camera->ProcessKeyboard(RIGHT, deltaTime);
		if (key == KEY_LEFT);

		if (key == KEY_RIGHT);

		if (key == KEY_SPACE)
			camera->ProcessKeyboard(UP, deltaTime);
		if (key == KEY_LEFT_SHIFT)
			camera->ProcessKeyboard(DOWN, deltaTime);

	}
	if (state == GAME_WIN) {
		if (key == KEY_ESCAPE ||key == KEY_ENTER)
			state = GAME_MENU;

	}
	if (state == GAME_LOSE) {
		if (key == KEY_ESCAPE || key == KEY_ENTER)
			state = GAME_MENU;
	}
}

void Game::processKey(KEY key, unsigned int level)
{
	if (this->state == GAME_MENU) {
		if (level == 0) { // PEASANT
			// init scenes
		}
		if (level == 1) { // CHALLENGER

		}
		if (level == 2) { // GOD

		}
	}
}



void Game::init()
{
	Manager::loadShader("./Shaders/block.vert", "./Shaders/block.frag","block");
	Manager::loadShader("./Shaders/lightblock.vert", "./Shaders/lightblock.frag", "lightblock");

	


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

