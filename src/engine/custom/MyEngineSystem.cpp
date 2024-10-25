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

Map::Map(short cellsIn[60][80]) {
	cells = cellsIn;
	//TODO: not how this works: write properly later
	//TODO: decide how this system is going to work at a time other than midnight
}