#ifndef SCENE_H
#define SCENE_H

#include "../Utilities/definitions.h"
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

	void drawShadowMap(Shader shader);
	void drawScene(Shader &shader, bool aabbDebug = false);
	void drawGUI(Shader &shader);
	void drawQuad();
	void drawCube();

	Camera * freeCamera;
	Camera3rdPerson * playerCamera;
	Player * player;
	Enemy * enemy;

	std::vector<GameObject*> sceneGraph;

	Model * getPModel() { return projectileModel; }
	Model * blockModel;
	Model * playerModel;
	Model * enemyModel;
private:
	Model * projectileModel;

	float blockLength;
	LevelDifficulty levelDifficulty;

	static LevelInfo readLevelFromFile(const char *levelPath);


	// exp
	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
};

#endif