#ifndef PLAYER
#define PLAYER

#include "Game.h"

class Player {
	private:
		int x = 20;
		int y = 11;
		int xAim = x;
		int yAim = y;
		int health = 100;
		int maxHealth = 100;
		int jammerCount = 0;
		int maxJammerCount = 3;
		int aimRadius = 6;
		int apMax = 3;
		int ap = apMax;
		bool isAiming = false;
		float distance_to_centre;
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
		void moveAim(int x, int y);
		void update();
		void draw();
		void drawAim();
};

#endif