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
	float x = 1, y = 1; //real gamers remember when these were fixed-point binary
	float ang(); //very fancy stuff
	void turn(float turnMult);
	void step(float stepMult);
private:
	float _ang = 0; //this is how you write these right?
	float turnSpeed = 0.02;
	float moveSpeed = 0.05;
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

const int RAY_MAX = 20;

class GameArea {
public:
	GameArea();

	void loadMap(Map* newMap) { mapData = newMap; }
	int getCell(int x, int y) { return mapData->getCell(x, y); }
	int getCell(int vect[2]) { return mapData->getCell(vect[0], vect[1]); } //I don't think this even works

	Character* getPcPtr() { return PC; } //might replace this with a better solution later

	void cast(float x, float y, float ang, bool isFast);
	//I promise this isn't stupid
	int rayX(int i) { return ray[i][0]; }
	int rayY(int i) { return ray[i][1]; }
	int rayCell(int i) { return mapData->getCell(ray[i][0], ray[i][1]); }

private:
	Character* PC = nullptr;
	Map* mapData = nullptr;
	int ray[RAY_MAX][2];
};

#endif //I don't know what this does it was like that when I got here