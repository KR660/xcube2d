#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false), box(5, 5, 30, 30) {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

    for (int i = 0; i < numKeys; i++) {
        std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
        k->isAlive = true;
        k->pos = Point2(getRandom(0, 750), getRandom(0, 550));
        gameKeys.push_back(k);
    }

	makeDebugMap();

	PC = Character();
}

MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	int speed = 3;

	if (eventSystem->isPressed(Key::W)) {
		velocity.y = -speed;
	}

	if (eventSystem->isPressed(Key::S)) {
		velocity.y = speed;
	}

	if (eventSystem->isPressed(Key::A)) {
		velocity.x = -speed;
	}

	if (eventSystem->isPressed(Key::D)) {
		velocity.x = speed;
	}

	if (eventSystem->isPressed(Key::W)) PC.step(1);
	if (eventSystem->isPressed(Key::S)) PC.step(-1);
	if (eventSystem->isPressed(Key::D)) PC.turn(1);
	if (eventSystem->isPressed(Key::A)) PC.turn(-1);
}

void MyGame::update() {
	box.x += velocity.x;
	box.y += velocity.y;

	for (auto key : gameKeys) {
		if (key->isAlive && box.contains(key->pos)) {
			score += 200;
			key->isAlive = false;
			numKeys--;
		}
	}

	velocity.x = 0;
    velocity.y = 0;

	if (numKeys == 0) {
		gameWon = true;
	}
}

void MyGame::render() {
	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawRect(box);

	gfx->setDrawColor(SDL_COLOR_YELLOW);
	for (auto key : gameKeys)
        if (key->isAlive)
		    gfx->drawCircle(key->pos, 5);

	drawMapTD();
	drawPlayerTD();
}

void MyGame::drawPlayerTD() {
	gfx->setDrawColor(SDL_COLOR_BLUE);
	gfx->drawCircle(Point2(PC.x, PC.y), 5);
	//ughh this is going to be so much trig
	gfx->setDrawColor(SDL_COLOR_AQUA);
	//don't look too hard at this
	gfx->drawLine(lineWrapper(PC.x-(5*sin(PC.ang())), PC.y-(-5*cos(PC.ang())), PC.x+(5*sin(PC.ang())), PC.y+(-5*cos(PC.ang()))));
	gfx->drawLine(lineWrapper(PC.x - (3 * sin(PC.ang()-2)), PC.y - (-3 * cos(PC.ang()-2)), PC.x + (5 * sin(PC.ang())), PC.y + (-5 * cos(PC.ang()))));
	gfx->drawLine(lineWrapper(PC.x - (3 * sin(PC.ang()+2)), PC.y - (-3 * cos(PC.ang()+2)), PC.x + (5 * sin(PC.ang())), PC.y + (-5 * cos(PC.ang()))));
}

void MyGame::drawMapTD() {
	//don't mind this weird code I was trying stupid shit that didn't work
	int tileX = 20;
	int tileY = 20;

	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			//hardcoding in the debugMap for now
			//since that's what it's for
			if (debugMap->getCell(x, y) == 1) {
				gfx->setDrawColor(SDL_COLOR_RED);
				gfx->drawRect(x * tileX, y * tileY, tileX, tileY);
				gfx->drawLine(lineWrapper(x * tileX, y * tileY, (x+1) * tileX, (y+1) * tileY));
				gfx->drawLine(lineWrapper(x * tileX, (y+1) * tileY, (x+1) * tileX, y * tileY));
			} else {
				gfx->setDrawColor(SDL_COLOR_GRAY);
				gfx->drawRect(x * tileX, y * tileY, tileX, tileY);
			}
			//std::cout << debugMap->getCell(x, y);
		}
		//std::cout << std::endl;
	}
}

void MyGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	std::string scoreStr = std::to_string(score);
	gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);

	if (gameWon)
		gfx->drawText("YOU WON", 250, 500);
}

Line2i MyGame::lineWrapper(float x1, float y1, float x2, float y2) {
	return Line2i(Point2(x1, y1), Point2(x2, y2)); 
}

void MyGame::makeDebugMap() {
	short mapDat[MAP_Y][MAP_X] =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	debugMap = new Map(mapDat);
	std::cout << "Map Written\n";
}