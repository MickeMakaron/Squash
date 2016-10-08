
#include "Game.h"

#define TILE_SIZE 60.f
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "HELLO WORLD!"

int main()
{
	Game game;
	game.init();
	game.run();

	return 0;
}
