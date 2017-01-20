#ifndef PLAYER
#define PLAYER

#include "Game.h"

class Player {
	private:
		int x = 5;
		int y = 5;
		int health = 100;
		int maxHealth = 100;
		int jammerCount = 0;
		int maxJammerCount = 3;
		tplay::Game *game;
	
	public:
		Player(tplay::Game *game);
		int getX();
		int getY();
		int getHealth();
		void changeHealth(int change);
		void changeJammerCount(int change);
		void setXY(int x, int y);
		void move(int x, int y);
		void update();
		void draw();

};

#endif