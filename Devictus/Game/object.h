#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <vector>

#include "../Engine/texture.h"
#include "../Engine/shader.h"
#include "../Engine/model.h"


#include "../Utilities/definitions.h"

class GameObject {
protected:
	glm::mat4 transformMatrix;
	bool transformed;

	float rotationDegree;
	float rotX = 0.f, rotY = 1.f, rotZ = 0.f;
	glm::vec3 scale;
	glm::vec3 position;

	bool destructable;
	bool available;

	std::string type;

	int health;

	Model * model;

	GameObject * parent;
	std::vector<GameObject*> children;
public:

	void increaseRotation(float x, float y, float z);
	void increasePosition(glm::vec3 position);

	float getRotX();
	float getRotY();
	float getRotZ();

	float getRotationDegree();
	void setRotationDegree(float degree);

	glm::vec3 getScale();
	void setScale(glm::vec3 scale);

	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);

	void disable();

	void setDestructable(bool b);
	bool isDestructable();

	bool isAvailable();
	std::string getType();
	
	void setHealth(int hp, bool add = false);
	int getHealth();

	bool getDamage(int damage);

	void drawModel(Shader shader);

	~GameObject();

	glm::mat4 getTransform();
	virtual void transform() = 0;

	void attachParent(GameObject * child);
	void addChild(GameObject * child);
};

class Player : public GameObject {
private:
	int stamina;
	bool casting;

	bool jumping;

	float currentWalkingSpeed;
	float currentJumpSpeed;
	float currentTurnSpeed;

	const float WALKING_SPEED = 1.f;
	const float TURNNING_SPEED = 5.f;
	const float GRAVITY = -25.f;
	const float JUMPING_SPEED = 25.f;

	const float LIMIT = 0.0f;	// WILL CHANGE WITH COLLUSION
public:
	void move(bool keys[], float deltaTime);

	Player(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model);
	void transform() override;
};

class Enemy : public GameObject {
private:
	bool casting;
public:
	Enemy(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model);
	void transform() override;
};

class Block : public GameObject {	
public:
	Block(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model, bool indestructable);
	void transform() override;
};

#endif