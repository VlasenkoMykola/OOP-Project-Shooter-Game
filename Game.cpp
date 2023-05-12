#include <iostream>
#include <string>
#include <ctime>
#include <SDL.h>
#include "Game.h"

using namespace std;

Game::Game()
{
    for (int i = 0; i < map.GRID_WIDTH; ++i)
        for (int j = 0; j < map.GRID_HEIGHT; ++j)
        {
            map.grid[i][j] = Block::empty;
        }

    srand(static_cast<unsigned int>(time(0)));
}

void Game::Run()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    // Create Window
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    running = true;
    ReplaceFood();
//now enemies are spawned automatically
//    SpawnEnemy();
//    SpawnEnemy();
//    SpawnEnemy();
    GameLoop();
}

void Game::ReplaceFood()
{
    int x, y;
    while (true)
    {
        //spawn food at random x and y coordinates
        x = rand() % map.GRID_WIDTH;
        y = rand() % map.GRID_HEIGHT;

        if (map.grid[x][y] == Block::empty)
        {
            map.grid[x][y] = Block::food;
            food.x = x;
            food.y = y;
            break;
        }
    }
}

void Game::SpawnEnemy()
{
    int x, y;
    while (true)
    {
        //spawn enemy at random x and y coordinates
        x = rand() % map.GRID_WIDTH;
        y = rand() % map.GRID_HEIGHT;

        //avoid spawning enemies too close to player
        if (((x > player.head.x - map.GRID_WIDTH / 3) && (x < player.head.x + map.GRID_WIDTH / 3)) && ((y > player.head.y - map.GRID_HEIGHT / 3) && (y < player.head.y + map.GRID_HEIGHT / 3))) {}
        else {

            if (map.grid[x][y] == Block::empty)
            {
                 Enemy added_enemy = Enemy(static_cast<int>(x), static_cast<int>(y), &map);
                 enemies.push_back(added_enemy);
                 break;
            }
        }
    }
}

void Game::SpawnBullet(Move bulletdir)
{
    int x, y;
        x = player.head.x;
        y = player.head.y;
        Bullet added_bullet = Bullet(static_cast<int>(x), static_cast<int>(y), &map);
        added_bullet.dir = bulletdir;
        bullets.push_back(added_bullet);
}


void Game::Player_Input()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        player.dir = Move::none;//if no keys are pressed, don't move
        if (e.type == SDL_QUIT)
        {
            player.close_game = true;
        }
        else if (e.type == SDL_KEYDOWN)

            if (e.type == SDL_KEYDOWN)
            {

                if (player.ammo > 0) {

                if (e.key.keysym.sym == SDLK_w) {
                    SpawnBullet(Move::up);
                    player.ammo = player.ammo - 1;
                }

                if (e.key.keysym.sym == SDLK_a ) {
                    SpawnBullet(Move::left);
                    player.ammo = player.ammo - 1;
                }

                if (e.key.keysym.sym == SDLK_s) {
                    SpawnBullet(Move::down);
                    player.ammo = player.ammo - 1;
                }

                if (e.key.keysym.sym == SDLK_d) {
                    SpawnBullet(Move::right);
                    player.ammo = player.ammo - 1;
                }

                }

//                if (e.key.keysym.sym == SDLK_SPACE) {
//                    SpawnBullet(Move::up);
//                }


                //test comamnd to spawn enemies with Lshift

                if (e.key.keysym.sym == SDLK_LSHIFT ) {
                    SpawnEnemy();
                }

                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    if (player.last_dir != Move::down || player.size == 1)
                        player.dir = Move::up;
                    break;

                case SDLK_DOWN:
                    if (player.last_dir != Move::up || player.size == 1)
                        player.dir = Move::down;
                    break;

                case SDLK_LEFT:
                    if (player.last_dir != Move::right || player.size == 1)
                        player.dir = Move::left;
                    break;

                case SDLK_RIGHT:
                    if (player.last_dir != Move::left || player.size == 1)
                        player.dir = Move::right;
                    break;
                }
            }
    }
}

void Game::GameLoop()
{
    Uint32 before, second = SDL_GetTicks(), after;
    int frame_time, frames = 0;

    while (running)
    {
        before = SDL_GetTicks();

        Player_Input();
        if (player.close_game == true) {
            running = false;
        }

//        PollEvents();
        Update();
        Render();
        UpdateWindowTitle();

        frames++;
        after = SDL_GetTicks();
        frame_time = after - before;

        if (after - second >= 1000)
        {
            fps = frames;
            frames = 0;
            second = after;
            UpdateWindowTitle();
        }

        if (FRAME_RATE > frame_time)
        {
            SDL_Delay(FRAME_RATE - frame_time);
        }
    }

}

void Game::Update()
{
    if (!player.alive)
        return;

    player.Update_Pos();

    for (Enemy& enemy : enemies)
    {
        enemy.Update_Pos();
    }

    for (Bullet& bullet : bullets)
    {
        bullet.Update_Pos();
    }

    //delete offscreen bullets:

    int bullet_index = 0;
    while (bullet_index < bullets.size()) {
        if (bullets[bullet_index].offscreen == true) {
            bullets.erase(bullets.begin() + bullet_index);
        }
        else {
            ++bullet_index;
        }
    }

    //delete enemies if colliding with bullet, and increase score:

    int enemy_index = 0;
    bool found_colliding_bullet = false;
    while (enemy_index < enemies.size()) {

        for (Bullet& bullet : bullets)
        {
            if ((enemies[enemy_index].head.x == bullet.head.x) && (enemies[enemy_index].head.y == bullet.head.y)) {
                found_colliding_bullet = true;
                bullet.offscreen = true;//bullet disappears on next frame
                break;
            }
        }

        if (found_colliding_bullet == true) {
            enemies.erase(enemies.begin() + enemy_index);
            found_colliding_bullet = false;
            score++;
        }
        else {
            ++enemy_index;
        }

        ++enemy_index;
    }

    //check collision with enemies, end the game if colliding with any enemy
    for (Enemy& enemy : enemies)
    {
        if ((player.head.x == enemy.head.x) && (player.head.y == enemy.head.y)) {
            player.alive = false;
            player.close_game = true;
        }
    }

    Block& next = map.grid[player.head.x][player.head.y];
    // Check if there's food over here, give extra ammo too
    if (next == Block::food)
    {
        score++;
        player.ammo = player.ammo + 10;
        ReplaceFood();
    }
    // Check if we're dead
    else if (next == Block::body)
    {
        alive = false;
    }

    next = Block::head;

    enemy_spawn_timer = enemy_spawn_timer - 1;
    if (enemy_spawn_timer <= 0) {
        enemy_spawn_timer = enemy_spawn_timer_max * FRAME_RATE;
        int enemies_spawn_amount = 2 + enemy_waves;
        for (int i = 0; i < enemies_spawn_amount;i++) {
            SpawnEnemy();
        }
        enemy_waves = enemy_waves + 1;
        enemy_spawn_timer_max = enemy_spawn_timer_max + 0.25;//later waves spawn slightly slower
    }
}

int Game::GetScore()
{
    return score;
}

void Game::UpdateWindowTitle()
{
    string title = "Shooter Game Ammo: " + to_string(player.ammo) + " Score: " + to_string(score) + " FPS : " + to_string(fps);
    SDL_SetWindowTitle(window, title.c_str());
}

void Game::Render()
{
    SDL_Rect block;
    block.w = SCREEN_WIDTH / map.GRID_WIDTH;
    block.h = SCREEN_WIDTH / map.GRID_HEIGHT;

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);//grey color
    SDL_RenderClear(renderer);

    // Render food
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);//green color
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(renderer, &block);

    // Render snake's body
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);//white color
    for (SDL_Point& point : body)
    {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(renderer, &block);
    }

    // Render enemies
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);//red color
//    for (SDL_Point& point : enemies)
    for (Enemy& enemy : enemies)
    {
        block.x = enemy.head.x * block.w;
        block.y = enemy.head.y * block.h;
        SDL_RenderFillRect(renderer, &block);
    }


    // Render bullets
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);//white color
    //    for (SDL_Point& point : enemies)
    for (Bullet& bullet : bullets)
    {
        block.x = bullet.head.x * block.w;
        block.y = bullet.head.y * block.h;
        SDL_RenderFillRect(renderer, &block);
    }

    // Render snake's head
    block.x = player.head.x * block.w;
    block.y = player.head.y * block.h;
    if (player.alive) SDL_SetRenderDrawColor(renderer, 0x00, 0x7A, 0xCC, 0xFF);//blue color
    else       SDL_SetRenderDrawColor(renderer, 0xCC, 0x00, 0x00, 0xFF);//darker red color (to seperate from enemies)
    SDL_RenderFillRect(renderer, &block);

    // Update Screen
    SDL_RenderPresent(renderer);
}

void Game::Close() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}