#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__
#define _USE_MATH_DEFINES

#include "../EngineCommon.h"
#include <math.h>

class MyEngineSystem {
	friend class XCube2Engine;
	private:

	public:
};

class Character {
public:
	Character();
	float x = 100, y = 100; //real gamers remember when these were fixed-point binary
	float ang(); //very fancy stuff
	void turn(float turnMult);
	void step(float stepMult);
private:
	float _ang = 0; //this is how you write these right?
	float turnSpeed = 0.05;
	float moveSpeed = 1;
};

class GameArea {

};

const int MAP_X = 16, MAP_Y = 12;
//low values for debugging
//screen is 800*600
//which barely means anything but at one point it was based on that
const int TILE_SIZE = 20;

class Map {
	//default window is 800*600
public:
	Map(short cellsIn[MAP_Y][MAP_X]);
	int getCell(int x, int y) { return cells[y][x]; }
private:
	short cells[MAP_Y][MAP_X];
};

#endif //I don't know what this does it was like that when I got here