#pragma once
#include <glm/glm.hpp>

class Quaternion
{
public:
	Quaternion(float x, float y, float z, float w);
	static Quaternion fromAngle(glm::vec3 eulerAngles);
	static Quaternion angleAxis(glm::vec3 axis, float angle);
	static Quaternion identity;
	Quaternion normalized();
	Quaternion operator*(Quaternion q);
	glm::vec3 operator*(glm::vec3 v);
	Quaternion conjugate();
	~Quaternion();
	float x, y, z, w;
};

