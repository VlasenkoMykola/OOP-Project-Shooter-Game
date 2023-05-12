#pragma once
#include <vector>
#include "SDL.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "enums.h"
//#include "SDL_image.h"

class Game
{

public:

    Game();
    void Run();
    void Player_Input();
    int GetScore();
    int GetSize();

private:

    bool running = false;
    bool alive = false;
    int fps = 0;
    int enemy_spawn_timer = 0;
    int enemy_spawn_timer_max = 10;//seconds
    int enemy_waves = 0;

    static const int FRAME_RATE = 1000 / 30;//30 FPS
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 640;
    static const int GRID_WIDTH = 32;
    static const int GRID_HEIGHT = 32;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Map map;
    Player player = Player(map.GRID_WIDTH / 2, map.GRID_HEIGHT / 2,&map);

    //    struct { float x = GRID_WIDTH / 2, y = GRID_HEIGHT / 2; } pos;

//    SDL_Point head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    SDL_Point food;
    std::vector<SDL_Point> body;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    int score = 0;

    void ReplaceFood();
    void SpawnEnemy();
    void SpawnBullet(Move bulletdir);
    void UpdateWindowTitle();
    void GameLoop();
    void Render();
    void Update();
    void Close();
};