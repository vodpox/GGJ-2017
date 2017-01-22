#ifndef PLAYER
#define PLAYER

#include "Game.h"
class Player;
class PlayScene;
#include "Scenes/PlayScene.hpp"


class Player {
	private:
		int x = 20;
		int y = 11;
		int xAim = x;
		int yAim = y;
		int health = 1;
		int maxHealth = 1;
		int jammerCount = 0;
		int maxJammerCount = 3;
		int aimRadius = 6;
		int apMax = 15;
		int ap = apMax;
		bool isAiming = false;
		float distance_to_centre;
		tplay::Game *game;
		PlayScene *playScene;
		bool aimSpace[256][256];
		int aimIntervals[256][2];
		int aimYInterval[2];
		/*std::ofstream file;
		file.open("file.txt");*/
		/*char Map[1000][1000];
		bool test = false;*/
		int text1 = 0;
		int text2 = 0;
	
	public:
		Player(tplay::Game *game, PlayScene *playScene);
		int getX();
		int getY();
		int getAimX();
		int getAimY();
		int getHealth();
		int getJammers();
		int getAP();
		void changeHealth(int change);
		void changeJammerCount(int change);
		void setXY(int x, int y);
		void move(int x, int y);
		void moveAim(int x, int y);
		void update(bool *playerTurn);
		void draw();
		void drawAim();
		void interactDoor();
		void resetAP();
		void shoot();
		bool aiming();
		/*void loadMap(int level);*/
};

#endif