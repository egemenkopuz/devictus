#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <vector>
#include <glm/gtx/vector_angle.hpp>

#include "../Engine/texture.h"
#include "../Engine/shader.h"
#include "../Engine/model.h"

#include "../Utilities/definitions.h"

class GameObject {
protected:
	glm::mat4 transformMatrix;
	glm::mat4 transformMatrixAABB;
	bool transformed;

	float rotationDegree;
	float rotX, rotY, rotZ;
	glm::vec3 scale;
	glm::vec3 position;

	bool destructable;
	bool available;

	std::string type;

	Model * model;

	GameObject * parent;
	std::vector<GameObject*> children;


public:
	AABB currentAABB;

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

	virtual bool isAvailable() = 0;
	virtual bool increaseLife(float v) = 0;
	virtual bool decreaseLife(float v) = 0;
	virtual float getLife() = 0;

	std::string getType();

	void drawAABB(Shader shader);
	void drawModel(Shader shader);

	~GameObject();

	glm::mat4 getAABBTransform();
	glm::mat4 getTransform();

	void attachParent(GameObject * child);
	void addChild(GameObject * child);
};

class Block : public GameObject {	
private:
	float durability;
public:
	Block(glm::vec3 position, float rotationDegree, glm::vec3 scale, Model * model, bool indestructable,string type);
	bool increaseLife(float v);
	bool decreaseLife(float v);
	bool isAvailable();
	float getLife() { return durability / 100.f; }
};

#endif