#include "../Utilities/definitions.h"
#include "../Engine/manager.h"

#include "scene.h"

enum GameState { GAME_MENU, GAME_ACTIVE, GAME_WIN, GAME_LOSE, GAME_EXIT };

class Game {
public:
	GameState state;
	std::vector<Scene> levels;

	//unsigned int width, height;

	Game();
	~Game();

	void processMouse(float x, float y);
	void processKey(KEY key, float deltaTime);
	void processKey(KEY key, unsigned int level);

	void init();
	void update(float deltaTime);
	void render(float deltaTime);
	void checkCollisions();

	bool isTerminated();
private:
	Camera * camera;
	Player * player;
	Enemy * enemy;


	bool terminated;
};