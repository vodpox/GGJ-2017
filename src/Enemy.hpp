#ifndef ENEMY
#define ENEMY

#include "Game.h"
class Enemy;
class PlayScene;
#include "Scenes/PlayScene.hpp"
#include "Player.hpp"


class Enemy {
	private:
		int x = 5;
		int y = 5;
		int AP = 10;
		int maxAP = 10;
		int health = 100;
		int maxHealth = 100;
		int jammerCount = 0;
		int maxJammerCount = 3;
		tplay::Game *game;
		PlayScene *playScene;
	
	public:
		Enemy(tplay::Game *game, PlayScene *playScene, int x, int y);
		int getX();
		int getY();
		int getHealth();
		void resetAP();
		void changeHealth(int change);
		void setXY(int x, int y);
		void move(int x, int y);
		int update(int id);
		void draw(int, int);

};

#endif