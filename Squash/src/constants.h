#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants
{
	const int MAX_TILE_WIDTH = 742;
	const int MIN_TILE_HEIGHT = 371;
	const int MAX_TILE_HEIGHT = 1280;
	const float TILE_SIZE = 185.5f / 2.f;//60.f;
    const unsigned int WINDOW_WIDTH = 1280;
    const unsigned int WINDOW_HEIGHT = 720;
    const char* const WINDOW_TITLE = "SQWOOOSH!";
    const float SECONDS_PER_FRAME = 1.f / 60.f;

    const float SQUASHBALL_RADIUS = 0.02f;
    const float SQUASHBALL_MASS = 0.024f;
    const float FOOTBALL_RADIUS = 0.11f;
    const float FOOTBALL_MASS = 0.425f;
}

#endif //CONSTANTS_H
