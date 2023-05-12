#include <iostream>
#include <string>
#include <ctime>
#include <SDL.h>
#include "Enemy.h"

void Enemy::Update_Pos() {

    if (map_pointer->player_head.x > head.x) {
        dir = Move::right;
    } else if (map_pointer->player_head.x < head.x) {
        dir = Move::left;
    } else if (map_pointer->player_head.y > head.y) {
        dir = Move::down;
    } else if (map_pointer->player_head.y < head.y) {
        dir = Move::up;
    }

//floor rounding is removed, as it is unnecessary and causes the "fast top-left diagonal movement" bug

    switch (dir)
    {
    case Move::up:
        pos.y -= speed;        
//        pos.x = floorf(pos.x);
        break;

    case Move::down:
        pos.y += speed;
//        pos.x = floorf(pos.x);
        break;

    case Move::left:
        pos.x -= speed;
//        pos.y = floorf(pos.y);
        break;

    case Move::right:
        pos.x += speed;
//        pos.y = floorf(pos.y);
        break;
    }

    // Wrap
    if (pos.x < 0) {
        pos.x = map_pointer->GRID_WIDTH - 1;
    }
    else if (pos.x > map_pointer->GRID_WIDTH - 1) {
        pos.x = 0;
    }
    if (pos.y < 0) {
        pos.y = map_pointer->GRID_HEIGHT - 1;
    } else if (pos.y > map_pointer->GRID_HEIGHT - 1) {
        pos.y = 0;
    }
    int new_x = static_cast<int>(pos.x);
    int new_y = static_cast<int>(pos.y);

    // Check if head position has changed
    if (new_x != head.x || new_y != head.y)
    {
        last_dir = dir;
    }

    head.x = new_x;
    head.y = new_y;
}

