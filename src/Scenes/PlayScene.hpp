#ifndef PLAYSCENE
#define PLAYSCENE


#include <vector>

#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "../Player.hpp"
#include "../Enemy.hpp"
#include "../Door.hpp"


class PlayScene : public tplay::Scene {
	private:
		int level;
		
		int mapX, mapY;
		char Map[1000][1000];
		
		int animationPlaying = 0;
		int sleepTime = 0;
		bool playerTurn = true;
		
		std::vector<Door> Doors;
	
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		PlayScene(tplay::Game *game, int level);
		Player *player;
		Enemy *enemy;
		
		void update();
		void draw();
		
		void loadMap(int level);
		bool collides(int x, int y);
};

#endif
