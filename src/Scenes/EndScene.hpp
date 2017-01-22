#ifndef ENDSCENE
#define ENDSCENE


#include "Game.h"
#include "Scene.h"
#include "Camera.h"
#include "../Player.hpp"


class EndScene : public tplay::Scene {
	private:
		int option = 1;
		std::string option1, option2, option3;
		
		std::string option1text   = "   Quit to menu   ";
		std::string option1textOn = ">  Quit to menu  <";
		/*std::string option2text   = "   Tutorial   ";
		std::string option2textOn = ">  Tutorial  <";
		std::string option3text   = "     Quit     ";
		std::string option3textOn = ">    Quit    <";*/
		
		Player player(tplay::Game *game);
		
		tplay::Scene *menuScene;
	
	public:
		tplay::Game *game;
		tplay::Camera camera;
		
		EndScene(tplay::Game *game, tplay::Scene *menuScene);
		
		
		void update();
		void draw();
};

#endif
