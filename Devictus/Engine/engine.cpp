#include "engine.h"

Engine::Engine(unsigned int Width, unsigned int Height) : width(Width), height(Height)
{
	this->deltaTime = 0.0f;
	this->timeFramePast = 0.0f;
	firstMouseMovement = true;
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

	glfwWindowHint(GLFW_SAMPLES, 4);	// optional

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

	glEnable(GL_MULTISAMPLE);	// optional
	glEnable(GL_CULL_FACE);	// optional
	glCullFace(GL_BACK);	// sub-optional

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
#ifdef FPS_LIMIT
		if (!(deltaTime >= maxPeriod)) continue;	// limiting to max fps
#endif
		timeFramePast = currentFrame;

		keyAction();
		
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

	if (engine->firstMouseMovement == true)
	{
		engine->mouseX = xpos;
		engine->mouseY = ypos;
		engine->firstMouseMovement = false;
	}

	float xoffset = xpos - engine->mouseX;
	float yoffset = engine->mouseY - ypos; // reversed since y-coordinates go from bottom to top

	engine->mouseX = xpos;
	engine->mouseY = ypos;

	engine->game->processMouse(xoffset, yoffset, engine->deltaTime);
}

void Engine::keyAction()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, true);
	if (keys[GLFW_KEY_1])
		game->processKey(KEY_PAUSE, deltaTime);
	if (keys[GLFW_KEY_2])
		game->processKey(KEY_AABB_DEBUG, deltaTime);
	if (keys[GLFW_KEY_3])
		game->processKey(KEY_SHADOW_DEBUG, deltaTime);
	if (keys[GLFW_KEY_4])
		game->processKey(KEY_AIM_ASSIST, deltaTime);
	if (keys[GLFW_KEY_5])
		game->processKey(KEY_GOD_MODE, deltaTime);
	if (keys[GLFW_KEY_UP])
		game->processKey(KEY_ARROW_UP, deltaTime);
	if (keys[GLFW_KEY_DOWN])
		game->processKey(KEY_ARROW_DOWN, deltaTime);
	if (keys[GLFW_KEY_LEFT])
		game->processKey(KEY_ARROW_LEFT, deltaTime);
	if (keys[GLFW_KEY_RIGHT])
		game->processKey(KEY_ARROW_RIGHT, deltaTime);
	if (keys[GLFW_KEY_W])
		game->processKey(KEY_W, deltaTime);
	if (keys[GLFW_KEY_S])
		game->processKey(KEY_S, deltaTime);
	if (keys[GLFW_KEY_A])
		game->processKey(KEY_A, deltaTime);
	if (keys[GLFW_KEY_D])
		game->processKey(KEY_D, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		game->processKey(KEY_SPACE, deltaTime);
	if (keys[GLFW_KEY_LEFT_SHIFT])
		game->processKey(KEY_LEFT_SHIFT, deltaTime);
	if (keys[GLFW_KEY_ESCAPE])
		game->processKey(KEY_ESCAPE, deltaTime);
	if (keys[GLFW_KEY_ENTER])
		game->processKey(KEY_ENTER, deltaTime);
}

void Engine::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
		engine->keys[key] = true;
	else if (action == GLFW_RELEASE)
		engine->keys[key] = false;
}

void Engine::mouseKeysCallback(GLFWwindow * window, int button, int action, int mods)
{
	Engine *engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		engine->game->processKey(KEY_RIGHT, engine->deltaTime);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		engine->game->processKey(KEY_LEFT, engine->deltaTime);
}