#pragma once
#include <iostream>
#include <algorithm>
#include "Camera.h"
#include "Input.h"
#include "Constants.h"
#include "Math.h"
#include "Shader.h"
#include "World.h"

class Player
{
public:
	Player(GLFWwindow* window, World* world);
	void update(Shader* shader, double deltaTime);
	glm::mat4 getView();
	~Player();
private:
	float mouseX, mouseY;
	float chunkPosX, chunkPosY;
	World* world;
	Camera* cam;
	GLFWwindow* window;
	Quaternion direction = Quaternion::identity;
	bool captureMouse = false;
	const float speed = 20.0f;
};

