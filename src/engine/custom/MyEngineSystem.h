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
	float x = 1.5, y = 1.5;
	float ang(); //necesarry to encapsulate the angle so that it can be reset when it wraps around
	void turn(float turnMult);
	void step(float stepMult);
private:
	float _ang = 2;
	float turnSpeed = 0.04;
	float moveSpeed = 0.05;
};


const int MAP_X = 32, MAP_Y = 32;
//screen is 800*600
const int TILE_SIZE = 20;

class Map {
public:
	Map(short cellsIn[MAP_Y][MAP_X]);
	int getCell(int x, int y) { return cells[y][x]; }
private:
	short cells[MAP_Y][MAP_X];
};

//TODO: pass as args instead of using consts
const int RAY_MAX = 32;
const float FOV = 1; //REMEMBER: radians
const int RES = 160; //number of columns per screen

class GameArea {
public:
	GameArea();

	void loadMap(Map* newMap) { mapData = newMap; }
	int getCell(int x, int y) { return mapData->getCell(x, y); }
	int getCell(int vect[2]) { return mapData->getCell(vect[0], vect[1]); } //I don't think this even works

	Character* getPcPtr() { return PC; } //might replace this with a better solution later

	void cast();
	float getRayDistance(short target);
	void TEST_RAY(float x, float y, float ang);
	void cameraCast(float x, float y, float ang);

	//slightly reduce how awkward this 2D array is
	int rayX(int i) { return ray[i][0]; }
	int rayY(int i) { return ray[i][1]; }
	int rayCell(int i) { return mapData->getCell(ray[i][0], ray[i][1]); }

	float camRays[RES];
	short tileContBuffer;
	short camRaysConts[RES];

private:
	Character* PC = nullptr;
	Map* mapData = nullptr;
	int ray[RAY_MAX][2];
	float rayAngle;
	float oX;
	float oY;
};

#endif //I don't know what this does it was like that when I got here