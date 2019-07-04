#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Quaternion.h"

/*
Represents player position, orientation and direction
*/
class Camera
{
public:

	Camera();

	/*
	Returns the view matrix taking into account player position and orientation
	*/
	glm::mat4 getView();

	/*
	Set vector position of player
	*/
	void setPosition(glm::vec3 position);

	/*
	Set vector direction of player
	*/
	void setDirection(glm::vec3 direction);

	/*
	Add deltaPosition to player position
	*/
	void movePosition(glm::vec3 deltaPosition);

	/*
	Rotate player by a quaternion
	*/
	void moveDirection(Quaternion rot);

	/*
	Return vector direction of player
	*/
	glm::vec3 getDirection();

	/*
	Returns vector position of player
	*/
	glm::vec3 getPosition();

	~Camera();

private:

	/*
	Stores vector position of player
	*/
	glm::vec3 position;

	/*
	Stores vector direction of player, defined absolutely (subtract position to get actual direction)
	*/
	glm::vec3 direction;

};

