#include "Game.h"

#include "Scenes/MenuScene.hpp"


int main() {
	tplay::Game game;
	MenuScene menuScene(&game);
	game.setScene(&menuScene);
	game.loop();
	return 0;
}
