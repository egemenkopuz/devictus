#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "../Game/game.h"
#include "../Utilities/definitions.h"

const float maxFPS = 72.0f;
const float maxPeriod = 1.0f / maxFPS;

class Engine {
public:
	Engine(unsigned int Width, unsigned int Height);
	~Engine();

	bool init();
	void start(Game * game);
private:
	GLFWwindow * window;
	Game * game;

	bool keys[1024];

	float deltaTime;
	float timeFramePast;
	unsigned int width, height;
	float mouseX, mouseY;

	bool firstMouseMovement;

	void keyAction();

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mouseKeysCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif