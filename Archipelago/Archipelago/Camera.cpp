#include "stdHeader.h"
#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : speed(SPEED), sensitivity(SENSITIVITY)
{
	this->position = position;
	this->worldUp = glm::normalize(up);
	this->forward = glm::normalize(forward);

	this->yaw = atan2(this->forward.z, this->forward.x);
	this->pitch = asin(this->forward.y);

	this->recalculateVectors();
}

glm::mat4 Camera::getViewMatrix(){
	return glm::mat4(glm::lookAt(this->position, this->position + this->forward, this->up));
}

void Camera::translateCamera(Movement direction){

	glm::vec3 nextPos(this->getNextPosition());

	if (direction == FORWARD)
		this->position += nextPos;
	if (direction == BACKWARD)
		this->position -= nextPos;
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

	recalculateVectors();
}

void Camera::climbAt(float y){
	position.y = y;
}

glm::vec3 Camera::getPosition()
{
		return position;
}

void Camera::recalculateVectors()
{

	glm::vec3 front;

	front.x = cos(this->yaw) * cos(this->pitch);
	front.y = sin(this->pitch);
	front.z = sin(this->yaw) * cos(this->pitch);

	this->forward = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->forward, this->worldUp));  
	this->up = glm::normalize(glm::cross(this->right, this->forward));

}

