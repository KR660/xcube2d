#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct GameKey {
	Point2 pos;
	bool isAlive;
};

class MyGame : public AbstractGame {
	private:
		Rect box;

		Vector2i velocity;

		std::vector<std::shared_ptr<GameKey>> gameKeys;

		/* GAMEPLAY */
		int score, numKeys, lives;
		bool gameWon;
		Character PC;
		Map* debugMap = nullptr;

		void handlePlayerEvents();
		void handleKeyEvents();
		void update();
		void render();
		void drawPlayerTD();
		void drawMapTD();
		void makeDebugMap();
		void renderUI();
		Line2i lineWrapper(float x1, float y1, float x2, float y2);
	public:
        MyGame();
		~MyGame();
};

#endif