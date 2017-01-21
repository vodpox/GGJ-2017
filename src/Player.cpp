#include "Game.h"
#include "Player.hpp"
#include <cmath>

Player::Player(tplay::Game *game){
	this->game = game;
	setXY(x, y);
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
	if(ap > 0){
		ap--;
	}else{
		//playerTurn = false; <---- Sutaisyt
		ap = apMax;
	}
}

void Player::moveAim(int x, int y) {
	this->xAim += x;
	this->yAim += y;
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}
void Player::update(){
    if (game->input.isButtonDown(tplay::Keyboard::S)) {
		if(isAiming) moveAim(0, -1);
		else move(0, -1);
	}else if(game->input.isButtonDown(tplay::Keyboard::W)){
		if(isAiming) moveAim(0, 1);
		else move(0, 1);
	}else if(game->input.isButtonDown(tplay::Keyboard::A)){
		if(isAiming) moveAim(-1, 0);
		else move(-1, 0);
	}else if(game->input.isButtonDown(tplay::Keyboard::D)){
		if(isAiming) moveAim(1, 0);
		else move(1, 0);
	}
	
	if(game->input.isButtonDown(tplay::Keyboard::SPACEBAR)){
		xAim = x;
		yAim = y;
		if(isAiming){
			isAiming = false;
		}else{
			isAiming = true;
		}
	}
}

void Player::draw(){
	game->graphics.addToWorld(x, y, "@");
	if(isAiming){
		drawAim();
	}
	game->graphics.addToScreen(1, game->graphics.getTerminalSizeY()-2, "Action point: "+std::to_string(ap));
}

void Player::drawAim(){
	for (int aimy = 0; aimy <= 2*aimRadius; aimy++){
		for (int aimx = 0; aimx <= 2*aimRadius; aimx++){
			distance_to_centre = sqrt((aimy - aimRadius)*(aimy - aimRadius) + (aimx - aimRadius)*(aimx - aimRadius));
			if (distance_to_centre > aimRadius-0.5 && distance_to_centre < aimRadius+0.5){
				game->graphics.addToWorld(aimx+x-aimRadius, aimy+y-aimRadius, ".");
			}
		}
	}
	game->graphics.addToWorld(xAim, yAim, "x");
}
