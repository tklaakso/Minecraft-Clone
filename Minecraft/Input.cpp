#include "Input.h"

bool* Input::keysPressed = (bool*)malloc(sizeof(bool) * (GLFW_KEY_LAST + 1));
bool* Input::keysPressedPrev = (bool*)malloc(sizeof(bool) * (GLFW_KEY_LAST + 1));
bool* Input::keysClicked = (bool*)malloc(sizeof(bool) * (GLFW_KEY_LAST + 1));
bool* Input::mouseButtonsPressed = (bool*)malloc(sizeof(bool) * 3);
bool* Input::mouseButtonsPressedPrev = (bool*)malloc(sizeof(bool) * 3);
bool* Input::mouseButtonsClicked = (bool*)malloc(sizeof(bool) * 3);
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
	for (int i = 0; i < 3; i++) {
		mouseButtonsPressed[i] = false;
		mouseButtonsPressedPrev[i] = false;
		mouseButtonsClicked[i] = false;
	}
}

void Input::keyEvent(GLFWwindow* window, int key, int scancode, int action, int mode) {
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

void Input::mouseButtonEvent(GLFWwindow* window, int button, int action, int mode) {
	if (button < 3) {
		if (action == GLFW_PRESS) {
			mouseButtonsPressed[button] = true;
		}
		else if (action == GLFW_RELEASE) {
			mouseButtonsPressed[button] = false;
		}
	}
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
	for (int i = 0; i < 3; i++) {
		if (mouseButtonsPressed[i] != mouseButtonsPressedPrev[i] && mouseButtonsPressed[i]) {
			mouseButtonsClicked[i] = true;
		}
		else {
			mouseButtonsClicked[i] = false;
		}
		mouseButtonsPressedPrev[i] = mouseButtonsPressed[i];
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

bool Input::mouseDown(int mouseButton) {
	return mouseButtonsPressed[mouseButton];
}

bool Input::mouseClicked(int mouseButton) {
	return mouseButtonsClicked[mouseButton];
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
