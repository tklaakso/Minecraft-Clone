#include "Camera.h"

Camera::Camera()
{

	position = glm::vec3(0.0f, 100.0f, 0.0f);

	direction = glm::vec3(0.0f, 0.0f, 1.0f);

}

glm::mat4 Camera::getView() {

	// Use glm to calculate view matrix
	return glm::lookAt(position, direction, glm::vec3(0.0f, 1.0f, 0.0f));

}

void Camera::setPosition(glm::vec3 position) {

	// Calculate direction relative to position
	glm::vec3 dir = direction - this->position;

	this->position = position;

	// Update direction to keep relative direction the same
	direction = this->position + dir;

}

void Camera::setDirection(glm::vec3 direction) {

	// Recall, direction is defined absolutely in terms of position
	this->direction = position + direction;

}

void Camera::movePosition(glm::vec3 deltaPosition) {

  // Add to position and update direction accordingly
	this->position += deltaPosition;

	this->direction += deltaPosition;

}

void Camera::moveDirection(Quaternion rot) {

	// Calculate relative direction and modify it instead of absolute direction
	glm::vec3 dir = direction - position;

	this->direction = rot * dir + position;

}

glm::vec3 Camera::getDirection() {

	return direction - position;

}

glm::vec3 Camera::getPosition() {

	return position;

}

Camera::~Camera()
{
}
