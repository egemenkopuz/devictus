#include "game.h"

Game::Game()
{
	this->terminated = false;
	this->state = GAME_MENU;
}

Game::~Game()
{

}

void Game::processMouse(float x, float y)
{
	// camera movement
}

void Game::processKey(KEY key, float deltaTime)
{
	if (this->state == GAME_MENU) {
		if (key == KEY_ESCAPE)
			this->terminated = true;
		if (key == KEY_ENTER);

		if (key == KEY_W);

		if (key == KEY_S);

	}
	if (this->state == GAME_ACTIVE) {
		if (key == KEY_ESCAPE)
			this->state = GAME_MENU;
		if (key == KEY_W);

		if (key == KEY_A);

		if (key == KEY_S);

		if (key == KEY_D);

		if (key == KEY_LEFT);

		if (key == KEY_RIGHT);

		if (key == KEY_SPACE);

		if (key == KEY_LEFT_SHIFT);

	}
	if (this->state == GAME_WIN) {
		if (key == KEY_ESCAPE ||key == KEY_ENTER)
			this->state = GAME_MENU;

	}
	if (this->state == GAME_LOSE) {
		if (key == KEY_ESCAPE || key == KEY_ENTER)
			this->state = GAME_MENU;
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

	Manager::loadTexture("./Textures/block.jpg", "block","texture_diffuse");


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

