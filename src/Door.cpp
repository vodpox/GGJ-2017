#include "Game.h"
#include "Door.hpp"
#include <string>
#include <cstdio>


Door::Door(tplay::Game *game, int x, int y, int openX, int openY, int closedX, int closedY, char closedDoor){
	this->game = game;
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


int Door::getX() {
	return x;
}


int Door::getY() {
	return y;
}


bool Door::toggle() {
	
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