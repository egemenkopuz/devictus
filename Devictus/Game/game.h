#include "../Utilities/definitions.h"
#include "../Engine/manager.h"
#include "../Engine/text.h"
#include "../Engine/particle.h"
#include "../Engine/shadow.h"
#include "../Engine/skybox.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"

enum GameState { GAME_MENU, GAME_ACTIVE, GAME_WIN, GAME_LOSE, GAME_EXIT };

class Game {
public:
	float gameWidth, gameHeight;
	float aspecRatio;

	Game();
	~Game();

	void processMouse(float x, float y,float deltaTime);
	void processKey(KEY key, float deltaTime);

	void init();
	void update(float deltaTime);
	void render(float deltaTime);

	bool isTerminated();

	bool aabbDebug = false;
	bool shadowDebug = false;
private:
	Skybox skybox;

	glm::vec3 lightColor;
	glm::vec3 lightPosition = glm::vec3(0.f, 5.f, 0.f);

	Camera * freeCamera;
	Camera3rdPerson * playerCamera;
	Player * player;
	Enemy * enemy;

	GameState state;
	LevelDifficulty currentDiff;
	Scene scene;

	ShadowMap shadowMap;

	TextRenderer * textRenderer;
	bool movementKeys[NUM_MOVE_KEYS];

	float menuSelectionBarrierVal;
	bool menuSelectionBarrier;

	bool terminated;
	bool paused = false;

	float FPS;

	void checkCollisions(float deltaTime);
};