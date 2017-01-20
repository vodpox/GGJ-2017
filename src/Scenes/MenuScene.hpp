#ifndef MENUSCENE
#define MENUSCENE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"


class MenuScene : public tplay::Scene {
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		MenuScene(tplay::Game *game);
		
		void update();
		void draw();
};

#endif
