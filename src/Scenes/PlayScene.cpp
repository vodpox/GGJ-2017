#include "Game.h"
#include "PlayScene.hpp"
#include <cstdio>
#include <string>
#include <unistd.h>
#include <cmath>
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
	
	player = new Player(game, this);
	loadMap(level);
}


void PlayScene::loadMap(int level) {
	FILE *filep;
	if (level < 0) {
		filep = fopen( (std::string("Maps/Tutorial") + std::to_string(level * -1) + std::string(".txt")).c_str(), "r");
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
			else if (Map[i][j] == '=') {
				Crate *crate = new Crate();
				crate->x = i;
				crate->y = j;
				Crates.push_back(crate);
			}
			else if (Map[i][j] == '!') {
				endX = i;
				endY = j;
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
	
	if (Map[x][y] == '=') return true;
	
	if (doDoors) {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y) return true;
		}
	}
	else {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y && Doors[i].isOpen) return true;
		}
	}
	
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i].getX() == x && Enemies[i].getY() == y) return true;
	}
	
	return false;
}


bool PlayScene::collides(int x, int y, bool doDoors, bool doJamming, int igor) {
	// map
	if (x < 0 || x >= mapX || y < 0 || y >= mapY) return true;
	if (Map[x][y] == '#') return true;
	
	if (Map[x][y] == 'O') return true;
	
	if (Map[x][y] == '=') return true;
	
	if (doDoors) {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y) return true;
		}
	}
	else {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y && Doors[i].isOpen) return true;
		}
	}
	
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i].getX() == x && Enemies[i].getY() == y && i != igor) return true;
	}
	
	if (doJamming) {
		for (int i = 0; i < Jammers.size(); i++) {
			if (Jammers[i].inRange(x, y)) return true;
		}
	}
	
	return false;
}


int PlayScene::nearDoor(int x, int y) {
	for (size_t i = 0; i < Doors.size(); i++) {
		if (Doors[i].getClosedX() == x - 1 && Doors[i].getClosedY() == y) return i;
		if (Doors[i].getClosedX() == x + 1 && Doors[i].getClosedY() == y) return i;
		if (Doors[i].getClosedX() == x && Doors[i].getClosedY() == y - 1) return i;
		if (Doors[i].getClosedX() == x && Doors[i].getClosedY() == y + 1) return i;
	}
	return -1;
}


int PlayScene::nearCrate(int x, int y) {
	for (size_t i = 0; i < Crates.size(); i++) {
		if (Crates[i]->x == x - 1 && Crates[i]->y == y) return i;
		if (Crates[i]->x == x + 1 && Crates[i]->y == y) return i;
		if (Crates[i]->x == x && Crates[i]->y == y - 1) return i;
		if (Crates[i]->x == x && Crates[i]->y == y + 1) return i;
	}
	return -1;
}


void PlayScene::update() {
	if (sleepTime > 1000) sleepTime = 1000;
	doSleep(sleepTime);
	sleepTime = 0;
	
	if (player->getHealth() <= 0) {
		game->quit();
	}
	else if (player->getX() == endX && player->getY() == endY) {
		if (level > 0) {
			if (level < 1) {
				PlayScene *playScene = new PlayScene(game, level + 1);
				game->setScene(playScene);
			}
			else {
				game->quit();
			}
		}
		else {
			if (level > -3) {
				PlayScene *playScene = new PlayScene(game, level - 1);
				game->setScene(playScene);
			}
			else {
				PlayScene *playScene = new PlayScene(game, 1);
				game->setScene(playScene);
			}
		}
	}
	
	if (animationPlaying) {
		
	}
	else if (playerTurn) {
		player->update(&playerTurn);
	}
	else { // enemy turn
		for (int i = 0; i < Enemies.size(); i++) {
			if (Enemies[i].update(i) != 0) return;
		}
		//Jammers get --health
		for (int i = 0; i < Jammers.size(); i++) {
			if (!Jammers[i].update()) Jammers.erase(Jammers.begin() + i);
		}
		playerTurn = true;
		player->resetAP();
		for (int i = 0; i < Enemies.size(); i++) {
			Enemies[i].resetAP();
		}
	}
}

void PlayScene::ray(int x1, int y1, int x2, int y2){
	rayX = x1;
	rayY = y1;
	int dx = x1 - x2;
	int dy = y1 - y2;
	float steps;
	
	if(abs(dx) > abs(dy)) steps = abs(dx);
	else steps = abs(dy);
	
	float xInc = dx / (float)steps;
	float yInc = dy / (float)steps;
	
	for(int i=0; i<steps; i++){
		rayX += xInc;
		rayY += yInc;
		
		game->graphics.addToWorld(-1*round(rayX), -1*round(rayY), ".");
	}
}	

void PlayScene::draw() {
	int termX = game->graphics.getTerminalSizeX();
	int termY = game->graphics.getTerminalSizeY();
	
	camera.setCoordinates(player->getX() - game->graphics.getTerminalSizeX() / 2, player->getY() - game->graphics.getTerminalSizeY() / 2);
	
	//Jammers
	for (int i = 0; i < Jammers.size(); i++) {
		Jammers[i].draw();
	}
	
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
	
	//bullet line
	//ray(0, 0, player->getX(), player->getY());
	
	//playa
	player->draw();
	
	// goal
	game->graphics.addToWorld(endX, endY, "!");
	
	//enemmies
	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i].draw(player->getX(), player->getY());
	}
	
	//crates
	for (int i = 0; i < Crates.size(); i++) {
		game->graphics.addToWorld(Crates[i]->x, Crates[i]->y, "=");
	}
	
	// ui
	game->graphics.setFormat(tplay::Format::NEGATIVE);
	
	
	// Important messages
	if (player->getHealth() <= 0) {
		std::string msg0 = "+--------------+";
		std::string msg1 = "| You are dead |";
		std::string msg2 = "+--------------+";
		
		game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 1, msg0);
		game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2, msg1);
		game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 - 1, msg2);
		sleepTime += 2000;
	}
	else if (player->getX() == endX && player->getY() == endY) {
		std::string msg0 = "+-----------------+";
		std::string msg1 = "| Level completed |";
		std::string msg2 = "+-----------------+";
		
		game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 1, msg0);
		game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2, msg1);
		game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 - 1, msg2);
		sleepTime += 2000;
	}
	
	// Meh messages
	if (!playerTurn) {
		std::string msg = "Enemy turn";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (player->getAP() <= 0) {
		std::string msg = "Out of AP";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (player->aiming() && player->getJammers() > 0) {
		std::string msg = "Press E to shoot";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (player->aiming()) {
		std::string msg = "No jammers left";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (nearDoor(player->getX(), player->getY()) != -1) {
		std::string msg = "Press E to open/close door";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (nearCrate(player->getX(), player->getY()) != -1) {
		std::string msg = "Press E to open crate";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	
	
	//stats
	game->graphics.addToScreen(1, 0, + "Jammers: " + std::to_string(player->getJammers()));
	//game->graphics.addToScreen(termX / 2 - std::string("Jammers: " + std::to_string(player->getJammers())).size() / 2 , 0, "Jammers: " + std::to_string(player->getJammers()));
	game->graphics.addToScreen(termX - 1 - std::string("AP: " + std::to_string(player->getAP())).size(), 0, "AP: " + std::to_string(player->getAP()));
	
	game->graphics.unsetFormat(tplay::Format::NEGATIVE);
}