#include "Game.h"
#include "Jammer.hpp"
#include <cmath>

Jammer::Jammer(tplay::Game *game, int x, int y){
	this->game = game;
	this->x = x;
	this->y = y;
}

bool Jammer::update(){
	health--;
	if (health > 0) return true;
	return false;
}

void Jammer::draw(){
	float distance_to_centre;
	bool found1 = false;
	bool found0 = false;
	bool inner = false;
	int lengthX = 0;
	int lengthY = 0;
	for (int aimy = 0; aimy <= 2*radius; aimy++){
		lengthY++;
		lengthX = 0;
		found1 = false;
		found0 = false;
		inner = false;
		for (int aimx = 0; aimx <= 2*radius; aimx++){
			lengthX++;
			distance_to_centre = sqrt((aimy - radius)*(aimy - radius) + (aimx - radius)*(aimx - radius));
			if (distance_to_centre > radius-0.5 && distance_to_centre < radius+0.5){
				if(aimy>0){
					found1 = true;
					found0 = false;
					if(inner){
						found0 = true;
					}
				}
				
				game->graphics.addToWorld(aimx+x-radius, aimy+y-radius, ".");
				space[aimx][aimy] = true;
			}else{
				if(aimy>0 && aimy<2*radius){
					if(found1 && !found0){
						game->graphics.addToWorld(aimx+x-radius, aimy+y-radius, ".");
						space[aimx][aimy] = true;
						inner = true;
					}else{
						found0 = true;
						space[aimx][aimy] = false;
					}
				}else{
					space[aimx][aimy] = false;
				}
			}
		}
	}
	game->graphics.addToWorld(x, y, "o");
}

bool Jammer::inRange(int x, int y){
	int temp1 = this->x-radius;
	int temp2 = this->y-radius;
	if(space[x-temp1][y-temp2]){
		return true;
	}
	return false;
}


int Jammer::getX() {
	return x;
}


int Jammer::getY() {
	return y;
}