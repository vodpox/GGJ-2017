#ifndef DOOR
#define DOOR

#include "Game.h"

class Door {
	private:
		int x, y, openX, openY, closedX, closedY;
		char openDoor, closedDoor;
		bool isOpen = false;
		tplay::Game *game;
	
	public:
		Door(tplay::Game *game, int x, int y, int openX, int openY, int closedX, int closedY, char closedDoor);
		int getX();
		int getY();
		bool toggle();
		bool setOpen(bool isOpen);
		void draw();

};

#endif