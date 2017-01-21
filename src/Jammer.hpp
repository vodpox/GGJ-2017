#ifndef JAMMER
#define JAMMER

#include "Game.h"

class Jammer {
	private:
		int x = 30;
		int y = 10;
		int radius = 5;
		int maxHealth = 3;
		int health = maxHealth;
		int space[256][256];
		tplay::Game *game;
	
	public:
		Jammer(tplay::Game *game);
		bool inRange(int x, int y);
		void update();
		void draw();
};

#endif