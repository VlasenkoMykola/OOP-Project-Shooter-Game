#include <iostream>
#include <string>
#include <ctime>
#include <SDL.h>
#include "Player.h"

void Player::Update_Pos() {

    switch (dir)
    {
    case Move::up:
        pos.y -= speed;
        pos.x = floorf(pos.x);
        break;

    case Move::down:
        pos.y += speed;
        pos.x = floorf(pos.x);
        break;

    case Move::left:
        pos.x -= speed;
        pos.y = floorf(pos.y);
        break;

    case Move::right:
        pos.x += speed;
        pos.y = floorf(pos.y);
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

        //body code from original snake game project is no longer necessary

        /*
        // If we are growing, just make a new neck
        if (growing > 0)
        {
            size++;
            body.push_back(head);
            growing--;
            grid[player.head.x][player.head.y] = Block::body;
        }
        else
        {
            // We need to shift the body
            SDL_Point free = head;
            vector<SDL_Point>::reverse_iterator rit = body.rbegin();
            for (; rit != body.rend(); ++rit)
            {
                grid[free.x][free.y] = Block::body;
                swap(*rit, free);
            }

            grid[free.x][free.y] = Block::empty;
        }
        */
    }

    head.x = new_x;
    head.y = new_y;

    //used to communicate with other classes like Enemy
    map_pointer->player_head.x = new_x;
    map_pointer->player_head.y = new_y;

    //collision check with enemies:
    Block& next = map_pointer->grid[head.x][head.y];
    if (next == Block::enemy)
    {
        alive = false;
        close_game = true;
    }

}

