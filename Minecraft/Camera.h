#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Quaternion.h"

class Camera
{
public:
	Camera();
	glm::mat4 getView();
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	void movePosition(glm::vec3 deltaPosition);
	void moveDirection(Quaternion rot);
	glm::vec3 getDirection();
	glm::vec3 getPosition();
	~Camera();
private:
	glm::vec3 position;
	glm::vec3 direction;
};

