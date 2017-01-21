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
	jammer = new Jammer(game);
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
	
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			if (Map[i][j] == '@') {
				player->setXY(i, j);
			}
			else if (Map[i][j] == 'R') {
				Enemies.push_back(Enemy(game, this, i, j));
			}
			else if (Map[i][j] == '[') {
				// TODO: crates
			}
			else if (Map[i][j] == 'O') {
				int closedX, closedY, openX, openY;
				char door;
				
				if (Map[i-1][j] == '|' || Map[i-1][j] == '-') {
					door = Map[i-1][j];
					closedX = i-1;
					closedY = j;
				}
				else if (Map[i+1][j] == '|' || Map[i+1][j] == '-') {
					door = Map[i+1][j];
					closedX = i+1;
					closedY = j;
				}
				else if (Map[i][j-1] == '|' || Map[i][j-1] == '-') {
					door = Map[i][j-1];
					closedX = i;
					closedY = j-1;
				}
				else if (Map[i][j+1] == '|' || Map[i][j+1] == '-') {
					door = Map[i][j+1];
					closedX = i;
					closedY = j+1;
				}
				if (Map[i-1][j] == '.') {
					openX = i-1;
					openY = j;
				}
				else if (Map[i+1][j] == '.') {
					openX = i+1;
					openY = j;
				}
				else if (Map[i][j-1] == '.') {
					openX = i;
					openY = j-1;
				}
				else if (Map[i][j+1] == '.') {
					openX = i;
					openY = j+1;
				}
				else {
					game->quit();
				}
				
				Doors.push_back(Door(game, i, j, openX, openY, closedX, closedY, door));
			}
		}
	}
	
}


bool PlayScene::collides(int x, int y, bool doDoors) {
	// map
	if (x < 0 || x >= mapX || y < 0 || y >= mapY) return true;
	if (Map[x][y] == '#') return true;
	
	if (Map[x][y] == 'O') return true;
	
	if (doDoors) {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y) return true;
		}
	}
	
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i].getX() == x && Enemies[i].getY() == y) return true;
	}
	
	return false;
}


void PlayScene::update() {
	if (sleepTime > 1000) sleepTime = 1000;
	doSleep(sleepTime);
	sleepTime = 0;
	
	if (player->getHealth() <= 0) {
		game->quit();
	}
	
	if (animationPlaying) {
		
	}
	else if (playerTurn) {
		player->update(&playerTurn);
	}
	else { // enemy turn
		for (int i = 0; i < Enemies.size(); i++) {
			if (Enemies[i].update() != 0) return;
		}
		playerTurn = true;
		player->resetAP();
		for (int i = 0; i < Enemies.size(); i++) {
			Enemies[i].resetAP();
		}
	}
}


void PlayScene::draw() {
	int termX = game->graphics.getTerminalSizeX();
	int termY = game->graphics.getTerminalSizeY();
	
	camera.setCoordinates(player->getX() - game->graphics.getTerminalSizeX() / 2, player->getY() - game->graphics.getTerminalSizeY() / 2);
	
	jammer->draw();
	//map
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			if (Map[i][j] == '#') {
				game->graphics.addToWorld(i, j, "#");
			}
		}
	}
	for (int i = 0; i < Doors.size(); i++) {
		Doors[i].draw();
	}
	
	//playa
	player->draw();
	
	//enemmies
	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i].draw(player->getX(), player->getY());
	}
	
	// ui
	game->graphics.setFormat(tplay::Format::NEGATIVE);
	
	
	// Messages
	if (player->getAP() <= 0) {
		std::string msg = "Out of AP";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	if (!playerTurn) {
		std::string msg = "Enemy turn";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	if (player->getHealth() <= 0) {
		std::string msg = "You are dead";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
		sleepTime += 2000;
	}
	
	//stats
	game->graphics.addToScreen(1, 0, "Health: " + std::to_string(player->getHealth()));
	game->graphics.addToScreen(termX / 2 - std::string("Jammers: " + std::to_string(player->getJammers())).size() / 2 , 0, "Jammers: " + std::to_string(player->getJammers()));
	game->graphics.addToScreen(termX - 1 - std::string("AP: " + std::to_string(player->getAP())).size(), 0, "AP: " + std::to_string(player->getAP()));
	
	game->graphics.unsetFormat(tplay::Format::NEGATIVE);
}