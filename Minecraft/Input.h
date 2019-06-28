#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdlib>

class Input
{
public:
	Input();
	static void initialize();
	static void deInitialize();
	static void update();
	static bool keyDown(int keyCode);
	static bool keyClicked(int keyCode);
	static bool mouseDown(int mouseButton);
	static bool mouseClicked(int mouseButton);
	static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouseEvent(GLFWwindow* window, double posx, double posy);
	static void mouseButtonEvent(GLFWwindow* window, int button, int action, int mode);
	static double mouseXDelta;
	static double mouseYDelta;
	static double mouseX;
	static double mouseY;
	~Input();
private:
	static bool* keysPressed;
	static bool* keysPressedPrev;
	static bool* keysClicked;
	static bool* mouseButtonsPressed;
	static bool* mouseButtonsPressedPrev;
	static bool* mouseButtonsClicked;
	static double mouseXPrev;
	static double mouseYPrev;
};

