#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(MOUSE_SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(MOUSE_SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == UP)
		Position += Up * velocity;
	if (direction == DOWN)
		Position -= Up * velocity;
}

//void Camera::ProcessPlayerMovement()
//{
//	if (player != nullptr)
//	{
//		float theta = player->getRotationDegree();
//		float horizontalDistance = distanceFromPlayer * cos(glm::radians(theta));
//		float verticalDistance = distanceFromPlayer * sin(glm::radians(theta));
//
//		float offsetX = verticalDistance * sin(glm::radians(theta));
//		float offsetZ = horizontalDistance * cos(glm::radians(theta));
//		Position.x = player->getPosition().x - offsetX;
//		Position.z = player->getPosition().z - offsetZ;
//		Position.y = player->getPosition().y + 2.5f;
//
//		//Yaw = 180.f - player->getRotation().y;
//		if (Pitch > 89.0f)
//			Pitch = 89.0f;
//		if (Pitch < -89.0f)
//			Pitch = -89.0f;
//	}
//	// TODO
//	updateCameraVectors();
//}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, glm::vec3 playerPosition)
{
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

// 3RD PERSON CAMERA

Camera3rdPerson::Camera3rdPerson(Player * player)
{
	this->player = player;
}

void Camera3rdPerson::processMovement()
{
	float verticalDistance = distanceFromPlayer * sin(glm::radians(pitch));
	float horizontalDistance = distanceFromPlayer * cos(glm::radians(pitch));

	float theta = player->getRotationDegree();
	float offsetX = horizontalDistance * sin(theta);
	float offsetZ = horizontalDistance * cos(theta);

	position.x = player->getPosition().x - offsetX;
	position.z = player->getPosition().z - offsetZ;
	position.y = player->getPosition().y + verticalDistance;

	yaw = 180.f - theta;
}

void Camera3rdPerson::processMouse(float yoffset)
{
	yoffset *= MOUSE_SENSITIVITY;
	pitch += yoffset;

	if (pitch > 20.f) pitch = 20.f;
	if (pitch < 0.f) pitch = 0.f;
}

glm::vec3 Camera3rdPerson::getPosition()
{
	return position;
}

glm::mat4 Camera3rdPerson::getViewMatrix()
{
	return glm::lookAt(position, player->getPosition(), glm::vec3(0.f,1.f,0.f));
}
