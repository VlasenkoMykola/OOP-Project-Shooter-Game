#pragma once
#include <vector>
#include "SDL.h"
#include "enums.h"

#ifndef CLASS_MAP
#define CLASS_MAP

class Map
{
private:


public:
    static const int GRID_WIDTH = 32;
    static const int GRID_HEIGHT = 32;
    Block grid[GRID_WIDTH][GRID_HEIGHT];
    SDL_Point player_head;//copied from player each gameloop, used for communicating with other classes like Enemy
};
#endif