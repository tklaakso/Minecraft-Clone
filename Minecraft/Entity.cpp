#include "Entity.h"

Entity::Entity()
{

	transform = glm::mat4(1.0f);
	pos = glm::mat4(1.0f);
	rot = glm::mat4(1.0f);
	scale = glm::mat4(1.0f);

}

void Entity::setPosition(glm::vec3 position) {

	pos = glm::translate(glm::mat4(1.0f), position);
	updateTransform();

}

void Entity::setRotation(float angle, glm::vec3 axis) {

	rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
	updateTransform();

}

void Entity::setScale(float scaleFactor) {
	
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	updateTransform();

}

void Entity::updateTransform() {
	
	transform = pos * rot * scale;

}

bool Entity::render(Shader* shader, ViewFrustum* frustum) {

	shader->setMat4("model", transform);
	return draw(frustum);

}

Entity::~Entity()
{
}
