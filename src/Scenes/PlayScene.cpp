#include "Game.h"
#include "PlayScene.hpp"
#include <cstdio>
#include <string>

PlayScene::PlayScene(tplay::Game *game, int level) {
	this->game = game;
	this->level = level;
	this->game->graphics.setCamera(&camera);
	
	loadMap(level);
}


void PlayScene::loadMap(int level) {
	FILE *filep;
	if (level == 0) {
		filep = fopen( "Maps/Tutorial.txt", "r");
	}
	else {
		filep = fopen( (std::string("Maps/Level") + std::to_string(level) + std::string(".txt")).c_str(), "r");
	}
	if (filep == NULL) game->quit();
	
	fscanf(filep, "%i %i\n", &mapX, &mapY);
	for (int i = mapY - 1; i >= 0; i--) {
		for (int j = 0; j < mapX; j++) {
			fscanf(filep, "%c", &Map[j][i]);
		}
		fscanf(filep, "\n");
	}
}


bool PlayScene::collides(int x, int y) {
	// map
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			if (Map[i][j]) return true;
		}
	}
	return false;
}


void PlayScene::update() {
	
}


void PlayScene::draw() {
	for (int i = 0; i < mapY; i++) {
		for (int j = 0; j < mapX; j++) {
			game->graphics.addToWorld(j, i, std::string(1, Map[j][i]));
		}
	}
}