#include "object.h"

#include <math.h>

void GameObject::increaseRotation(float x, float y , float z)
{	
	rotX += x;
	rotY += y;
	rotZ += z;
	transformed = true;
}

void GameObject::increasePosition(glm::vec3 position)
{
	this->position += position;
	transformed = true;
}

float GameObject::getRotX()
{
	return this->rotX;
}

float GameObject::getRotY()
{
	return this->rotY;
}

float GameObject::getRotZ()
{
	return this->rotZ;
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
	if (position.x != NULL)
		this->position.x = position.x;
	if (position.y != NULL)
		this->position.y = position.y;
	if (position.z != NULL)
		this->position.z = position.z;
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

std::string GameObject::getType()
{
	return type;
}

void GameObject::drawAABB(Shader shader)
{
	this->currentAABB.draw(shader);
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

glm::mat4 GameObject::getAABBTransform()
{
	return transformMatrixAABB;
}

glm::mat4 GameObject::getTransform()
{
	if (transformed) {
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, this->position);
		glm::mat4 cTransform = glm::scale(transform, this->scale);
		transform = glm::rotate(transform, this->rotationDegree,glm::vec3(rotX,rotY,rotZ));
		transform = glm::scale(transform, this->scale);
		transformMatrix = transform;
		transformMatrixAABB = cTransform;
		this->currentAABB.updateAABB(cTransform, position);
		//this->currentAABB.updateAABB(transform, position);
		transformed = false;
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

Block::Block(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model, bool destructable,string type) {
	this->type = type;
	this->transformed = true;
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->scale = scale;
	this->destructable = destructable;
	this->available = true;
	this->model = model;

	this->durability = 100;

	this->rotX = 0.f;
	this->rotY = 1.f;
	this->rotZ = 0.f;

	this->currentAABB = model->aabb;
}

bool Block::increaseLife(float v)
{
	durability += v;
	if (durability <= 0.f)
		return false;
	else return true;
}

bool Block::decreaseLife(float v)
{
	durability -= v;
	if (durability <= 0.f)
		return false;
	else return true;
}

bool Block::isAvailable()
{
	if (durability <= 0.f || !available)
		return false;
	else return true;
}
