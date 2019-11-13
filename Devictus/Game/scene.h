#ifndef SCENE_H
#define SCENE_H

#include "../Engine/manager.h"

#include "camera.h"
#include "object.h"

#include <vector>
#include <string>

enum LevelDifficulty { PEASANT, CHALLENGER, GOD };

struct LevelInfo {
	std::vector<std::vector<char>> level1;
	std::vector<std::vector<char>> level0;
	float blockLength;
};

class Scene {
public:
	Scene();
	~Scene();

	void init(LevelDifficulty levelDifficulty, const char *levelPath);
	void draw();

	Camera * freeCamera;
	Camera3rdPerson * playerCamera;
	Player * player;
	Enemy * enemy;

private:
	Model * blockModel;
	Model * playerModel;
	Model * enemyModel;

	float blockLength;
	LevelDifficulty levelDifficulty;

	std::vector<GameObject*> sceneGraph;

	static LevelInfo readLevelFromFile(const char *levelPath);
};

#endif