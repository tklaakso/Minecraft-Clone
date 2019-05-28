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
	static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseEvent(GLFWwindow* window, double posx, double posy);
	static double mouseXDelta;
	static double mouseYDelta;
	static double mouseX;
	static double mouseY;
	~Input();
private:
	static bool* keysPressed;
	static bool* keysPressedPrev;
	static bool* keysClicked;
	static double mouseXPrev;
	static double mouseYPrev;
};

