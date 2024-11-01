#include "MyEngineSystem.h"

Character::Character() {
	//TODO: actually implement
}

float Character::ang() {
	return _ang;
}

void Character::turn(float turnMult) {
	_ang += turnSpeed * turnMult;
	if (_ang < 0) _ang = std::fmod(_ang, 2 * M_PI); //it works how I need it to with negative numbers
	else if (_ang > 2 * M_PI) _ang = std::fmod(_ang, 2 * M_PI); //fmod is just floating point modulo
	//^code of a girl who really wishes she didn't have to use radians
}

void Character::step(float stepMult) {
	x += moveSpeed * stepMult * sin(ang());
	y += moveSpeed * stepMult * cos(ang()) * -1; //I swear I know what I'm doing
	/*std::cout << moveSpeed * stepMult * sin(ang()) << ", \n";
	std::cout << moveSpeed * stepMult * cos(ang()) << ". \n\n";*/
}

GameArea::GameArea() {
	PC = new Character();
}

void GameArea::cast(float initX, float initY, float ang, bool isFast) {
	//step 1: setup
	float x = initX, y = initY;
	int adr = 1;
	bool loop = true;
	int dx, dy;

	float m = tan(ang); //movement of y relative to x
	std::cout << m << ", " << ang << std::endl;
	float c = y - (m * x); //x where y = 0 
	// y = mx + c

	if (ang <= M_PI) dx = 1; else dx = -1;
	if (m >= 0) dy = 1; else dy = -1;

	//step 2: identify starting tile
	ray[0][0] = floor(x);
	ray[0][1] = floor(y);

	//step 3: perform initial step
	//x = floor(x) + 1;
	//y = (m * x) + c;

	//step 4: itterate
	for (adr = 1; adr < RAY_MAX; adr++) {
		x = floor(x) + dx;
		y = (m * x) + c;
		ray[adr][0] = x;
		ray[adr][1] = floor(y);
	}
}

Map::Map(short cellsIn[MAP_Y][MAP_X]) {
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			cells[y][x] = cellsIn[y][x];
			//this took more tries to get right than I would like to admit
		}
	}
}