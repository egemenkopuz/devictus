#include "../Utilities/definitions.h"
#include "../Engine/manager.h"
#include "../Engine/text.h"

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

	void processMouse(float x, float y);
	void processKey(KEY key, float deltaTime);

	void init();
	void update(float deltaTime);
	void render(float deltaTime);

	bool isTerminated();
private:
	Camera * freeCamera;
	Camera3rdPerson * playerCamera;
	Player * player;
	Enemy * enemy;

	GameState state;
	LevelDifficulty currentDiff;
	Scene scene;

	TextRenderer * textRenderer;
	bool movementKeys[NUM_MOVE_KEYS];

	float menuSelectionBarrierVal;
	bool menuSelectionBarrier;
	bool terminated;
	bool paused = false;

	float FPS;

	void checkCollisions();
};