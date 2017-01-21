#include "Game.h"
#include "Jammer.hpp"
#include <cmath>

Jammer::Jammer(tplay::Game *game){
	this->game = game;
}

void Jammer::update(){
	
}

void Jammer::draw(){
	float distance_to_centre;
	/*bool found1 = false;
	bool found0 = false;*/
	for (int aimy = 0; aimy <= 2*radius; aimy++){
		for (int aimx = 0; aimx <= 2*radius; aimx++){
			distance_to_centre = sqrt((aimy - radius)*(aimy - radius) + (aimx - radius)*(aimx - radius));
			if (distance_to_centre > radius-0.5 && distance_to_centre < radius+0.5){
				/*if(aimy>0){
					found1 = true;
				}*/
				game->graphics.addToWorld(aimx+x-radius, aimy+y-radius, "*");
				space[x-radius+aimx][y-radius+aimy] = true;
			}else{
				space[x-radius+aimx][y-radius+aimy] = false;
			}
		}
	}
	game->graphics.addToWorld(x, y, "o");
}

bool Jammer::inRange(int x, int y){
	
}