#ifndef PLAYSCENE
#define PLAYSCENE


#include <vector>

#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "../Player.hpp"
#include "../Enemy.hpp"
#include "../Door.hpp"
#include "../Jammer.hpp"


class PlayScene : public tplay::Scene {
	private:
		int level;
		
		int animationPlaying = 0;
		bool playerTurn = true;
		
		std::vector<Door> Doors;
		std::vector<Enemy> Enemies;
	
	public:
		
		int sleepTime = 0;
		int mapX, mapY;
		char Map[1000][1000];
		
		tplay::Game *game;
		tplay::Camera camera;
		
		PlayScene(tplay::Game *game, int level);
		Player *player;
		Jammer *jammer;
		
		void update();
		void draw();
		
		void loadMap(int level);
		bool collides(int x, int y, bool doDoors);
		bool collides(int x, int y, bool doDoors, int igor);
};

#endif
