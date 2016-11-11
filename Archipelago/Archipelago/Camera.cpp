#include "stdHeader.h"
#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : speed(SPEED), sensitivity(SENSITIVITY)
{
	this->position = position;
	this->worldUp = glm::normalize(up);
	this->forward = glm::normalize(forward);

	this->yaw = atan2(this->forward.z, this->forward.x);
	this->pitch = asin(this->forward.y);

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
	if (this->pitch > glm::radians(89.0f))
		this->pitch = glm::radians(89.0f);
	if (this->pitch < glm::radians(-89.0f))
		this->pitch = glm::radians(-89.0f);

//	this->yaw = glm::radians(this->yaw);
//	this->pitch = glm::radians(this->pitch);

	recalculateVectors();
}

void Camera::recalculateVectors()
{
	glm::vec3 front;

	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);

	this->forward = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->forward, worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->forward));
}

