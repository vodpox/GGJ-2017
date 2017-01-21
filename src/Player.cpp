#include "Game.h"
#include "Player.hpp"
#include <cmath>

Player::Player(tplay::Game *game){
	this->game = game;
	setXY(x, y);
	//loadMap(0);
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
int Player::getJammers() {
	return jammerCount;
}
int Player::getAP() {
	return ap;
}

void Player::move(int x, int y) {
	this->x += x;
	this->y += y;
	if(ap > 0){
		ap--;
	}
}

void Player::moveAim(int x, int y) {
	/*bool move = true;
	if(y!=0){
		if(xAim < aimIntervals[yAim+y][0] && xAim > aimIntervals[yAim+y][1] ){
			move = false; // <-------------
		}
	}
	if( yAim+y >= aimYInterval[0] && yAim+y <= aimYInterval[1] && move){
		if(xAim+x >= aimIntervals[yAim][0] && xAim+x <= aimIntervals[yAim][1] ){
			this->xAim += x;
			this->yAim += y;
		}
	}*/
	if(!aimSpace[xAim+x][yAim+y]){
		this->xAim += x;
		this->yAim += y;
	}
}

void Player::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}
void Player::update(bool *playerTurn){
	if(ap>0){
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
		if(game->input.isButtonDown(tplay::Keyboard::E)){
			if(isAiming){  }
			else{
				interactDoor();
			}
		}
	}
	if(game->input.isButtonDown(tplay::Keyboard::ENTER)){
		*playerTurn = false;
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

void Player::interactDoor(){
	/*if( Map[x+1][y] == '|'){ // Right
		Map[x+1][y];
	}*/
}

void Player::resetAP(){
	this->ap = apMax;
}

void Player::draw(){
	game->graphics.addToWorld(x, y, "@");
	if(isAiming){
		drawAim();
	}
	//game->graphics.addToScreen(1, game->graphics.getTerminalSizeY()-2, "Action point: "+std::to_string(ap));
}

void Player::drawAim(){
	bool firstx = true;
	for (int aimy = 0; aimy <= 2*aimRadius; aimy++){
		firstx = true;
		for (int aimx = 0; aimx <= 2*aimRadius; aimx++){
			distance_to_centre = sqrt((aimy - aimRadius)*(aimy - aimRadius) + (aimx - aimRadius)*(aimx - aimRadius));
			if (distance_to_centre > aimRadius-0.5 && distance_to_centre < aimRadius+0.5){
				/*if(aimx+x-aimRadius<0) continue;
				if(aimx+y-aimRadius<0) continue;*/
				game->graphics.addToWorld(aimx+x-aimRadius, aimy+y-aimRadius, ".");
				aimSpace[x-aimRadius+aimx][y-aimRadius+aimy] = true;
				/*if(firstx){
					aimIntervals[aimy+y-aimRadius][0] = aimx+x-aimRadius;
					firstx = false;
				}
				if(aimy==0){
					aimYInterval[0] = aimy+y-aimRadius;
				}
				aimYInterval[1] = aimy+y-aimRadius;
				aimIntervals[aimy+y-aimRadius][1] = aimx+x-aimRadius;*/
			}else{
				aimSpace[aimx][aimy] = false;
			}
		}
	}
	game->graphics.addToWorld(xAim, yAim, "x");
	/*game->graphics.addToWorld(20, 20, std::to_string(aimIntervals[yAim][0]));
	game->graphics.addToWorld(23, 20, std::to_string(aimIntervals[yAim][1]));
	game->graphics.addToWorld(26, 20, "-");
	game->graphics.addToWorld(29, 20, std::to_string(xAim));
	game->graphics.addToWorld(20, 19, std::to_string(aimYInterval[0]));
	game->graphics.addToWorld(23, 19, std::to_string(aimYInterval[1]));*/
	
	/*int num;
	for(int i=0; i<13; i++){
		for(int j=0; j<13; j++){
			if(aimSpace[j][i]){
				num = 1;
			}else{
				num = 0;
			}
			game->graphics.addToWorld(j, i, std::to_string(num));
		}
	}*/
}
