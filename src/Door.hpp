#ifndef DOOR
#define DOOR

#include "Game.h"

class Door {
	private:
		int x, y, openX, openY, closedX, closedY;
		char openDoor, closedDoor;
		tplay::Game *game;
	
	public:
		bool isOpen = false;
		Door(tplay::Game *game, int x, int y, int openX, int openY, int closedX, int closedY, char closedDoor);
		int getBaseX();
		int getBaseY();
		int getDoorX();
		int getDoorY();
		bool toggle();
		bool setOpen(bool isOpen);
		void draw();

};

#endif