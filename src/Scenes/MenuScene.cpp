#include "Game.h"
#include "MenuScene.hpp"
#include "PlayScene.hpp"


MenuScene::MenuScene(tplay::Game *game) {
	this->game = game;
	this->game->graphics.setCamera(&camera);
}

void MenuScene::update() {
    if (game->input.isButtonDown(tplay::Keyboard::S)) {
		option++;
	}else if(game->input.isButtonDown(tplay::Keyboard::W)){
		option--;
	}else if(game->input.isButtonDown(tplay::Keyboard::ENTER)){
		if(option == 1){
			PlayScene *playScene = new PlayScene(game, 1);
			game->setScene(playScene);
		}else if(option == 2){
			PlayScene *playScene = new PlayScene(game, -1);
			game->setScene(playScene);
		}else if(option == 3){
			game->quit();
		}
	}
	
	if(option > 3){
		option = 1;
	}else if(option < 1){
		option = 3;
	}
	
	if(option == 1){
		option1 = option1textOn;
		option2 = option2text;
		option3 = option3text;
	}else if(option == 2){	
		option1 = option1text;
		option2 = option2textOn;
		option3 = option3text;
	}else if(option == 3){
		option1 = option1text;
		option2 = option2text;
		option3 = option3textOn;
	}
}


void MenuScene::draw() {
	game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 11, 18, "Global Gaming Jam 2017");
	
	game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 7, 12, option1);
	game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 7, 10, option2);
	game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 7, 8, option3);
}