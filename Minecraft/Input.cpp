#include "Input.h"

bool* Input::keysPressed = (bool*)malloc(sizeof(bool) * (GLFW_KEY_LAST + 1));
bool* Input::keysPressedPrev = (bool*)malloc(sizeof(bool) * (GLFW_KEY_LAST + 1));
bool* Input::keysClicked = (bool*)malloc(sizeof(bool) * (GLFW_KEY_LAST + 1));
double Input::mouseX = 0;
double Input::mouseY = 0;
double Input::mouseXPrev = 0;
double Input::mouseYPrev = 0;
double Input::mouseXDelta = 0;
double Input::mouseYDelta = 0;

Input::Input()
{
}

void Input::initialize() {
	for (int i = 0; i < GLFW_KEY_LAST + 1; i++) {
		keysPressed[i] = false;
		keysPressedPrev[i] = false;
		keysClicked[i] = false;
	}
}

void Input::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		keysPressed[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keysPressed[key] = false;
	}
}

void Input::mouseEvent(GLFWwindow* window, double posx, double posy) {
	mouseX = posx;
	mouseY = posy;
}

void Input::update() {
	for (int i = 0; i < GLFW_KEY_LAST + 1; i++) {
		if (keysPressed[i] != keysPressedPrev[i] && keysPressed[i]) {
			keysClicked[i] = true;
		}
		else {
			keysClicked[i] = false;
		}
		keysPressedPrev[i] = keysPressed[i];
	}
	mouseXDelta = mouseX - mouseXPrev;
	mouseYDelta = mouseY - mouseYPrev;
	mouseXPrev = mouseX;
	mouseYPrev = mouseY;
}

bool Input::keyDown(int keyCode) {
	return keysPressed[keyCode];
}

bool Input::keyClicked(int keyCode) {
	return keysClicked[keyCode];
}

void Input::deInitialize() {
	free(keysPressed);
}

Input::~Input()
{
	free(keysPressed);
	free(keysPressedPrev);
	free(keysClicked);
}
