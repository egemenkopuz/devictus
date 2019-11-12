#include "object.h"

#include <math.h>

void GameObject::increaseRotation(glm::vec3 rotation)
{
	this->rotation += rotation;
	transformed = true;
}

void GameObject::increasePosition(glm::vec3 position)
{
	this->position += position;
	transformed = true;
}

float GameObject::getRotationDegree()
{
	return rotationDegree;
}

void GameObject::setRotationDegree(float degree)
{
	this->rotationDegree = degree;
	transformed = true;
}

glm::vec3 GameObject::getScale()
{
	return scale;
}

void GameObject::setScale(glm::vec3 scale)
{
	this->scale = scale;
	transformed = true;
}

glm::vec3 GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(glm::vec3 position)
{
	this->position = position;
	transformed = true;
}

void GameObject::disable()
{
	available = false;
}

void GameObject::setDestructable(bool b)
{
	destructable = b;
}

bool GameObject::isDestructable()
{
	return destructable;
}

bool GameObject::isAvailable()
{
	return available;
}

std::string GameObject::getType()
{
	return type;
}

void GameObject::setHealth(int hp, bool add)
{
	if (add)
		health += hp;
	else health = hp;
}

int GameObject::getHealth()
{
	return health;
}

bool GameObject::getDamage(int damage)
{
	if (destructable) {
		health -= damage;
		if (health <= 0) {
			available = false;
			return false;
		}
		return true;
	}
	return true;
}

void GameObject::drawModel(Shader shader)
{
	this->model->Draw(shader);
}

GameObject::~GameObject()
{
	this->model = nullptr;
	this->parent = nullptr;
	for (auto iter : this->children)
		iter = nullptr;
	children.clear();
}

glm::mat4 GameObject::getTransform()
{
	if (transformed) {
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, this->position);
		transform = glm::rotate(transform, glm::radians(this->rotationDegree), glm::vec3(0.f, 0.f, 1.f));
		transform = glm::scale(transform, this->scale);
		transformMatrix = transform;
		transformed = false;
		// TODO CHILDREN
	}
	return transformMatrix;;
}

void GameObject::attachParent(GameObject * parent)
{
	this->parent = parent;
}

void GameObject::addChild(GameObject * child)
{
	child->attachParent(this);
	children.push_back(child);
}


void Player::move(bool keys[], float deltaTime)
{
	if (keys[MOVE_UP] == true && !jumping)
	{
		currentJumpSpeed = JUMPING_SPEED;
		jumping = true;
	}
	else currentJumpSpeed = 0.f;

	if (keys[MOVE_FORWARD] == true)
	{
		currentWalkingSpeed = WALKING_SPEED;
	}
	else if (keys[MOVE_BACKWARD] == true)
	{
		currentWalkingSpeed = -WALKING_SPEED;
	}
	else currentWalkingSpeed = 0.f;

	if (keys[MOVE_LEFT] == true)
	{
		currentTurnSpeed = -TURNNING_SPEED;
	}
	else if (keys[MOVE_RIGHT] == true)
	{
		currentTurnSpeed = TURNNING_SPEED;
	}
	else currentTurnSpeed = 0.f;

	increaseRotation(glm::vec3(0.f, currentTurnSpeed * deltaTime, 0.f));
	setRotationDegree(currentJumpSpeed * deltaTime);
	float distance = currentWalkingSpeed * deltaTime;
	float dx = (float)(distance * sin(rotation.y));
	float dz = (float)(distance * cos(rotation.y));
	currentJumpSpeed += GRAVITY * deltaTime;
	increasePosition(glm::vec3(dx, currentJumpSpeed * deltaTime, dz));
	if (position.y < LIMIT) {
		currentJumpSpeed = 0.f;
		position.y = LIMIT;
		jumping = false;
	}
}

Player::Player(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model) {
	this->type = "block";
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
	this->health = 100;
	this->casting = false;
	this->destructable = true;
	this->available = true;
	this->model = model;

	this->currentWalkingSpeed = 0.f;
	this->currentTurnSpeed = 0.f;
	this->currentJumpSpeed = 0.f;

	this->jumping = false;

	this->rotation = glm::vec3(0.f, 0.f, 0.f);
}

void Player::transform() {

}

Enemy::Enemy(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model) {
	this->type = "block";
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
	this->health = 100;
	this->casting = false;
	this->destructable = true;
	this->available = true;
	this->model = model;

	this->rotation = glm::vec3(0.f, 0.f, 0.f);
}

void Enemy::transform() {

}

Block::Block(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model, bool destructable) {
	this->type = "block";
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
	this->destructable = destructable;
	this->available = true;
	this->model = model;

	this->rotation = glm::vec3(0.f, 0.f, 0.f);
}
void Block::transform() {

}