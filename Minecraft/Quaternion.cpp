#include "Quaternion.h"

Quaternion Quaternion::identity = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion Quaternion::fromAngle(glm::vec3 eulerAngles) {
	float roll = eulerAngles.x / 2;
	float pitch = eulerAngles.y / 2;
	float yaw = eulerAngles.z / 2;
	float x = glm::cos(glm::radians(roll)) * glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)) + glm::sin(glm::radians(roll)) * glm::sin(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	float y = glm::sin(glm::radians(roll)) * glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)) - glm::cos(glm::radians(roll)) * glm::sin(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	float z = glm::cos(glm::radians(roll)) * glm::sin(glm::radians(pitch)) * glm::cos(glm::radians(yaw)) + glm::sin(glm::radians(roll)) * glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	float w = glm::cos(glm::radians(roll)) * glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw)) - glm::sin(glm::radians(roll)) * glm::sin(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::normalized() {
	float len = glm::sqrt(x * x + y * y + z * z + w * w);
	return Quaternion(x / len, y / len, z / len, w / len);
}

Quaternion Quaternion::angleAxis(glm::vec3 axis, float angle) {
	float factor = glm::sin(glm::radians(angle / 2.0f));
	float x = axis.x * factor;
	float y = axis.y * factor;
	float z = axis.z * factor;
	float w = glm::cos(glm::radians(angle / 2.0f));
	return Quaternion(x, y, z, w);
}

Quaternion Quaternion::operator*(Quaternion q) {
	float qx = x * q.x - y * q.y - z * q.z - w * q.w;
	float qy = x * q.y + y * q.x + z * q.w - w * q.z;
	float qz = x * q.z - y * q.w + z * q.x + w * q.y;
	float qw = x * q.w + y * q.z - z * q.y + w * q.x;
	return Quaternion(qx, qy, qz, qw);
}

Quaternion Quaternion::conjugate() {
	return Quaternion(x, -y, -z, -w);
}

glm::vec3 Quaternion::operator*(glm::vec3 v) {
	Quaternion q = ((*this) * Quaternion(0, v.x, v.y, v.z)) * conjugate();
	return glm::vec3(q.y, q.z, q.w);
}

Quaternion::~Quaternion()
{
}
