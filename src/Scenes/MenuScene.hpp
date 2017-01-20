#ifndef MENUSCENE
#define MENUSCENE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"


class MenuScene : public tplay::Scene {
	private:
		int option = 1;
		std::string option1, option2, option3;
		
		std::string option1text   = "     Play     ";
		std::string option1textOn = ">    Play    <";
		std::string option2text   = "   Tutorial   ";
		std::string option2textOn = ">  Tutorial  <";
		std::string option3text   = "     Quit     ";
		std::string option3textOn = ">    Quit    <";
	
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		MenuScene(tplay::Game *game);
		
		
		void update();
		void draw();
};

#endif
