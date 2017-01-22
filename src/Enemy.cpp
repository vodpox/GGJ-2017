#include "Game.h"
#include "Enemy.hpp"
#include <cmath>
#include <cstdio>


struct Node {
	int x, y;
	bool passable;
	int h, g, f;
	bool open = false, closed = false;
	Node *parent;
};


struct Coords {
	int x, y;
	Coords(int initX, int initY) : x(initX), y(initY) {}
};





int negative(int i) {
	if (i >= 0) {
		return 1;
	}
	else return -1;
}


bool ray(int x1, int y1, int x2, int y2, int iterate, int *getX, int *getY) {
	int lenghtX = abs(x2 - x1);
	int lenghtY = abs(y2 - y1);
	int lenghtTotal = lenghtX + lenghtY; 
	
	if (iterate >= lenghtTotal) return false;
	else {
		*getX = x1;
		*getY = y1;
		
		for (int i = 0; i < iterate; i++) {
			if (i % (lenghtX / lenghtY) != 0) {
				*getX = *getX + negative(x2 - x1);
			}
			else {
				*getY = *getY + negative(y2 - y1);
			}
		}
		
	}
	return true;
}


Enemy::Enemy(tplay::Game *game, PlayScene *playScene, int x, int y){
	this->game = game;
	this->playScene = playScene;
	setXY(x, y);
}


int Enemy::getX() {
	return x;
}


int Enemy::getY() {
	return y;
}


void Enemy::changeHealth(int change) {
	health += change;
}


void Enemy::resetAP() {
	AP = maxAP;
}


int Enemy::getHealth() {
	return health;
}


void Enemy::move(int x, int y) {
	this->x += x;
	this->y += y;
}


void Enemy::setXY(int x, int y) {
	this->x = x;
	this->y = y;
}


int Enemy::update(int id){
	if (AP <= 0) return 0;
	
	int door = playScene->nearDoor(x, y);
	if (door != -1 && !playScene->Doors[door].isOpen) {
		playScene->Doors[door].toggle();
		AP--;
		return 1;
	}
	
	if (std::abs(playScene->player->getX() - x) + std::abs(playScene->player->getY() - y) <= 1) {
		playScene->player->changeHealth(-1);
		return 1;
	}
	
	// pathfinding !!!!!!!!!!!!!!!!!! OG GOD WHY
	
	y--; // WHY DO I HAVE TO DO THIS
	int goX = 0, goY = 0;
	
	/* - pathfinding pseudo code
		
		init nodes (heuristics and all that (nope, no time for that) )
		
		add first node to open
		
		while (not done) {
			take from open
			
			if this is the target finish:
				trace back until parent is start
			
			else look around:
				ignore closed
				recheck open
				add nonopen/nonclosed
			
		}
		
	*/
	
	// node init
	Node Nodes[playScene->mapX][playScene->mapY];
	for (int i = 0; i < playScene->mapX; i++) {
		for (int j= 0; j < playScene->mapY; j++) {
			Nodes[i][j].x = i;
			Nodes[i][j].y = j;
			//Nodes[i][j].passable = (playScene->Map[i][j] != '#' && playScene->Map[i][j] != 'O');
			Nodes[i][j].passable = !playScene->collides(i, j, false, false, id);
			Nodes[i][j].h = std::abs(x - playScene->player->getX()) + std::abs(y - playScene->player->getY());
		}
	}
	
	// list init
	std::vector<Coords> OpenList;
	OpenList.push_back(Coords(x, y));
	
	// loop de loop
	while (OpenList.size() > 0) {
		
		// if this is the target
		if (OpenList[0].x == playScene->player->getX() && OpenList[0].y == playScene->player->getY()) {
			
			Node traceNode = Nodes[OpenList[0].x][OpenList[0].y];
			
			if (traceNode.x == x && traceNode.y == y) {
				goX = traceNode.x - x;
				goY = traceNode.y - y;
			}
			
			while (!(traceNode.parent->x == x && traceNode.parent->y == y)) {
				traceNode = *traceNode.parent;
			}
			goX = traceNode.x - x;
			goY = traceNode.y - y;
		}
		else {
			// look around the world (or node):
			
			// look up (y++)
			if (OpenList[0].x >= 0 && OpenList[0].x < playScene->mapX && OpenList[0].y + 1 >= 0 && OpenList[0].y + 1 < playScene->mapY) {
				if (Nodes[OpenList[0].x][OpenList[0].y + 1].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
				else if (Nodes[OpenList[0].x][OpenList[0].y + 1].open) { // special check
					// TODO
				}
				else { // add to open
					Nodes[OpenList[0].x][OpenList[0].y + 1].parent = &Nodes[OpenList[0].x][OpenList[0].y];
					Nodes[OpenList[0].x][OpenList[0].y + 1].open = true;
					OpenList.push_back(Coords(OpenList[0].x, OpenList[0].y + 1));
				}
				Nodes[OpenList[0].x][OpenList[0].y].closed = true;
			}
			
			// look down (y--)
			if (OpenList[0].x >= 0 && OpenList[0].x < playScene->mapX && OpenList[0].y - 1 >= 0 && OpenList[0].y - 1 < playScene->mapY) {
				if (Nodes[OpenList[0].x][OpenList[0].y - 1].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
				else if (Nodes[OpenList[0].x][OpenList[0].y - 1].open) { // special check
					// TODO
				}
				else { // add to open
					Nodes[OpenList[0].x][OpenList[0].y - 1].parent = &Nodes[OpenList[0].x][OpenList[0].y];
					Nodes[OpenList[0].x][OpenList[0].y - 1].open = true;
					OpenList.push_back(Coords(OpenList[0].x, OpenList[0].y - 1));
				}
				Nodes[OpenList[0].x][OpenList[0].y].closed = true;
			}
			
			// look left (x--)
			if (OpenList[0].x - 1 >= 0 && OpenList[0].x - 1 < playScene->mapX && OpenList[0].y >= 0 && OpenList[0].y < playScene->mapY) {
				if (Nodes[OpenList[0].x - 1][OpenList[0].y].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
				else if (Nodes[OpenList[0].x - 1][OpenList[0].y].open) { // special check
					// TODO
				}
				else { // add to open
					Nodes[OpenList[0].x - 1][OpenList[0].y].parent = &Nodes[OpenList[0].x][OpenList[0].y];
					Nodes[OpenList[0].x - 1][OpenList[0].y].open = true;
					OpenList.push_back(Coords(OpenList[0].x - 1, OpenList[0].y));
				}
				Nodes[OpenList[0].x][OpenList[0].y].closed = true;
			}
			
			// look right (x++)
			if (OpenList[0].x + 1 >= 0 && OpenList[0].x + 1 < playScene->mapX && OpenList[0].y >= 0 && OpenList[0].y < playScene->mapY) {
				if (Nodes[OpenList[0].x + 1][OpenList[0].y].closed || !Nodes[OpenList[0].x][OpenList[0].y + 1].passable); // ignore
				else if (Nodes[OpenList[0].x + 1][OpenList[0].y].open) { // special check
					// TODO
				}
				else { // add to open
					Nodes[OpenList[0].x + 1][OpenList[0].y].parent = &Nodes[OpenList[0].x][OpenList[0].y];
					Nodes[OpenList[0].x + 1][OpenList[0].y].open = true;
					OpenList.push_back(Coords(OpenList[0].x + 1, OpenList[0].y));
				}
				Nodes[OpenList[0].x][OpenList[0].y].closed = true;
			}
			
		}
		OpenList.erase(OpenList.begin());
	}
	y++; // D:
	
	if (!playScene->collides(x + goX, y + goY, true, true, id)) {
		x += goX;
		y += goY;
		playScene->sleepTime += 100;
		AP--;
		return 1;
	}
	return 0;
}


void Enemy::draw(int playerX, int playerY){
	game->graphics.addToWorld(x, y, "R");
	/*
	int xo, yo;
	for (int i = 0; ray(0, 0, playerX, playerY, i, &xo, &yo); i++) {
		game->graphics.addToWorld(xo, yo, "*");
	}
	game->graphics.addToWorld(0, 0, std::to_string(xo));*/
}
