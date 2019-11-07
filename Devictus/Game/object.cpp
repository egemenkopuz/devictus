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
	this->avaliable = true;

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
	this->avaliable = true;

	// model load enemy object
	//this->model = new Model("./Objects/Enemy/enemy.obj");
	this->model = new Model("./Objects/Block/block.obj");
}

void Enemy::transform() {

}

Block::Block(glm::vec3 position, bool destructable) {
	this->type = "block";
	this->position = position;
	this->destructable = destructable;
	this->avaliable = true;

	this->model = new Model("./Objects/Block/block.obj");
}
void Block::transform() {

}