#pragma once
#include <vector>
#include "SDL.h"
#include "enums.h"
#include "Map.h"

class Enemy
{
private:
    int size = 1;


public:
    struct { float x; float y; } pos;
    float speed = 0.125f;//move speed relative to framerate
    SDL_Point head;
    Move last_dir = Move::none;
    Move dir = Move::none;
    Map* map_pointer;
    Enemy(float x, float y,Map* new_map) {
        pos.x = x;
        pos.y = y;
        map_pointer = new_map;
        head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    };
    void Update_Pos();
};