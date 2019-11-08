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
	glfwSetKeyCallback(window, Engine::keyCallback);
	
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

void Engine::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
#ifdef DEBUG
	std::cout << "KEY_CALL_BACK:" << key << std::endl;
#endif 
	Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (true ||action != GLFW_RELEASE) {       // avoid double updates
		switch (key) {
		case GLFW_KEY_1: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		case GLFW_KEY_2: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;

		case GLFW_KEY_W: engine->game->processKey(KEY_W, engine->deltaTime); break;
		case GLFW_KEY_S: engine->game->processKey(KEY_S, engine->deltaTime); break;

		case GLFW_KEY_A: engine->game->processKey(KEY_A, engine->deltaTime); break;
		case GLFW_KEY_D: engine->game->processKey(KEY_D, engine->deltaTime); break;

		case GLFW_KEY_SPACE: engine->game->processKey(KEY_SPACE, engine->deltaTime); break;
		case GLFW_KEY_LEFT_SHIFT: engine->game->processKey(KEY_LEFT_SHIFT, engine->deltaTime); break;

		case GLFW_KEY_ESCAPE: engine->game->processKey(KEY_ESCAPE, engine->deltaTime); break;
		case GLFW_KEY_ENTER: engine->game->processKey(KEY_ENTER, engine->deltaTime); break;
		}
	}
}

void Engine::mouseKeysCallback(GLFWwindow * window, int button, int action, int mods)
{
	Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		engine->game->processKey(KEY_RIGHT, engine->deltaTime);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		engine->game->processKey(KEY_LEFT, engine->deltaTime);
}