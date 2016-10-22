#include "stdHeader.h"
#include "Camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : speed(SPEED), sensitivity(SENSITIVITY), worldUp(up)
{
	this->position = position;
	this->forward = glm::normalize(forward);

	//Derive the initial values of pitch and yaw from the given attribute vectors
	this->pitch = glm::acos(glm::dot(this->up, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 forwardProjectionXZ = glm::normalize(glm::vec3(this->forward.x, 0.0f, this->forward.y));   //need to test
	this->yaw = glm::acos(glm::dot(forwardProjectionXZ, glm::vec3(-1.0f, 0.0f, 0.0f)));

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
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);

	this->forward = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->forward, worldUp));
	this->up = glm::normalize(glm::cross(this->forward, this->right));
}

