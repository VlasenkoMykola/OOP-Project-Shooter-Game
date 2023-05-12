#pragma once
#include <vector>
#include "SDL.h"
#include "enums.h"
#include "Map.h"

class Player
{
private:
    //move enum is duplicated from Game for now
//    enum class Move { up, down, left, right, none };


public:
    int size = 1;
    int ammo = 10;
    bool alive = true;
    bool close_game = false;//public value for communicating between Player and Game classes
    struct { float x; float y; } pos;
    float speed = 0.5f;//move speed relative to framerate
    SDL_Point head;
    Move last_dir = Move::none;
    Move dir = Move::none;
    Map* map_pointer;
    Player(float x, float y,Map* new_map) {
        pos.x = x;
        pos.y = y;
        map_pointer = new_map;
        head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    };
    void Update_Pos();
};