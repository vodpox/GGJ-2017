#include "Game.h"
#include "Enemy.hpp"


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


void Enemy::draw(){
	game->graphics.addToWorld(x, y, "R");
}
