#ifndef DOOR
#define DOOR

#include "Game.h"

class Door {
	private:
		int x = 5;
		int y = 5;
		tplay::Game *game;
	
	public:
		Door(tplay::Game *game, int x, int y);
		int getX();
		int getY();
		bool toggle();
		bool setOpen(bool isOpen);
		void draw(int, int);

};

#endif