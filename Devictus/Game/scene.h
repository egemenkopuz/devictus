#ifndef SCENE_H
#define SCENE_H

#include "../Engine/manager.h"

#include "camera.h"

#include "enemy.h"
#include "player.h"
#include "projectile.h"

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
	void draw(bool aabbDebug);

	Camera * freeCamera;
	Camera3rdPerson * playerCamera;
	Player * player;
	Enemy * enemy;

	std::vector<GameObject*> sceneGraph;

	Model * getPModel() { return projectileModel; }
private:
	Model * blockModel;
	Model * playerModel;
	Model * enemyModel;
	Model * projectileModel;

	float blockLength;
	LevelDifficulty levelDifficulty;

	static LevelInfo readLevelFromFile(const char *levelPath);
};

#endif