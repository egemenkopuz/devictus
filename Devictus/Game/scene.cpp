#include "scene.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

Scene::Scene()
{
}

Scene::~Scene()
{
	delete this->sceneCamera;
	delete this->uiCamera;
	delete this->player;
	delete this->enemy;

	for (auto iter : sceneGraph)
		delete iter;
}

void Scene::init(LevelDifficulty levelDifficulty, const char *levelPath)
{
	this->player = new Player(glm::vec3(0.0f, 1.5f, -7.0f));
	this->enemy = new Enemy(glm::vec3(0.0f, 1.5f, 0.0f));
	
	this->sceneCamera = new Camera(glm::vec3(0.0f, 2.5f, -8.0f));

	LevelInfo levelInfo = readLevelFromFile(levelPath);
	this->levelDifficulty = levelDifficulty;

	bool destructable = true;
	if (levelDifficulty == PEASANT)
		destructable = false;

	for (auto iter : sceneGraph)
		delete iter;
	sceneGraph.clear();

	unsigned int areanaSize0 = levelInfo.level0.size();
	unsigned int center0 = floor(areanaSize0 / 2);

	unsigned int arenaSize1 = levelInfo.level1.size();
	unsigned int center1 = floor(arenaSize1 / 2);

	int blockLength = 1;

	// level0

	for (int m = 0; m < areanaSize0; m++) {
		for (int n = 0; n < areanaSize0; n++) {
			int xCoord = (m - center1) * blockLength;
			int zCoord = (n - center1) * blockLength;

			float test = (float)(m - center1);
			int height = levelInfo.level0[m][n];
			if (height != 0) {
				for (int k = 0; k < height; k++) {
					int yCoord = -(k * blockLength);
					sceneGraph.push_back(new Block(glm::vec3((float)xCoord, (float)yCoord, (float)zCoord), destructable));
				}
			}
		}
	}

	// level1

	for (int m = 0; m < areanaSize0; m++) {
		for (int n = 0; n < areanaSize0; n++) {
			int xCoord = (m - center1) * blockLength;
			int zCoord = (n - center1) * blockLength;
			int height = levelInfo.level0[m][n];
			if (height != 0) {
				for (int k = 0; k < height; k++) {
					int yCoord = (k * blockLength) + 1;
					sceneGraph.push_back(new Block(glm::vec3((float)xCoord, (float)yCoord, (float)zCoord), destructable));
				}
			}
		}
	}
}

void Scene::draw()
{
	for (auto iter : sceneGraph) {
		if (iter->avaliable) {
			iter->model->Draw(Manager::getShader(iter->type));
		}
	}

	player->model->Draw(Manager::getShader(player->type));
	enemy->model->Draw(Manager::getShader(enemy->type));
}

LevelInfo Scene::readLevelFromFile(const char * levelPath)
{
	LevelInfo info;

	try {
		std::ifstream file(levelPath);
		std::string str;
		std::string word;

		unsigned int col;
		unsigned int row;

		std::getline(file, str);	// reads the level0 size
		int levelSize0 = std::stoi(str);

		std::vector<std::vector<char>> level0(levelSize0, std::vector<char>(levelSize0));

		row = 0;
		while (levelSize0 > 0 && std::getline(file, str)) {
			std::istringstream iss(str);
			col = 0;
			while (iss >> word) {
				level0[row][col] = std::stoi(word);
				col++;
			}

			row++;
			levelSize0--;
		}

		std::getline(file, str);	// reads the level1 size
		int levelSize1 = std::stoi(str);

		std::vector<std::vector<char>> level1(levelSize1, std::vector<char>(levelSize1));

		row = 0;
		while (levelSize1 > 0 && std::getline(file, str)) {
			std::istringstream iss(str);
			col = 0;
			while (iss >> word) {
				level1[row][col] = std::stoi(word);
				col++;
			}
			row++;
			levelSize1--;
		}

		info.level0 = level0;
		info.level1 = level1;
	}
	catch (std::exception e) {
		std::cout << "ERROR, Level failed to load at path: " << levelPath << std::endl;
	}

	return info;
}
