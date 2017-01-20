#ifndef ENEMY
#define ENEMY

#include "Game.h"

class Enemy {
	private:
		int x = 5;
		int y = 5;
		int health = 100;
		int maxHealth = 100;
		int jammerCount = 0;
		int maxJammerCount = 3;
		tplay::Game *game;
	
	public:
		Enemy(tplay::Game *game, int x, int y);
		int getX();
		int getY();
		int getHealth();
		void changeHealth(int change);
		void setXY(int x, int y);
		void move(int x, int y);
		int update();
		void draw();

};

#endif