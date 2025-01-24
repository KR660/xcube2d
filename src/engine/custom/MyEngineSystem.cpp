#include "MyEngineSystem.h"

Character::Character() {
	//TODO: actually implement
}

float Character::ang() {
	return _ang;
}

void Character::turn(float turnMult) {
	_ang += turnSpeed * turnMult;
	if (_ang < 0) _ang = std::fmod(_ang, 2 * M_PI) + (2 * M_PI); //confusing function outputs negative numbers sometimes
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

void GameArea::cast() {
	//TODO: swap out angle arg with the rayAngle field
	//don't worry about the sedimentary layers of mostly useless and irrelevent comments in this method

	//OVERVIEW:
	//take args as shown
	//write tiles to int array "ray" (list of vector2s)
	//calculate distance somewhere else I guess
	//returning all that sounds like kind of a pain and may not be necesarry every time anyway

	//SETUP:
	//we need to convert from pixel coords to tile coords
	float X0 = oX / TILE_SIZE;
	float Y0 = oY / TILE_SIZE;
	//just chart the fuck out of all this
	//also I think the value of h is basically entirely arbitrary?
	//SUGGESTION: don't push it
	float h = RAY_MAX - 1;
	float dX = (h * sin(rayAngle));
	float dY = 0 - (h * cos(rayAngle));
	float X1 = X0 + dX;
	float Y1= Y0 + dY;
	//I don't think these are used yet
	//because I'm just too good at algebra
	int Xdir = dX >= 0 ? 1 : -1;
	int Ydir = dY >= 0 ? 1 : -1;

	//steps = the larger value between dX and dY
	//can't decide if I like this notation or not
	int steps = abs(dX) > abs(dY) ? abs(dX) : abs(dY);

	//calculate the increment of each step
	//also we've already used delta so we need to think of something worse to call these
	float Xinc = dX / (steps + 1);
	float Yinc = dY / (steps + 1);
	//shitty quickfix to tile-skipping problem
	//causes tiles to be repeated instead
	//if this starts causing problems fix properly
	//(original is just steps, not steps+1)
	//hm I wonder if this is the source of all the problems I'm having
	//PROPOSAL: ignore until it gets worse

	//std::cout << Xinc << ", " << Yinc << std::endl;

	//finally start itterating
	float X = oX;
    float Y = oY;
	float Xb = X; 
	float Yb = Y; 

	//just do the first tile out here I don't care any more
	ray[0][0] = X;
	ray[0][1] = Y;

	//oh they're going to kill me for this one
	int MISTAKE_TIMER = 0;

	for (int i = 1; i < RAY_MAX && MISTAKE_TIMER < RAY_MAX * 2; i++) {
		Xb = X;
		X += Xinc;
		Yb = Y;
		Y += Yinc;
		//if both X and Y have changed, meaning the step was diagonal
		if (floor(Xb) != floor(X) && floor(Yb) != floor(Y)) {
			//figure out a way to determine which side of the line to put the missing tile on
			//and then add it
			
			//uhhhh look
			//I'll add a diagram to explain my working here in the doc
			//because it's a bunch of algebra and geometry
			if ((abs(floor(abs(X)) - abs(Xb))) / abs(Xinc) < abs((floor(abs(Y)) - abs(Yb))) / abs(Yinc)) {
				ray[i][0] = X;
				ray[i][1] = Yb;
			} else {
				ray[i][0] = Xb;
				ray[i][1] = Y;
			}
			i++;
		}
		//look I know this is gross but I'm tired and I want a drink
		if (i < RAY_MAX) {
			ray[i][0] = X;
			ray[i][1] = Y;
		}

	}
	//std::cout << " ~~~~~~~~ \n\n";
	//stolen code by the way
	//and also fucking miles better than my own attempt
	//like I might genuinely be stupid
	//https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/

	//yessssssss it works
	//TODO: modify to include every tile the ray touches
	//that implementation didn't work at all
	//TODO: once more with feeling
	//well that's closer
	//TODO: you fucked something up with all those abs() functions
	//I'm going to lose my fucking mind
	//it still doesn't work unless dx and dy are both positive but it seems... better?
	//less accurate in the +- and -+ quadrants and worse in the -- quadrant
	//but generally okay??
	//rounding error????
	//look maybe this is good enough
	//TODO: have another look if it starts causing even more problems
}

float GameArea::getRayDistance(short target) {
	//TODO: modify from direct to perpendicular distance
	//this version creates a fisheye effect
	//ALT-TODO: edit design docs to pretend this is a deliberate artistic choice

	float x1 = oX;
	float y1 = oY;
	float x2 = oX + (RAY_MAX * sin(rayAngle));
	float y2 = oY - (RAY_MAX * cos(rayAngle));
	int tX, tY;
	float x3, y3, x4, y4;

	//(stolen) formula:
	//float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
	//float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

	//itterate through the buffered raycast to find the target tile
	for (int i = 0; i < RAY_MAX; i++) {
		tX = rayX(i);
		tY = rayY(i);
		if (getCell(tX, tY) != 0) {
			//test collision on all four sides of the tile
			x3 = tX, y3 = tY;
			x4 = tX + 1, y4 = tY;
			//NORTH
			float nA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			float nB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			if (nA < 0 || nA > 1 || nB < 0 || nB > 1) nA = 2; //shut up this is fine
			float out = nA;

			//this implementation is basically completely illegible and also completely awful
			//but the funny thing about that is that it's 4am and it really doesn't feel like my problem right now
			x3 = x4, y3 = y4;
			x4 = x3; y4 = y3 + 1;
			//EAST
			float eA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			float eB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			if (eA < 0 || eA > 1 || eB < 0 || eB > 1) eA = 2;
			if (eA < out) out = eA; //we want the smallest out of all of these ~A values

			//imagine that this made sense to a mad alchemist's fevered mind in an era long forgotten
			//because that's basically the situation here
			x3 = x4, y3 = y4;
			x4 = x3 - 1, y4 = y3;
			//SOUTH
			float sA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			float sB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			if (sA < 0 || sA > 1 || sB < 0 || sB > 1) sA = 2;
			if (sA < out) out = sA;

			x3 = x4, y3 = y4;
			x4 = x3, y4 = y3 - 1;
			//YOU GET THE IDEA
			float wA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			float wB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
			if (wA < 0 || wA > 1 || wB < 0 || wB > 1) wA = 2;
			if (wA < out) out = wA;
			if (out != 2) { //yeah 2 is our exception value; -1 wouldn't work
				//horrible janky way to output the cell that was hit
				tileContBuffer = getCell(tX, tY);
				//std::cout << out << ", ";
				return RAY_MAX * out;
			}
			//if we miss everything just let it continue looping through
			//non-ideal but it's better than nothing
		}
	}
	tileContBuffer = -1;
	return RAY_MAX; //if we don't find anything just return the maximum distance
}

void GameArea::TEST_RAY(float x, float y, float ang) {
	oX = x, oY = y, rayAngle = ang;
	cast();
	std::cout << getRayDistance(1) << std::endl;
}

void GameArea::cameraCast(float x, float y, float ang) {
	oX = x, oY = y;
	//for every column
	for (int i = 0; i < RES; i++) {
		//angle the ray
		rayAngle = (ang - FOV / 2) + i * (FOV / RES);
		//cast and get distance
		cast();
		camRays[i] = getRayDistance(1);
		camRaysConts[i] = tileContBuffer;
		//calling getraydistance writes some data to this buffer
		//this is because programming is fucking hard
		//so cut me some slack
		//I just got out of the hospital
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