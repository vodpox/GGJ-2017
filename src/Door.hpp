#ifndef DOOR
#define DOOR

#include "Game.h"
class Door;
#include "Scenes/PlayScene.hpp"

class Door {
	private:
		int x, y, openX, openY, closedX, closedY;
		char openDoor, closedDoor;
		tplay::Game *game;
		PlayScene *playScene;
	
	public:
		bool isOpen = false;
		Door(tplay::Game *game, PlayScene *playScene, int x, int y, int openX, int openY, int closedX, int closedY, char closedDoor);
		int getBaseX();
		int getBaseY();
		int getDoorX();
		int getDoorY();
		int getClosedX();
		int getClosedY();
		bool getJam();
		bool toggle();
		bool setOpen(bool isOpen);
		void draw();

};

#endif