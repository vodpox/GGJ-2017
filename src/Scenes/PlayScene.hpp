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


struct Crate {
	int x, y;
};


class PlayScene : public tplay::Scene {
	private:
		int level;
		int endX, endY;
		int animationPlaying = 0;
		bool playerTurn = true;
		float rayX, rayY;
	
	public:
		std::vector<Enemy> Enemies;
		std::vector<Door> Doors;
		std::vector<Crate> Crates;
		
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
		int nearDoor(int x, int y);
		int nearCrate(int x, int y);
		void ray(int x1, int y1, int x2, int y2);
};

#endif
