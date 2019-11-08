#include "object.h"

GameObject::~GameObject()
{
	delete this->model;
}


Player::Player(glm::vec3 position, Model * model) {
	this->type = "block";

	this->position = position;
	this->health = 100;
	this->casting = false;
	this->destructable = true;
	this->available = true;
	this->model = model;
}

void Player::transform() {

}

Enemy::Enemy(glm::vec3 position, Model * model) {
	this->type = "block";

	this->position = position;
	this->health = 100;
	this->casting = false;
	this->destructable = true;
	this->available = true;
	this->model = model;
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