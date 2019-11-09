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
	delete this->player;
	delete this->enemy;

	for (auto iter : sceneGraph)
		delete iter;
}

void Scene::init(LevelDifficulty levelDifficulty, const char *levelPath)
{
	this->blockModel = new Model("./Objects/Block/block.obj");
	this->enemyModel = new Model("./Objects/Enemy/Earth_Golem_OBJ.obj");
	this->playerModel = new Model("./Objects/Block/block.obj");

	LevelInfo levelInfo = readLevelFromFile(levelPath);
	this->levelDifficulty = levelDifficulty;

	bool destructable = true;
	if (levelDifficulty == PEASANT)
		destructable = false;

	for (auto iter : sceneGraph)
		delete iter;
	sceneGraph.clear();

	unsigned int arenaSize0 = levelInfo.level0.size();
	unsigned int center0 = floor(arenaSize0 / 2);

	unsigned int arenaSize1 = levelInfo.level1.size();
	unsigned int center1 = floor(arenaSize1 / 2);

	blockLength = levelInfo.blockLength;

	// level0
	
	for (int m = 0; m < arenaSize0; m++) {
		for (int n = 0; n < arenaSize0; n++) {
			float xCoord = ((float)m - (float)center0) * blockLength;
			float zCoord = ((float)n - (float)center0) * blockLength;
			int height = levelInfo.level0[m][n];
			if (height != 0) {
				for (int k = 0; k < height; k++) {
					float yCoord = -((float)k * blockLength);
					sceneGraph.push_back(new Block(glm::vec3(xCoord, yCoord, zCoord), blockModel, destructable));
				}
			}
		}
	}

	// level1

	for (int m = 0; m < arenaSize1; m++) {
		for (int n = 0; n < arenaSize1; n++) {
			float xCoord = ((float)m - (float)center1) * blockLength;
			float zCoord = ((float)n - (float)center1) * blockLength;
			int height = levelInfo.level1[m][n];
			if (height != 0) {
				for (int k = 0; k < height; k++) {
					float yCoord = ((float)k * blockLength) + blockLength;
					sceneGraph.push_back(new Block(glm::vec3(xCoord, yCoord, zCoord), blockModel, destructable));
				}
			}
		}
	}

	this->enemy = new Enemy(glm::vec3(0.0f, 0.5f * blockLength, 0.0f), enemyModel);
	this->player = new Player(glm::vec3(0.0f, 1.0f * blockLength, 4.0f * blockLength), playerModel);

	this->sceneCamera = new Camera(glm::vec3(0.0f, 2.0f * blockLength, 5.0f * blockLength));
}

void Scene::draw()
{
	for (auto iter : sceneGraph) {
		if (iter->available) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, iter->position);
			model = glm::scale(model, glm::vec3(blockLength));
			Shader shader = Manager::getShader(iter->type);
			shader.setMat4("model", model);
			iter->model->Draw(shader);
		}
	}
	Shader shader = Manager::getShader(player->type);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, player->position);
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	player->model->Draw(shader);

	shader = Manager::getShader(enemy->type);
	model = glm::mat4(1.0f);
	model = glm::translate(model, enemy->position);
	model = glm::scale(model, glm::vec3(0.2f));
	shader.setMat4("model", model);
	enemy->model->Draw(shader);
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

		std::getline(file, str);	// reads block length
		float blockLength = std::atof(str.c_str());

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
		info.blockLength = blockLength;
	}
	catch (std::exception e) {
		std::cout << "ERROR, Level failed to load at path: " << levelPath << std::endl;
	}

	return info;
}
