#include <glad/glad.h>
#include "Player.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "stb_image.h"
#include "Block.h"
#include "Math.h"
#include "World.h"
#include "Input.h"
#include <GLFW/glfw3.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initGLFW(int width, int height, const char* title);
int initGLAD();

int main() {
	int screenWidth = 800;
	int screenHeight = 600;
	GLFWwindow* window = initGLFW(screenWidth, screenHeight, "Minecraft");
	glfwSetKeyCallback(window, Input::keyEvent);
	glfwSetCursorPosCallback(window, Input::mouseEvent);
	if (window == NULL) {
		return -1;
	}
	if (initGLAD() == -1) {
		return -1;
	}

	ViewFrustum* frustum = new ViewFrustum();

	Player* player = new Player(window);

	Shader* shader = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	shader->use();
	shader->setFloat("fogStart", 10.0f);
	shader->setFloat("fogEnd", 100.0f);

	Block::initialize(shader);
	Input::initialize();

	World* world = new World();
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			world->makeChunk(x, y);
		}
	}

	/*double start = glfwGetTime();
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 80; y++) {
			for (int z = 0; z < 64; z++) {
				Block* grass = new Block(x, y, z);
				grass->type = (x * 16 * 16 + y * 16 + z) % NUM_TEXTURES;
				world->setBlock(x, y, z, grass, false, false);
			}
		}
	}
	double addBlocks = glfwGetTime();

	world->reorderBlocks();

	double reorder = glfwGetTime();

	world->updateChunkVAOs();

	double update = glfwGetTime();

	cout << "Add blocks: " << (addBlocks - start) << endl;
	cout << "Reorder blocks: " << (reorder - addBlocks) << endl;
	cout << "Update VAOs: " << (update - reorder) << endl;
	cout << "Time: " << Chunk::time << endl;*/

	//world->deleteChunk(0, 0);

	glViewport(0, 0, 800, 600);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)screenWidth / screenHeight, 0.1f, 1000.0f);

	double startTime = glfwGetTime();

	double prevTime = startTime;

	const float speed = 10.0f;

	// Render loop
	while (!glfwWindowShouldClose(window)) {

		Input::update();

		double t = glfwGetTime();

		double deltaTime = t - prevTime;

		player->update(shader, deltaTime);

		prevTime = t;

		view = player->getView();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		shader->use();

		frustum->update(projection * view);
		world->updateViewFrustum(frustum);
		
		world->render();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	
	delete world;

	Block::deInitialize();
	Input::deInitialize();

	glfwTerminate();

	delete shader;

	delete frustum;
	
	delete player;

	return 0;
}

GLFWwindow* initGLFW(int width, int height, const char* title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

int initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}