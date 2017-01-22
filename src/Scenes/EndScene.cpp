#include "Game.h"
#include "EndScene.hpp"
#include "MenuScene.hpp"


EndScene::EndScene(tplay::Game *game) {
	this->game = game;
	this->game->graphics.setCamera(&camera);
}

void EndScene::update() {
    if(game->input.isButtonDown(tplay::Keyboard::ENTER)){
		if(option == 1){
			//game->setScene(menuScene);
		}
	}
}


void EndScene::draw() {
	game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 4, 18, "You won!");
	
	game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 9, 12, option1);
}