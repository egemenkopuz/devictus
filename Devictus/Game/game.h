#include "../Utilities/definitions.h"
#include "../Engine/manager.h"

#include "scene.h"

enum GameState { GAME_MENU, GAME_ACTIVE, GAME_WIN, GAME_LOSE, GAME_EXIT };

class Game {
public:
	//unsigned int width, height;

	Game();
	~Game();

	void processMouse(float x, float y);
	void processKey(KEY key, float deltaTime);

	void init();
	void update(float deltaTime);
	void render(float deltaTime);

	bool isTerminated();
private:
	Camera * sceneCamera;
	Camera * uiCamera;
	Player * player;
	Enemy * enemy;

	GameState state;
	LevelDifficulty currentDiff;
	Scene scene;
	
	bool terminated;

	void checkCollisions();
};