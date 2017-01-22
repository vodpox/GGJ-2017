#include "Game.h"
#include "PlayScene.hpp"
#include "EndScene.hpp"
#include <cstdio>
#include <string>
#include <unistd.h>
#include <cmath>
#ifdef WIN32
	#include "windows.h"
#endif


void doSleep(int milliseconds) {
	#ifdef WIN32
		Sleep(milliseconds);
	#else
		usleep(milliseconds * 1000);
	#endif
}


PlayScene::PlayScene(tplay::Game *game, tplay::Scene *menuScene, int level) {
	this->game = game;
	this->level = level;
	this->menuScene = menuScene;
	this->game->graphics.setCamera(&camera);
	
	player = new Player(game, this);
	loadMap(level);
}


PlayScene::PlayScene(tplay::Game *game, tplay::Scene *menuScene, PlayScene *oldScene, int level) {
	this->game = game;
	this->level = level;
	this->menuScene = menuScene;
	this->game->graphics.setCamera(&camera);
	
	delete oldScene;
	
	player = new Player(game, this);
	loadMap(level);
}


void PlayScene::loadMap(int level) {
	FILE *filep;
	if (level < 0) {
		filep = fopen( (std::string("Maps/Tutorial") + std::to_string(level * -1) + std::string(".txt")).c_str(), "r");
	}
	else {
		filep = fopen( (std::string("Maps/Level") + std::to_string(level) + std::string(".txt")).c_str(), "r");
	}
	if (filep == NULL) game->quit();
	
	fscanf(filep, "%i %i\n", &mapX, &mapY);
	for (int i = mapY - 1; i >= 0; i--) {
		for (int j = 0; j < mapX; j++) {
			fscanf(filep, "%c", &Map[j][i]);
		}
		fscanf(filep, "\n");
	}
	
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			if (Map[i][j] == '@') {
				player->setXY(i, j);
			}
			else if (Map[i][j] == 'R') {
				Enemies.push_back(Enemy(game, this, i, j));
			}
			else if (Map[i][j] == '=') {
				Crate *crate = new Crate();
				crate->x = i;
				crate->y = j;
				Crates.push_back(crate);
			}
			else if (Map[i][j] == '!') {
				endX = i;
				endY = j;
			}
			else if (Map[i][j] == 'O') {
				int closedX, closedY, openX, openY;
				char door;
				
				if (Map[i-1][j] == '|' || Map[i-1][j] == '-') {
					door = Map[i-1][j];
					closedX = i-1;
					closedY = j;
				}
				else if (Map[i+1][j] == '|' || Map[i+1][j] == '-') {
					door = Map[i+1][j];
					closedX = i+1;
					closedY = j;
				}
				else if (Map[i][j-1] == '|' || Map[i][j-1] == '-') {
					door = Map[i][j-1];
					closedX = i;
					closedY = j-1;
				}
				else if (Map[i][j+1] == '|' || Map[i][j+1] == '-') {
					door = Map[i][j+1];
					closedX = i;
					closedY = j+1;
				}
				if (Map[i-1][j] == '.') {
					openX = i-1;
					openY = j;
				}
				else if (Map[i+1][j] == '.') {
					openX = i+1;
					openY = j;
				}
				else if (Map[i][j-1] == '.') {
					openX = i;
					openY = j-1;
				}
				else if (Map[i][j+1] == '.') {
					openX = i;
					openY = j+1;
				}
				else {
					game->quit();
				}
				
				Doors.push_back(Door(game, i, j, openX, openY, closedX, closedY, door));
			}
		}
	}
	
}


bool PlayScene::collides(int x, int y, bool doDoors) {
	// map
	if (x < 0 || x >= mapX || y < 0 || y >= mapY) return true;
	if (Map[x][y] == '#') return true;
	
	if (Map[x][y] == 'O') return true;
	
	for (int i = 0; i < Crates.size(); i++) {
		if (Crates[i]->x == x && Crates[i]->y == y) return true;
	}
	
	if (doDoors) {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y) return true;
		}
	}
	else {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y && Doors[i].isOpen) return true;
		}
	}
	
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i].getX() == x && Enemies[i].getY() == y) return true;
	}
	
	return false;
}


bool PlayScene::collides(int x, int y, bool doDoors, bool doJamming, int igor) {
	// map
	if (x < 0 || x >= mapX || y < 0 || y >= mapY) return true;
	if (Map[x][y] == '#') return true;
	
	if (Map[x][y] == 'O') return true;
	
	for (int i = 0; i < Crates.size(); i++) {
		if (Crates[i]->x == x && Crates[i]->y == y) return true;
	}
	
	if (doDoors) {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y) return true;
		}
	}
	else {
		for (int i = 0; i < Doors.size(); i++) {
			if (Doors[i].getDoorX() == x && Doors[i].getDoorY() == y && Doors[i].isOpen) return true;
		}
	}
	
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i].getX() == x && Enemies[i].getY() == y && i != igor) return true;
	}
	
	if (doJamming) {
		for (int i = 0; i < Jammers.size(); i++) {
			if (Jammers[i].inRange(x, y)) return true;
		}
	}
	
	return false;
}


int PlayScene::nearDoor(int x, int y) {
	for (size_t i = 0; i < Doors.size(); i++) {
		if (Doors[i].getClosedX() == x - 1 && Doors[i].getClosedY() == y) return i;
		if (Doors[i].getClosedX() == x + 1 && Doors[i].getClosedY() == y) return i;
		if (Doors[i].getClosedX() == x && Doors[i].getClosedY() == y - 1) return i;
		if (Doors[i].getClosedX() == x && Doors[i].getClosedY() == y + 1) return i;
	}
	return -1;
}


int PlayScene::nearCrate(int x, int y) {
	for (size_t i = 0; i < Crates.size(); i++) {
		if (Crates[i]->x == x - 1 && Crates[i]->y == y) return i;
		if (Crates[i]->x == x + 1 && Crates[i]->y == y) return i;
		if (Crates[i]->x == x && Crates[i]->y == y - 1) return i;
		if (Crates[i]->x == x && Crates[i]->y == y + 1) return i;
	}
	return -1;
}


void PlayScene::update() {
	
	// TUTORIAL LOGIC
	if (level == -1) {
		if (!tut1_movement) {
			showingTutorial = true;
			tut1_movement = true;
			currentTut = 1;
		}
		else if (nearDoor(player->getX(), player->getY()) != -1 && !tut1_doors) {
			showingTutorial = true;
			tut1_doors = true;
			currentTut = 3;
		}
		else if (player->getAP() <= 0 && !tut1_turns) {
			showingTutorial = true;
			tut1_turns = true;
			currentTut = 2;
		}
		else if (tut1_movement && tut1_turns && tut1_doors && !tut1_goal && !showingTutorial) {
			showingTutorial = true;
			tut1_goal = true;
			currentTut = 4;
		}
	}
	else if (level == -2 && !tut2_jammers) {
		showingTutorial = true;
		tut2_jammers = true;
		currentTut = 5;
	}
	else if (level == -3 && !tut3_enemies) {
		showingTutorial = true;
		tut3_enemies = true;
		currentTut = 6;
	}
	
	if (sleepTime > 1000) sleepTime = 1000;
	doSleep(sleepTime);
	sleepTime = 0;
	
	if (player->getHealth() <= 0) {
		PlayScene *playScene = new PlayScene(game, menuScene, level);
		game->setScene(playScene);
	}
	else if (player->getX() == endX && player->getY() == endY) {
		if (level > 0) {
			if (level < 3) {
				PlayScene *playScene = new PlayScene(game, menuScene, level + 1);
				game->setScene(playScene);
			}
			else {
				EndScene *endScene = new EndScene(game, menuScene);
				game->setScene(endScene);
			}
		}
		else {
			if (level > -3) {
				PlayScene *playScene = new PlayScene(game, menuScene, level - 1);
				game->setScene(playScene);
			}
			else {
				PlayScene *playScene = new PlayScene(game, menuScene, 1);
				game->setScene(playScene);
			}
		}
	}
	
	
	if(game->input.isButtonDown(tplay::Keyboard::ESCAPE)){
		if(isPaused) isPaused = false;
		else isPaused = true;
	}
	
	if(isPaused){
		if(game->input.isButtonDown(tplay::Keyboard::W)){
			pauseOption--;
		}else if(game->input.isButtonDown(tplay::Keyboard::S)){
			pauseOption++;
		}else if(game->input.isButtonDown(tplay::Keyboard::ENTER)){
			if(pauseOption == 0){
				isPaused = false;
			}else if(pauseOption == 1){
				// Restart level
				PlayScene *playScene = new PlayScene(game, menuScene, level);
				game->setScene(playScene);
			}else if(pauseOption == 2){
				// Change to menu scene
				game->setScene(menuScene);
			}
		}
		
		if(pauseOption > 2) pauseOption = 0;
		else if(pauseOption < 0) pauseOption = 2;
		
		if(pauseOption == 0){
			resumeText = resumeOn;
			restartLevelText = restartLevel;
			quitText = quit;
		}else if(pauseOption == 1){
			resumeText = resume;
			restartLevelText = restartLevelOn;
			quitText = quit;
		}else if(pauseOption == 2){
			resumeText = resume;
			restartLevelText = restartLevel;
			quitText = quitOn;
		}
	}
	else if (showingTutorial) {
		if(game->input.isButtonDown(tplay::Keyboard::ENTER)){
			showingTutorial = false;
		}
	}
	else if (animationPlaying) {
		
	}
	else if (playerTurn) {
		player->update(&playerTurn);
	}
	else { // enemy turn
		for (int i = 0; i < Enemies.size(); i++) {
			if (Enemies[i].update(i) != 0) return;
		}
		//Jammers get --health
		for (int i = 0; i < Jammers.size(); i++) {
			if (!Jammers[i].update()) Jammers.erase(Jammers.begin() + i);
		}
		playerTurn = true;
		player->resetAP();
		for (int i = 0; i < Enemies.size(); i++) {
			Enemies[i].resetAP();
		}
	}
}

void PlayScene::ray(int x1, int y1, int x2, int y2){
	rayX = x1;
	rayY = y1;
	int dx = -(x1 - x2);
	int dy = -(y1 - y2);
	float steps;
	
	if(abs(dx) > abs(dy)) steps = abs(dx);
	else steps = abs(dy);
	
	float xInc = dx / (float)steps;
	float yInc = dy / (float)steps;
	
	for(int i=0; i<steps; i++){
		rayX += xInc;
		rayY += yInc;
		
		game->graphics.addToWorld(round(rayX), round(rayY), "o");
	}
}	

void PlayScene::draw() {
	int termX = game->graphics.getTerminalSizeX();
	int termY = game->graphics.getTerminalSizeY();
	
	camera.setCoordinates(player->getX() - game->graphics.getTerminalSizeX() / 2, player->getY() - game->graphics.getTerminalSizeY() / 2);
	
	//Jammers ranges
	for (int i = 0; i < Jammers.size(); i++) {
		Jammers[i].draw();
	}
	
	//map
	for (int i = 0; i < mapX; i++) {
		for (int j = 0; j < mapY; j++) {
			if (Map[i][j] == '#') {
				game->graphics.addToWorld(i, j, "#");
			}
		}
	}
	for (int i = 0; i < Doors.size(); i++) {
		Doors[i].draw();
	}
	
	//bullet line
	//ray(0, 0, player->getX(), player->getY());
	
	//playa
	player->draw();
	
	// goal
	game->graphics.addToWorld(endX, endY, "!");
	
	//enemmies
	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i].draw(player->getX(), player->getY());
	}
	
	//crates
	for (int i = 0; i < Crates.size(); i++) {
		game->graphics.addToWorld(Crates[i]->x, Crates[i]->y, "=");
	}
	
	//Jammers
	for (int i = 0; i < Jammers.size(); i++) {
		game->graphics.addToWorld(Jammers[i].getX(), Jammers[i].getY(), "o");
	}
	
	// ui
	game->graphics.setFormat(tplay::Format::NEGATIVE);
	
	
	// Important messages
	if (player->getHealth() <= 0) {
		std::string msg0 = "+--------------+";
		std::string msg1 = "| You are dead |";
		std::string msg2 = "+--------------+";
		
		game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 1, msg0);
		game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2, msg1);
		game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 - 1, msg2);
		sleepTime += 2000;
	}
	else if (player->getX() == endX && player->getY() == endY) {
		std::string msg0 = "+-----------------+";
		std::string msg1 = "| Level completed |";
		std::string msg2 = "+-----------------+";
		
		game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 1, msg0);
		game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2, msg1);
		game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 - 1, msg2);
		sleepTime += 2000;
	}
	
	// Meh messages
	if (!playerTurn) {
		std::string msg = "Enemy turn";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (player->getAP() <= 0) {
		std::string msg = "Out of AP";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (player->aiming() && player->getJammers() > 0) {
		std::string msg = "Press E to shoot";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (player->aiming()) {
		std::string msg = "No jammers left";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (nearDoor(player->getX(), player->getY()) != -1) {
		std::string msg = "Press E to open/close door";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	else if (nearCrate(player->getX(), player->getY()) != -1) {
		std::string msg = "Press E to open crate";
		game->graphics.addToScreen(termX / 2 - msg.size()  / 2, termY - 2, msg);
	}
	
	
	//stats
	game->graphics.addToScreen(1, 0, + "Jammers: " + std::to_string(player->getJammers()));
	//game->graphics.addToScreen(termX / 2 - std::string("Jammers: " + std::to_string(player->getJammers())).size() / 2 , 0, "Jammers: " + std::to_string(player->getJammers()));
	game->graphics.addToScreen(termX - 1 - std::string("AP: " + std::to_string(player->getAP())).size(), 0, "AP: " + std::to_string(player->getAP()));
	
	if(isPaused){
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2+4, "+------------------------------------+");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2+3, "|                                    |");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2+2, "|            Game  Paused            |");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2+1, "|                                    |");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2  , "|         " + resumeText + "         |");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2-1, "|         " + restartLevelText +   "         |");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2-2, "|         " + quitText +   "         |");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2-3, "|                                    |");
		game->graphics.addToScreen(game->graphics.getTerminalSizeX() / 2 - 19, game->graphics.getTerminalSizeY() / 2-4, "+------------------------------------+");
	}
	else if (showingTutorial) {
		if (currentTut == 1) {
			std::string msg0 = "+----------------------------------------+";
			std::string msg1 = "|   Welcome to [insert game name here]   |";
			std::string msg2 = "|   Try to walk around using WASD keys   |";
			std::string msg3 = "|                                        |";
			std::string msg4 = "|              > continue <              |";
			std::string msg5 = "+----------------------------------------+";
			
			game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 2, msg0);
			game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2 + 1, msg1);
			game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2, msg2);
			game->graphics.addToScreen(termX / 2 - msg3.size()  / 2, termY / 2 - 1, msg3);
			game->graphics.addToScreen(termX / 2 - msg4.size()  / 2, termY / 2 - 2, msg4);
			game->graphics.addToScreen(termX / 2 - msg5.size()  / 2, termY / 2 - 3, msg5);
		}
		else if (currentTut == 2) {
			std::string msg0 = "+----------------------------------------------+";
			std::string msg1 = "|      In this game you and your enemies       |";
			std::string msg2 = "|        will be taking turns to  move.        |";
			std::string msg3 = "|    You just ran out of action points (AP)    |";
			std::string msg4 = "|       so press enter to end your turn!       |";
			std::string msg5 = "|                                              |";
			std::string msg6 = "|                 > continue <                 |";
			std::string msg7 = "+----------------------------------------------+";
			
			game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 3, msg0);
			game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2 + 2, msg1);
			game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 + 1, msg2);
			game->graphics.addToScreen(termX / 2 - msg3.size()  / 2, termY / 2 , msg3);
			game->graphics.addToScreen(termX / 2 - msg4.size()  / 2, termY / 2 - 1, msg4);
			game->graphics.addToScreen(termX / 2 - msg5.size()  / 2, termY / 2 - 2, msg5);
			game->graphics.addToScreen(termX / 2 - msg6.size()  / 2, termY / 2 - 3, msg6);
			game->graphics.addToScreen(termX / 2 - msg7.size()  / 2, termY / 2 - 4, msg7);
		}
		else if (currentTut == 3) {
			std::string msg0 = "+----------------------------------------------+";
			std::string msg1 = "|     This is a door. Open it using E key.     |";
			std::string msg2 = "|   Enemies that you will find in the future   |";
			std::string msg3 = "|     will have a hard time opening these!     |";
			std::string msg4 = "|                                              |";
			std::string msg5 = "|                 > continue <                 |";
			std::string msg6 = "+----------------------------------------------+";
			
			game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 3, msg0);
			game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2 + 2, msg1);
			game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 + 1, msg2);
			game->graphics.addToScreen(termX / 2 - msg3.size()  / 2, termY / 2 , msg3);
			game->graphics.addToScreen(termX / 2 - msg4.size()  / 2, termY / 2 - 1, msg4);
			game->graphics.addToScreen(termX / 2 - msg5.size()  / 2, termY / 2 - 2, msg5);
			game->graphics.addToScreen(termX / 2 - msg6.size()  / 2, termY / 2 - 3, msg6);	
		}
		else if (currentTut == 4) {
			std::string msg0 = "+-----------------------------------------------+";
			std::string msg1 = "|         Your goal in this game is to          |";
			std::string msg2 = "|   reach the end of each level marked as '!'   |";
			std::string msg3 = "|                  Good luck!                   |";
			std::string msg4 = "|                                               |";
			std::string msg5 = "|                 > continue <                  |";
			std::string msg6 = "+-----------------------------------------------+";
			
			game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 3, msg0);
			game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2 + 2, msg1);
			game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 + 1, msg2);
			game->graphics.addToScreen(termX / 2 - msg3.size()  / 2, termY / 2 , msg3);
			game->graphics.addToScreen(termX / 2 - msg4.size()  / 2, termY / 2 - 1, msg4);
			game->graphics.addToScreen(termX / 2 - msg5.size()  / 2, termY / 2 - 2, msg5);
			game->graphics.addToScreen(termX / 2 - msg6.size()  / 2, termY / 2 - 3, msg6);
		}
		else if (currentTut == 5) {
			std::string msg0 = "+------------------------------------------------------------+";
			std::string msg1 = "|     Crates filled with radio jammers are marked as '='     |";
			std::string msg2 = "|           Go near one and press E to collect it.           |";
			std::string msg3 = "|        Then You can press spacebar to start aiming         |";
			std::string msg4 = "|            and press E when aiming to throw it!            |";
			std::string msg5 = "|   It will  knock out all robots and doors in it's range    |";
			std::string msg6 = "|                       Use it wisely!                       |";
			std::string msg7 = "|                                                            |";
			std::string msg8 = "|                        > continue <                        |";
			std::string msg9 = "+------------------------------------------------------------+";
			
			game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 4, msg0);
			game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2 + 3, msg1);
			game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 + 2, msg2);
			game->graphics.addToScreen(termX / 2 - msg3.size()  / 2, termY / 2 + 1, msg3);
			game->graphics.addToScreen(termX / 2 - msg4.size()  / 2, termY / 2 , msg4);
			game->graphics.addToScreen(termX / 2 - msg5.size()  / 2, termY / 2 - 1, msg5);
			game->graphics.addToScreen(termX / 2 - msg6.size()  / 2, termY / 2 - 2, msg6);
			game->graphics.addToScreen(termX / 2 - msg7.size()  / 2, termY / 2 - 3, msg7);
			game->graphics.addToScreen(termX / 2 - msg8.size()  / 2, termY / 2 - 4, msg8);
			game->graphics.addToScreen(termX / 2 - msg9.size()  / 2, termY / 2 - 5, msg9);
		}
		else if (currentTut == 6) {
			std::string msg0 = "+-------------------------------------------------+";
			std::string msg1 = "|         Enemy robots are marked as 'R'          |";
			std::string msg2 = "|   If one touches you you are as good as dead!   |";
			std::string msg3 = "|             Avoid them at all costs             |";
			std::string msg4 = "|                                                 |";
			std::string msg5 = "|                  > continue <                   |";
			std::string msg6 = "+-------------------------------------------------+";
			
			game->graphics.addToScreen(termX / 2 - msg0.size()  / 2, termY / 2 + 3, msg0);
			game->graphics.addToScreen(termX / 2 - msg1.size()  / 2, termY / 2 + 2, msg1);
			game->graphics.addToScreen(termX / 2 - msg2.size()  / 2, termY / 2 + 1, msg2);
			game->graphics.addToScreen(termX / 2 - msg3.size()  / 2, termY / 2, msg3);
			game->graphics.addToScreen(termX / 2 - msg4.size()  / 2, termY / 2 - 1, msg4);
			game->graphics.addToScreen(termX / 2 - msg5.size()  / 2, termY / 2 - 2, msg5);
			game->graphics.addToScreen(termX / 2 - msg6.size()  / 2, termY / 2 - 3, msg6);
		}
	}
	
	game->graphics.unsetFormat(tplay::Format::NEGATIVE);
}