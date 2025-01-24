#include "MyGame.h"

MyGame::MyGame() : AbstractGame() {
	TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 38);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	//makeDebugMap();
	makeDemoMap();

	gameArea = GameArea();
	gameArea.loadMap(demoMap);
	PC = gameArea.getPcPtr();
}

MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {

	if (eventSystem->isPressed(Key::W)) PC->step(1);
	if (eventSystem->isPressed(Key::S)) PC->step(-1);
	if (eventSystem->isPressed(Key::D)) PC->turn(1);
	if (eventSystem->isPressed(Key::A)) PC->turn(-1);

	//if (eventSystem->isPressed(SPACE)) gameArea.TEST_RAY(PC->x, PC->y, PC->ang());
}

void MyGame::update() {
	gameArea.cameraCast(PC->x, PC->y, PC->ang());
}

void MyGame::render() {

	drawCamFull();
	eggFinder();

	//assorted debug methods
	//drawCam();
	//drawMapTD();
	//drawPlayerTD();
}

void MyGame::eggFinder() {
	if (floor(PC->x) == 26 && floor(PC->y) == 25) {
		gfx->setDrawColor(SDL_COLOR_BLACK);
		std::string eggStr = "KITTY_ROSE-CI517-2024";
		gfx->drawText(eggStr, 180, 280);
	}
}

void MyGame::drawPlayerTD() {
	int x = PC->x * TILE_SIZE;
	int y = PC->y * TILE_SIZE;

	gfx->setDrawColor(SDL_COLOR_RED);
	gfx->drawLine(lineWrapper(x, y, x + 100 * sin(PC->ang() - FOV / 2), y - 100 * cos(PC->ang() - FOV / 2)));
	gfx->drawLine(lineWrapper(x, y, x + 100 * sin(PC->ang() + FOV / 2), y - 100 * cos(PC->ang() + FOV / 2)));

	gfx->setDrawColor(SDL_COLOR_BLUE);
	gfx->drawCircle(Point2(x, y), 5);
	//ughh this is going to be so much trig
	gfx->setDrawColor(SDL_COLOR_AQUA);
	//don't look too hard at this
	gfx->drawLine(lineWrapper(x-(5*sin(PC->ang())), y-(-5*cos(PC->ang())), x+(5*sin(PC->ang())), y+(-5*cos(PC->ang()))));
	gfx->drawLine(lineWrapper(x - (3 * sin(PC->ang()-2)), y - (-3 * cos(PC->ang()-2)), x + (5 * sin(PC->ang())), y + (-5 * cos(PC->ang()))));
	gfx->drawLine(lineWrapper(x - (3 * sin(PC->ang()+2)), y - (-3 * cos(PC->ang()+2)), x + (5 * sin(PC->ang())), y + (-5 * cos(PC->ang()))));

	//TD_CAM_POINTS();
	
}

void MyGame::drawMapTD() {
	//this is just debug code it's fine if it's weird and bad
	int tileX = 20;
	int tileY = 20;

	for (int y = 0; y < MAP_Y; y++) {
		for (int x = 0; x < MAP_X; x++) {
			//hardcoding in the debugMap for now
			//since that's what it's for
			if (demoMap->getCell(x, y) != 0) {
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

	/*gfx->setDrawColor(SDL_COLOR_GREEN);
	for (int i = 0; i < RAY_MAX; i++) {
		gfx->drawRect(gameArea.rayX(i) * tileX, gameArea.rayY(i) * tileY, tileX, tileY);
	}*/
}

void MyGame::drawCam() {
	int lineHeight;

	for (int i = 0; i < RES; i++) {
		if (gameArea.camRaysConts[i] == 1) gfx->setDrawColor(SDL_COLOR_BLUE);
		else gfx->setDrawColor(SDL_COLOR_RED);
		lineHeight = 160 - gameArea.camRays[i] * (160/RAY_MAX);
		if (gameArea.camRaysConts[i] != -1)
			gfx->drawLine(lineWrapper(500 + i * 3, 80 - lineHeight / 2, 500 + i * 3, 80 + lineHeight / 2));
	}
}

void MyGame::drawCamFull() {
	int screenWidth = 800;
	int screenHeight = 600;
	int lineHeight;
	float scale;

	//draw "floor" and "ceiling"
	gfx->setDrawScale(Vector2f(1, screenHeight / 2));
	gfx->setDrawColor(SDL_COLOR_GRAY);
	gfx->drawLine(lineWrapper(0, 1, screenWidth, 1));
	gfx->setDrawColor({ (Uint8)64, (Uint8)64, (Uint8)64 });
	gfx->drawLine(lineWrapper(0, 0, screenWidth, 0));

	scale = screenWidth / RES;
	gfx->setDrawScale(Vector2f(scale, 1));

	//choose colour
	for (int i = 0; i < RES; i++) {
		switch (gameArea.camRaysConts[i]) {
		case 1:
			gfx->setDrawColor(SDL_COLOR_RED);
			break;
		case 2:
			gfx->setDrawColor(SDL_COLOR_ORANGE);
			break;
		case 3:
			gfx->setDrawColor(SDL_COLOR_YELLOW);
			break;
		case 4:
			gfx->setDrawColor(SDL_COLOR_GREEN);
			break;
		case 5:
			gfx->setDrawColor(SDL_COLOR_AQUA);
			break;
		case 6:
			gfx->setDrawColor(SDL_COLOR_BLUE);
			break;
		case 7:
			gfx->setDrawColor(SDL_COLOR_PURPLE);
			break;
		case 8:
			gfx->setDrawColor(SDL_COLOR_WHITE);
			break;
		case 9:
			gfx->setDrawColor(SDL_COLOR_BLACK);
			break;
		default:
			//error code pink
			gfx->setDrawColor(SDL_COLOR_PINK);
		}
		lineHeight = screenHeight / gameArea.camRays[i];
		if (gameArea.camRaysConts[i] != -1)
			gfx->drawLine(lineWrapper(i, screenHeight/2 - lineHeight/2, i, screenHeight/2 + lineHeight/2));
	}

	gfx->setDrawScale(Vector2f(1, 1));

}

void MyGame::TD_CAM_POINTS() {
	//draw the point the ray has found on the top-down map
	//used for debugging
	gfx->setDrawColor(SDL_COLOR_GREEN);
	for (int i = 0; i < RES; i++) {
		gfx->drawCircle(Point2(PC->x * TILE_SIZE + gameArea.camRays[i] * TILE_SIZE * sin((PC->ang() - (FOV / 2)) + i * (FOV/RES)), PC->y * TILE_SIZE - gameArea.camRays[i] * TILE_SIZE * cos((PC->ang() - (FOV / 2)) + i * (FOV / RES))), 2);
	}
}

Line2i MyGame::lineWrapper(float x1, float y1, float x2, float y2) {
	//make gfx calls just a little less awful
	return Line2i(Point2(x1, y1), Point2(x2, y2)); 
}

void MyGame::makeDebugMap() {
	//this won't work any more since the dimensions were changed for the full map
	//included for posterity
	//I guess
	short mapDat[MAP_Y][MAP_X] =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	debugMap = new Map(mapDat);
	std::cout << "Map Written\n";
}

void MyGame::makeDemoMap() {
	short mapDat[MAP_X][MAP_Y] =
	{
		
		{8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8},
		{8,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,8,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,4,0,0,0,4,0,0,8},
		{8,8,0,8,8,8,8,8,8,8,8,8,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,8,5,5,5,5,5,5,8,0,0,1,1,1,1,1,1,1,0,0,0,4,0,0,0,4,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,8,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,8,5,5,5,5,5,0,8,0,0,0,0,1,1,1,0,0,0,0,0,4,0,0,0,4,0,0,8},
		{8,8,8,8,8,8,8,8,8,8,0,8,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,6,6,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,6,6,6,6,6,6,6,6,0,0,8},
		{8,0,0,6,6,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,6,6,6,6,6,6,6,6,0,0,8},
		{8,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,0,0,8},
		{8,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,3,6,6,0,0,8},
		{8,0,0,6,6,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,6,6,6,6,3,0,3,6,0,0,8},
		{8,0,0,6,6,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,6,6,6,6,6,0,6,6,0,0,8},
		{8,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,8},
		{8,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
		{8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8}
	};
	demoMap = new Map(mapDat);
}