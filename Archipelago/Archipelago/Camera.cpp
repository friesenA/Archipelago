#include "stdHeader.h"
#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : speed(SPEED), sensitivity(SENSITIVITY)
{
	this->position = position;
	this->worldUp = glm::normalize(up);
	this->forward = glm::normalize(forward);
	this->up = up;
	//this->worldUp = up;
	//this->forward = forward;

	this->yaw = atan2(this->forward.z, this->forward.x);
	this->pitch = asin(this->forward.y);


	//Calculate the remaining attribute vectors
	this->recalculateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::mat4(glm::mat3(glm::lookAt(this->position, this->position + this->forward, this->up)));
}

void Camera::translateCamera(Movement direction)
{
	if (direction == FORWARD)
		this->position += this->forward * SPEED;

	if (direction == BACKWARD)
		this->position -= this->forward * SPEED;

	if (direction == RIGHT) {
		//this->position += this->right * SPEED;
		GLfloat xoffset = 1.0f;

		GLfloat sensitivity = 0.40;	// Change this value to your liking
		xoffset *= sensitivity;

		yaw += xoffset;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw));
		front.z = sin(glm::radians(yaw));
		this->forward = glm::normalize(front);
	}

	if (direction == LEFT) {
		//this->position -= this->right * SPEED;
		GLfloat xoffset = -1.0f;

		GLfloat sensitivity = 0.40;	// Change this value to your liking
		xoffset *= sensitivity;

		yaw += xoffset;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw));
		front.z = sin(glm::radians(yaw));
		this->forward = glm::normalize(front);
	}
		

	if (direction == UP)
		this->position += this->up * SPEED;

	if (direction == DOWN)
		this->position -= this->up * SPEED;
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

	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	this->forward = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->forward, this->worldUp));  
	this->up = glm::normalize(glm::cross(this->right, this->forward));

}

