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

bool Player::aiming(){
	return isAiming;
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
	bool inner = false;
	for (int aimy = 0; aimy <= 2*aimRadius; aimy++){
		found1 = false;
		found0 = false;
		inner = false;
		for (int aimx = 0; aimx <= 2*aimRadius; aimx++){
			if(x-aimRadius+aimx < 0) continue;
			distance_to_centre = sqrt((aimy - aimRadius)*(aimy - aimRadius) + (aimx - aimRadius)*(aimx - aimRadius));
			if (distance_to_centre > aimRadius-0.5 && distance_to_centre < aimRadius+0.5){
				if(aimy>0){
					found1 = true;
					found0 = false;
					if(inner){
						found0 = true;
					}
				}
				
				game->graphics.addToWorld(aimx+x-aimRadius, aimy+y-aimRadius, ".");
				aimSpace[x-aimRadius+aimx][y-aimRadius+aimy] = true;
			}else{
				if(aimy>0 && aimy<2*aimRadius){
					if(found1 && !found0){
						aimSpace[x-aimRadius+aimx][y-aimRadius+aimy] = true;
						inner = true;
					}else{
						found0 = true;
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
	int inc1 = -20, inc2 = 0;
	int gg = aimRadius*2+1;
	for(int i=temp2; i<gg+temp2; i++){
		inc1=-20;
		for(int j=temp1; j<gg+temp1; j++){
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
