#include "Game.h"
#include "PlayScene.hpp"
#include <cstdio>
#include <string>
#include <unistd.h>
#ifdef WIN32
	#include "windows.h"
#endif


void doSleep(int milliseconds) {
	#ifdef WIN32
		Sleep(milliseconds);
	#else
		usleep(milliseconds * 1000);
	#endif
}


PlayScene::PlayScene(tplay::Game *game, int level) {
	this->game = game;
	this->level = level;
	this->game->graphics.setCamera(&camera);
	
	player = new Player(game);
	enemy = new Enemy(game, 5, 7);
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
	if (sleepTime > 1000) sleepTime = 1000;
	doSleep(sleepTime);
	sleepTime = 0;
	
	if (animationPlaying) {
		
	}
	else if (playerTurn) {
		player->update();
	}
	else { // enemy turn
		enemy->update();
		playerTurn = true;
	}
}


void PlayScene::draw() {
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			if (Map[i][j] == '#') {
				game->graphics.addToWorld(i, j, '#');
			}
		}
	}
	player->draw();
	enemy->draw(player->getX(), player->getY());
}