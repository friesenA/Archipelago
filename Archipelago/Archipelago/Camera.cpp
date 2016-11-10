#include "stdHeader.h"
#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, GLfloat yaw, GLfloat pitch) : speed(SPEED), sensitivity(SENSITIVITY), yaw(yaw), pitch(pitch)
{
	this->position = position;
	this->worldUp = glm::normalize(up);
	this->forward = glm::normalize(forward);

	//Calculate the remaining attribute vectors
	this->recalculateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->position, this->position + this->forward, this->up);
}

void Camera::translateCamera(Movement direction)
{
	if (direction == FORWARD)
		this->position += this->forward;

	if (direction == BACKWARD)
		this->position -= this->forward;

	if (direction == RIGHT)
		this->position += this->right;

	if (direction == LEFT)
		this->position -= this->right;

	if (direction == UP)
		this->position += this->up;

	if (direction == DOWN)
		this->position -= this->up;
}

void Camera::rotateCamera(GLfloat xOffset, GLfloat yOffset)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	this->yaw += xOffset;
	this->pitch += yOffset;

	//Constrain the pitch so the screen doesn't flip
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	recalculateVectors();
}

void Camera::recalculateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->forward = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->forward, worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->forward));
}

