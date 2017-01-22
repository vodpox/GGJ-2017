#ifndef JAMMER
#define JAMMER

#include "Game.h"

class Jammer {
	private:
		int x, y;
		int radius = 5;
		int maxHealth = 3;
		int health = maxHealth;
		int space[256][256];
		tplay::Game *game;
	
	public:
		Jammer(tplay::Game *game, int x, int y);
		bool inRange(int x, int y);
		int getX();
		int getY();
		bool update();
		void draw();
};

#endif