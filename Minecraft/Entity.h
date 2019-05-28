#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "Shader.h"
#include "Math.h"

class Entity
{
public:
	Entity();
	bool render(Shader* shader, ViewFrustum* frustum);
	void setPosition(glm::vec3 position);
	void setRotation(float angle, glm::vec3 axis);
	void setScale(float scaleFactor);
	~Entity();
protected:
	glm::mat4 pos;
	glm::mat4 rot;
	glm::mat4 scale;
	glm::mat4 transform;
	virtual bool draw(ViewFrustum* frustum) = 0;
	void updateTransform();
};

