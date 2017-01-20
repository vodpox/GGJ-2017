#include "Game.h"
#include "Player.hpp"

Player::Player(tplay::Game *game){
	this->game = game;
	setXY(5, 5);
}

int Player::getX() {
	return x;
}

int Player::getY() {
	return y;
}

void Player::changeHealth(int change) {
	health += change;
}

void Player::changeJammerCount(int change) {
	jammerCount += change;
}

int Player::getHealth() {
	return health;
}

void Player::move(int x, int y) {
	this->x += x;
	this->y += y;
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}
void Player::update(){
    if (game->input.isButtonDown(tplay::Keyboard::S)) {
		move(0, -1);
	}else if(game->input.isButtonDown(tplay::Keyboard::W)){
		move(0, 1);
	}else if(game->input.isButtonDown(tplay::Keyboard::A)){
		move(-1, 0);
	}else if(game->input.isButtonDown(tplay::Keyboard::D)){
		move(1, 0);
	}
}

void Player::draw(){
	game->graphics.addToWorld(x, y, "@");
}