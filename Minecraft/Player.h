#pragma once
#include <iostream>
#include <algorithm>
#include "Camera.h"
#include "Input.h"
#include "Constants.h"
#include "Math.h"
#include "Shader.h"

class Player
{
public:
	Player(GLFWwindow* window);
	void update(Shader* shader, double deltaTime);
	glm::mat4 getView();
	~Player();
private:
	float mouseX, mouseY;
	Camera* cam;
	GLFWwindow* window;
	Quaternion direction = Quaternion::identity;
	bool captureMouse = false;
	const float speed = 20.0f;
};

