#include "Player.h"

Player::Player(GLFWwindow* window)
{
	cam = new Camera();
	this->window = window;
	mouseX = mouseY = 0;
}

void Player::update(Shader* shader, double deltaTime) {
	cam->setDirection(direction * glm::vec3(0.0f, 0.0f, 1.0f));
	if (Input::keyDown(GLFW_KEY_W)) {
		cam->movePosition(direction * glm::vec3(0.0f, 0.0f, 1.0f) * (float)deltaTime * speed);
	}
	if (Input::keyDown(GLFW_KEY_S)) {
		cam->movePosition(direction * glm::vec3(0.0f, 0.0f, -1.0f) * (float)deltaTime * speed);
	}
	if (Input::keyDown(GLFW_KEY_D)) {
		cam->movePosition(direction * glm::vec3(-1.0f, 0.0f, 0.0f) * (float)deltaTime * speed);
	}
	if (Input::keyDown(GLFW_KEY_A)) {
		cam->movePosition(direction * glm::vec3(1.0f, 0.0f, 0.0f) * (float)deltaTime * speed);
	}
	if (Input::keyDown(GLFW_KEY_SPACE)) {
		cam->movePosition(direction * glm::vec3(0.0f, 1.0f, 0.0f) * (float)deltaTime * speed);
	}
	if (Input::keyDown(GLFW_KEY_LEFT_SHIFT)) {
		cam->movePosition(direction * glm::vec3(0.0f, -1.0f, 0.0f) * (float)deltaTime * speed);
	}
	if (Input::keyClicked(GLFW_KEY_ESCAPE)) {
		captureMouse = !captureMouse;
		if (captureMouse) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	if (captureMouse) {
		mouseX += Input::mouseXDelta;
		mouseY += Input::mouseYDelta;
		mouseY = clamp(mouseY, -89.9f / MOUSE_SENSITIVITY, 89.9f / MOUSE_SENSITIVITY);
		Quaternion rotX = Quaternion::angleAxis(glm::vec3(0.0f, 1.0f, 0.0f), -mouseX * MOUSE_SENSITIVITY);
		Quaternion rotY = Quaternion::angleAxis(glm::vec3(0.0f, 0.0f, 1.0f), mouseY * MOUSE_SENSITIVITY);
		direction = rotX * rotY;
	}
	shader->setVec3("playerPosition", cam->getPosition());
}

glm::mat4 Player::getView() {
	return cam->getView();
}

Player::~Player()
{
	delete cam;
}
