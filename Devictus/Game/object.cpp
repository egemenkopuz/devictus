#include "object.h"

GameObject::~GameObject()
{
	delete this->model;
}


Player::Player(glm::vec3 position) {
	//this->type = "player";
	this->type = "block";

	this->position = position;
	this->health = 100;
	this->casting = false;
	this->destructable = true;
	this->available = true;

	// model load player object
	//this->model = new Model("./Objects/Player/player.obj");
	this->model = new Model("./Objects/Block/block.obj");
}

void Player::transform() {

}

Enemy::Enemy(glm::vec3 position) {
	//this->type = "enemy";
	this->type = "block";

	this->position = position;
	this->health = 100;
	this->casting = false;
	this->destructable = true;
	this->available = true;

	// model load enemy object
	//this->model = new Model("./Objects/Enemy/enemy.obj");
	this->model = new Model("./Objects/Block/block.obj");
}

void Enemy::transform() {

}

Block::Block(glm::vec3 position,Model * model, bool destructable) {
	this->type = "block";
	this->position = position;
	this->destructable = destructable;
	this->available = true;

	this->model = model;
}
void Block::transform() {

}