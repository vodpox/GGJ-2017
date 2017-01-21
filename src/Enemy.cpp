#include "Game.h"
#include "Enemy.hpp"
#include <cmath>


int negative(int i) {
	if (i >= 0) {
		return 1;
	}
	else return -1;
}


bool ray(int x1, int y1, int x2, int y2, int iterate, int *getX, int *getY) {
	int lenghtX = abs(x2 - x1);
	int lenghtY = abs(y2 - y1);
	int lenghtTotal = lenghtX + lenghtY; 
	
	if (iterate >= lenghtTotal) return false;
	else {
		*getX = x1;
		*getY = y1;
		
		for (int i = 0; i < iterate; i++) {
			if (i % (lenghtX / lenghtY) != 0) {
				*getX = *getX + negative(x2 - x1);
			}
			else {
				*getY = *getY + negative(y2 - y1);
			}
		}
		
	}
	return true;
}


Enemy::Enemy(tplay::Game *game, int x, int y){
	this->game = game;
	setXY(x, y);
}


int Enemy::getX() {
	return x;
}


int Enemy::getY() {
	return y;
}


void Enemy::changeHealth(int change) {
	health += change;
}


int Enemy::getHealth() {
	return health;
}


void Enemy::move(int x, int y) {
	this->x += x;
	this->y += y;
}


void Enemy::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}


int Enemy::update(){
	
}


void Enemy::draw(int playerX, int playerY){
	/*game->graphics.addToWorld(x, y, "R");
	
	int xo, yo;
	for (int i = 0; ray(0, 0, playerX, playerY, i, &xo, &yo); i++) {
		game->graphics.addToWorld(xo, yo, "*");
	}
	game->graphics.addToWorld(0, 0, std::to_string(xo));*/
}
