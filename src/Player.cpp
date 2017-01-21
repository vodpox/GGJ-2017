#include "Game.h"
#include "Player.hpp"
#include <cmath>
#include "Scenes/PlayScene.hpp"


Player::Player(tplay::Game *game, PlayScene *playScene){
	this->game = game;
	this->playScene = playScene;
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
	if(ap > 0 && !playScene->collides(this->x + x, this->y + y, true)){
		ap--;
		this->x += x;
		this->y += y;
	}
}

void Player::moveAim(int x, int y) {
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
			if(isAiming) shoot();
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

void Player::shoot(){
	
}

void Player::resetAP(){
	this->ap = apMax;
}

void Player::draw(){
	game->graphics.addToWorld(x, y, "@");
	if(isAiming){
		drawAim();
	}
}

void Player::drawAim(){
	bool found1 = false;
	bool found0 = false;
	for (int aimy = 0; aimy <= 2*aimRadius; aimy++){
		for (int aimx = 0; aimx <= 2*aimRadius; aimx++){
			distance_to_centre = sqrt((aimy - aimRadius)*(aimy - aimRadius) + (aimx - aimRadius)*(aimx - aimRadius));
			if (distance_to_centre > aimRadius-0.5 && distance_to_centre < aimRadius+0.5){
				if(!found1 && found0){
					found0 = false;
				}
				if(aimy>0){
					found1 = true;
				}
				
				game->graphics.addToWorld(aimx+x-aimRadius, aimy+y-aimRadius, ".");
				aimSpace[x-aimRadius+aimx][y-aimRadius+aimy] = true;
			}else{
				if(aimy>0){
					found0 = true;
					if(found1 && found0){
						aimSpace[x-aimRadius+aimx][y-aimRadius+aimy] = true;
					}else{
						aimSpace[x-aimRadius+aimx][y-aimRadius+aimy] = false;
					}
				}else{
					aimSpace[x-aimRadius+aimx][y-aimRadius+aimy] = false;
				}
			}
		}
	}
	game->graphics.addToWorld(xAim, yAim, "x");
	
	int num;
	int temp1 = x-aimRadius;
	int temp2 = y-aimRadius;
	int inc1 = -15, inc2 = 0;
	for(int i=temp2; i<13+temp2; i++){
		inc1=-15;
		for(int j=temp1; j<13+temp1; j++){
			if(aimSpace[j][i]){
				num = 1;
			}else{
				num = 0;
			}
			game->graphics.addToWorld(inc1, inc2, std::to_string(num));
			inc1++;
		}
		inc2++;
	}
}
