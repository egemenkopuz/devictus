#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "object.h"

enum CameraMovement {FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN, NONE};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float ZOOM = 45.0f;

class Camera {
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	float Yaw;	// x circle direction left-right
	float Pitch;	// y circle direction up-down

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseMovement(float xoffset, float yoffset, glm::vec3 playerPosition);
private:
	void updateCameraVectors();
};


class Camera3rdPerson {
private:
	glm::vec3 position;
	float angle = 0.f;

	Player * player = nullptr;
	float distanceFromPlayer = 5.f;

	float yaw;		// x circle direction
	float pitch = 30.f;	// y circle direction
	float roll;		// z circle direction
public:
	Camera3rdPerson(Player * player);
	void processMovement();
	void processMouse(float yoffset);
	glm::vec3 getPosition();
	glm::mat4 getViewMatrix();
};

#endif