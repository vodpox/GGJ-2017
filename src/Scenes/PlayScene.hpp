#ifndef PLAYSCENE
#define PLAYSCENE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"


class PlayScene : public tplay::Scene {
	private:
		int level;
		
		int mapX, mapY;
		char Map[1000][1000];
	
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		PlayScene(tplay::Game *game, int level);
		
		void update();
		void draw();
		void loadMap(int level);
};

#endif
