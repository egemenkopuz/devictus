#include "scene.h"

#include <iostream>
#include <sstream>
#include <fstream>

Scene::Scene(LevelDifficulty levelDifficulty, const char *levelPath)
{
	LevelInfo levelInfo = readLevelFromFile(levelPath);


}

Scene::~Scene()
{
	delete this->camera;
	delete this->player;
	delete this->enemy;
}

void Scene::init()
{

}

void Scene::draw()
{
}

LevelInfo Scene::readLevelFromFile(const char * levelPath)
{
	LevelInfo levelInfo;

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

		std::vector<std::vector<char>> level1(levelSize0, std::vector<char>(levelSize0));

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

		levelInfo.level0 = level0;
		levelInfo.level1 = level1;
	}
	catch (std::exception e) {
		std::cout << "ERROR, Level failed to load at path: " << levelPath << std::endl;
	}

	return levelInfo;
}
