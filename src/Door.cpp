#include "Game.h"
#include "Door.hpp"


Door::Door(tplay::Game *game, int x, int y){
	this->game = game;
	this->x = x;
	this->y = y;
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


int Door::update(){
	
}


void Door::draw(int playerX, int playerY){
	game->graphics.addToWorld(x, y, "O");
	
}
