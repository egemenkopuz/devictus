#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "object.h"

#include <vector>
#include <string>

enum LevelDifficulty { PEASANT, CHALLENGER, GOD, NOT_CHOSEN };

struct LevelInfo {
	std::vector<std::vector<char>> level1;
	std::vector<std::vector<char>> level0;
};

class Scene {
public:
	unsigned int arenaSize;

	Scene(LevelDifficulty levelDifficulty, const char *levelPath);
	~Scene();

	void init();
	void draw();

private:
	LevelDifficulty levelDifficulty;

	Camera * camera;
	Player * player;
	Enemy * enemy;

	std::vector<GameObject> sceneGraph;

	static LevelInfo readLevelFromFile(const char *levelPath);
};

#endif