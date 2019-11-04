#include "engine.h"

Engine::Engine(unsigned int Width, unsigned int Height) : width(Width), height(Height)
{
	this->deltaTime = 0.0f;
	this->timeFramePast = 0.0f;
}

Engine::~Engine()
{
	glfwTerminate();
}

bool Engine::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(this->width, this->height, "DEVICTUS", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Engine::framebufferSizeCallback);
	glfwSetCursorPosCallback(window, Engine::mouseMoveCallback);
	glfwSetMouseButtonCallback(window, Engine::mouseKeysCallback);
	
	//glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/


	glfwSetWindowUserPointer(window, this);

	return true;
}

void Engine::start(Game * game)
{
	this->game = game;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - timeFramePast;
		timeFramePast = currentFrame;

		Engine::keysCallback(this->window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		this->game->update(deltaTime);
		this->game->render(deltaTime);

		glfwSwapBuffers(this->window);
		glfwPollEvents();

		if (this->game->isTerminated())
			glfwSetWindowShouldClose(this->window, true);
	}
}

void Engine::framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Engine::mouseMoveCallback(GLFWwindow * window, double xpos, double ypos)
{
	Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

	if (engine->deltaTime == 0.0f)
	{
		engine->mouseX = xpos;
		engine->mouseY = ypos;
	}

	float xoffset = xpos - engine->mouseX;
	float yoffset = engine->mouseY - ypos; // reversed since y-coordinates go from bottom to top

	engine->mouseX = xpos;
	engine->mouseY = ypos;

	engine->game->processMouse(xoffset, yoffset);
}

void Engine::keysCallback(GLFWwindow * window)
{
	Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		engine->game->processKey(KEY_ESCAPE, engine->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		engine->game->processKey(KEY_ENTER, engine->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		engine->game->processKey(KEY_W, engine->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		engine->game->processKey(KEY_S, engine->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		engine->game->processKey(KEY_A, engine->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		engine->game->processKey(KEY_D, engine->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		engine->game->processKey(KEY_SPACE, engine->deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		engine->game->processKey(KEY_LEFT_SHIFT, engine->deltaTime);
}

void Engine::mouseKeysCallback(GLFWwindow * window, int button, int action, int mods)
{
	Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		engine->game->processKey(KEY_RIGHT, engine->deltaTime);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		engine->game->processKey(KEY_LEFT, engine->deltaTime);
}