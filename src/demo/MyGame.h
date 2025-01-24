#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

class MyGame : public AbstractGame {
	private:
		GameArea gameArea;
		Character* PC;
		Map* debugMap = nullptr;
		Map* demoMap = nullptr;

		void handleKeyEvents();
		void update();
		void render();
		void drawPlayerTD();
		void drawMapTD();
		void makeDebugMap();
		void makeDemoMap();
		void eggFinder();
		Line2i lineWrapper(float x1, float y1, float x2, float y2);
		void drawCam();
		void drawCamFull();
		void TD_CAM_POINTS();
	public:
        MyGame();
		~MyGame();
};

#endif