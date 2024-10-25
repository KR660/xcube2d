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

Map::Map(short cellsIn[MAP_Y][MAP_X]) {
	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			cells[y][x] = cellsIn[y][x];
			//this took more tries to get right than I would like to admit
		}
	}
}