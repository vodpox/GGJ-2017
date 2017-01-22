#include "Game.h"
#include "Door.hpp"
#include <string>
#include <cstdio>


Door::Door(tplay::Game *game, PlayScene *playScene, int x, int y, int openX, int openY, int closedX, int closedY, char closedDoor){
	this->game = game;
	this->playScene = playScene;
	this->x = x;
	this->y = y;
	this->openX = openX;
	this->openY = openY;
	this->closedX = closedX;
	this->closedY = closedY;
	this->closedDoor = closedDoor;
	if (closedDoor == '-') openDoor = '|';
	else openDoor = '-';
}


int Door::getBaseX() {
	return x;
}


int Door::getBaseY() {
	return y;
}


int Door::getDoorX() {
	if (isOpen) return openX;
	else return closedX;
}


int Door::getDoorY() {
	if (isOpen) return openY;
	else return closedY;
}


int Door::getClosedX() {
	return closedX;
}


int Door::getClosedY() {
	return closedY;
}


bool Door::toggle() {
	bool doToggle = true;
	for (int i = 0; i < playScene->Jammers.size(); i++) {
		if (playScene->Jammers[i].inRange(x, y)) doToggle = false;
	}
	if (doToggle) {
		isOpen = !isOpen;
		return true;
	}
	else return false;
}


bool Door::getJam() {
	for (int i = 0; i < playScene->Jammers.size(); i++) {
		if (playScene->Jammers[i].inRange(x, y)) return true;
	}
	return false;
}


bool Door::setOpen(bool isOpen) {
	
}


void Door::draw() {
	game->graphics.addToWorld(x, y, "O");
	if (isOpen) {
		game->graphics.addToWorld(openX, openY, std::string(1, openDoor));
	}
	else {
		game->graphics.addToWorld(closedX, closedY, std::string(1, closedDoor));
		//printf("%i %i %i %i\n", closedX, closedY, openX, openY);
	}
}
