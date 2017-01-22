#include "Game.h"
#include "Player.hpp"
#include <cmath>
#include "Scenes/PlayScene.hpp"


Player::Player(tplay::Game *game, PlayScene *playScene){
	this->game = game;
	this->playScene = playScene;
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
	int temp1 = this->x-aimRadius;
	int temp2 = this->y-aimRadius;
	if(aimSpace[xAim+x-temp1][yAim+y-temp2]){
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
			if (isAiming) shoot();
			else if ( playScene->nearDoor(x, y) != -1 && ap > 0 ) {
				if ( playScene->Doors[playScene->nearDoor(x, y)].toggle() ) {
					ap--;
				}
			}
			else if ( playScene->nearCrate(x, y) != -1 && ap > 0 ) {
				playScene->Crates.erase(playScene->Crates.begin() + playScene->nearCrate(x, y));
				jammerCount += 3;
				ap--;
			}
		}
	}
	if(game->input.isButtonDown(tplay::Keyboard::ENTER)){
		if(!isAiming) *playerTurn = false;
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
	if (ap > 0 && jammerCount > 0) {
		playScene->animationPlaying = 1;
		ap--;
		jammerCount--;
		isAiming = false;
	}
}


int Player::getAimX() {
	return xAim;
}


int Player::getAimY() {
	return yAim;
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
	/*game->graphics.addToWorld(-20, 30, std::to_string(text1));
	game->graphics.addToWorld(-18, 30, std::to_string(text2));*/
}

void Player::drawAim(){
	bool found1 = false;
	bool found0 = false;
	bool inner = false;
	int lengthX = 0;
	int lengthY = 0;
	for (int aimy = 0; aimy <= 2*aimRadius; aimy++){
		lengthY++;
		lengthX = 0;
		found1 = false;
		found0 = false;
		inner = false;
		for (int aimx = 0; aimx <= 2*aimRadius; aimx++){
			lengthX++;
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
				aimSpace[aimx][aimy] = true;
			}else{
				if(aimy>0 && aimy<2*aimRadius){
					if(found1 && !found0){
						aimSpace[aimx][aimy] = true;
						inner = true;
					}else{
						found0 = true;
						aimSpace[aimx][aimy] = false;
					}
				}else{
					aimSpace[aimx][aimy] = false;
				}
			}
		}
	}
	game->graphics.addToWorld(xAim, yAim, "x");
	
	int temp;
	for (int i = 1; playScene->ray(x, y, xAim, yAim, i, &temp, &temp, '*') && i < 100; i++);
	
	/*int num;
	int temp1 = x-aimRadius;
	int temp2 = y-aimRadius;
	int inc1 = -20, inc2 = 0;
	int gg = aimRadius*2+1;
	for(int i=0; i<lengthY; i++){
		inc1=-20;
		for(int j=0; j<lengthX; j++){
			if(aimSpace[j][i]){
				num = 1;
			}else{
				num = 0;
			}
			game->graphics.addToWorld(inc1, inc2, std::to_string(num));
			inc1++;
		}
		inc2++;
	}*/
}
