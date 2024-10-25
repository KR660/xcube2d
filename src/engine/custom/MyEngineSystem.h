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
	float moveSpeed = 2;
};

class GameArea {

};

class Map {
	//default window is 800*600
public:
	Map(short cellsIn[60][80]);
	int getCell(int x, int y);
private:
	short cells[60][80];
};

#endif //I don't know what this does it was like that when I got here