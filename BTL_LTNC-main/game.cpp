#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "params.h"
#include "objects.h"
#include "collisions.h"
#include "utils.h"
#include "game.h"

using namespace std;

void game(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* music, int &score){

    bool game_over = 0;
    SDL_Color textColor = {255, 255, 255};
    srand(time(0));
    score = 0;

    // TẠO BACKGROUND

    SDL_Rect background_layer1_images[BACKGROUND_FRAMES];
    SDL_Rect background_layer2_images[BACKGROUND_FRAMES];
    SDL_Rect background_layer3_images[BACKGROUND_FRAMES];
    SDL_Texture* background_layer1 = loadTexture("background_layer1.png", renderer);
    SDL_Texture* background_layer2 = loadTexture("background_layer2.png", renderer);
    SDL_Texture* background_layer3 = loadTexture("background_layer3.png", renderer);
    int background_layers_count = 0;
    for (int i = 0; i < BACKGROUND_FRAMES; i++){

        background_layer1_images[i].x = background_layers_count;
        background_layer1_images[i].y = 0;
        background_layer1_images[i].w = 640;
        background_layer1_images[i].h = 360;

        background_layer2_images[i].x = background_layers_count;
        background_layer2_images[i].y = 0;
        background_layer2_images[i].w = 640;
        background_layer2_images[i].h = 360;

        background_layer3_images[i].x = background_layers_count;
        background_layer3_images[i].y = 0;
        background_layer3_images[i].w = 640;
        background_layer3_images[i].h = 360;

        background_layers_count += 640;
    }
    int background_count = 0;

    // NHẠC NỀN VÀ HIỆU ỨNG ÂM THANH

    Mix_PlayMusic(music, -1);

    Mix_Chunk* laser_sound = Mix_LoadWAV("laser_sound.wav");
    if (laser_sound == NULL){
        cout << "Unable to load laser sound " << Mix_GetError();
    }
    Mix_Chunk* explosion_sound = Mix_LoadWAV("explosion_sound.wav");
    if (explosion_sound == NULL){
        cout << "Unable to load explosion sound " << Mix_GetError();
    }
    Mix_Chunk* enemy_bullet_sound = Mix_LoadWAV("alienshoot1.wav");
    if (enemy_bullet_sound == NULL){
        cout << "Unable to load enemy bullet sound " << Mix_GetError();
    }
    Mix_Chunk* damage_sound = Mix_LoadWAV("damage.wav");
    if (damage_sound == NULL){
        cout << "Unable to load damage sound " << Mix_GetError();
    }
    Mix_Chunk* game_over_sound = Mix_LoadWAV("game_over_sound.wav");
    if (game_over_sound == NULL){
        cout << "Unable to load game over sound " << Mix_GetError();
    }

    // CHỮ TRONG GAME

    stringstream currentScoreText, shieldRecoveryText, ultiRecoveryText;

    SDL_Rect currentScoreTextRect;
    currentScoreTextRect.x = 10;
    currentScoreTextRect.y = 5;
    currentScoreTextRect.h = 20;
    currentScoreTextRect.w = 100;

    SDL_Rect shieldRecoveryTextRect;
    shieldRecoveryTextRect.x = 500;
    shieldRecoveryTextRect.y = 5;
    shieldRecoveryTextRect.h = 20;

    SDL_Rect ultiRecoveryTextRect;
    ultiRecoveryTextRect.x = 500;
    ultiRecoveryTextRect.y = 40;
    ultiRecoveryTextRect.h = 20;

    // TRÁI ĐẤT

    SDL_Rect earth_images[EARTH_FRAMES];
    SDL_Texture* earth_series = loadTexture("earths.png", renderer);
    int earth_frames_count = 10;
    for (int i = 0; i < EARTH_FRAMES; i++){
        earth_images[i].x = earth_frames_count;
        earth_images[i].y = 10;
        earth_images[i].w = 76;
        earth_images[i].h = 76;
        earth_frames_count += 96;
    }
    SDL_Rect earth_position;
    earth_position.x = SCREEN_WIDTH/2 - EARTH_SIZE/2;
    earth_position.y = SCREEN_HEIGHT/2 - EARTH_SIZE/2;
    earth_position.w = EARTH_SIZE;
    earth_position.h = EARTH_SIZE;

    // TÀU VŨ TRỤ

    Spaceship ship;
    ship._size = 30;
    ship.x = SCREEN_WIDTH/2 - ship._size/2;
    ship.y = SCREEN_HEIGHT/2 - EARTH_SIZE/2 - ship._size - 10;
    ship.image = loadTexture("spaceship1.png", renderer);
    ship.angle = 0;

    SDL_Rect health_remain;

    SDL_Rect health_lost;
    health_remain.h = 5;
    health_remain.w = ship._size;
    health_lost.h = 5;
    health_lost.w = ship._size;

    // THIÊN THẠCH

    int earth_count = 0, asteroid_time = 0, asteroid_limited_count = 325;

    vector<Asteroid> asteroidGroup;
    Asteroid asteroid;
    asteroid.image = loadTexture("Asteroid_Base.png", renderer);

    // LAZE

    vector<Laser> laserGroup;
    Laser laser;

    laser.image = loadTexture("laser.png", renderer);
    int laser_count = 0, laser_limited_count = 40;
    Uint32 last_shot = 0;
    Uint32 laser_recovery_time = 650;

    // HIỆU ỨNG NỔ

    SDL_Rect explosion[EXPLOSION_FRAMES];
    SDL_Texture* explosion_series = loadTexture("Asteroid_Explode.png", renderer);
    int k = 20 + 96;
    for (int i = 0; i < EXPLOSION_FRAMES; i++){
        explosion[i].x = k;
        explosion[i].y = 20;
        explosion[i].w = 56;
        explosion[i].h = 56;
        k += 96;
    }
    vector<SDL_Rect> collisionGroup;
    vector<int> collisionTime;

    // CẢNH BÁO VỊ TRÍ THIÊN THẠCH

    vector<Warning> warningGroup;
    Warning warning;
    SDL_Texture* appearance[2] = {loadTexture("warning.png", renderer), NULL};

    // CHIÊU THỨC ĐẶC BIỆT

    vector<Ultimate> ultimateGroup;
    Ultimate ulti;
    ulti.image = loadTexture("ulti.png", renderer);
    Uint32 last_ulti = SDL_GetTicks(), ulti_time;

    // KHIÊN

    SDL_Rect shield_images[SHIELD_FRAMES];
    SDL_Texture* shield_series = loadTexture("shield.png", renderer);
    int shield_frames_count = 0;
    for (int i = 0; i < SHIELD_FRAMES; i++){
        shield_images[i].x = shield_frames_count;
        shield_images[i].y = 0;
        shield_images[i].w = 64;
        shield_images[i].h = 64;
        shield_frames_count += 64;
    }
    bool shield_on = 0;
    SDL_Rect shield_position;
    shield_position.w = 60;
    shield_position.h = 60;
    int shield_count = 0;
    Uint32 last_shield = SDL_GetTicks();
    Uint32 shield_time;

    // KẺ ĐỊCH VÀ ĐẠN

    vector<Enemy> enemyGroup;
    Enemy enemy;
    SDL_Texture* enemyImages[2] = {loadTexture("alien0.png", renderer), loadTexture("alien1.png", renderer)};
    enemy._size = ENEMY_SIZE;

    SDL_Texture* laser_images[4] = {loadTexture("rocket_1.png", renderer), loadTexture("rocket_2.png", renderer), loadTexture("rocket_3.png", renderer), loadTexture("rocket_4.png", renderer)} ;
    vector<Laser> enemyBulletGroup;
    Laser enemy_bullet;
    enemy_bullet._size = ENEMY_BULLET_SIZE;

    int enemy_time = 0, enemy_limited_time = 500, enemy_motion_time = 0, enemy_motion_limited_time = 160;

    SDL_Event e;

    while (e.type != SDL_QUIT && game_over == 0)
    {

            SDL_Delay(1);
            SDL_RenderClear(renderer);

            // VẼ BACKGROUND

            SDL_RenderCopy(renderer, background_layer1, &background_layer1_images[background_count/5], NULL);
            SDL_RenderCopy(renderer, background_layer2, &background_layer2_images[background_count/5], NULL);
            SDL_RenderCopy(renderer, background_layer3, &background_layer3_images[background_count/5], NULL);

            SDL_RenderCopy(renderer, earth_series, &earth_images[earth_count/5], &earth_position);

            // VẼ THANH MÁU CỦA TÀU

            health_remain.x = ship.x;
            health_remain.y = ship.y - 10;
            health_lost.x = ship.x;
            health_lost.y = ship.y - 10;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &health_lost);
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &health_remain);

            // VẼ TÀU

            ship.render(renderer);
            shield_position.x = ship.x - 15;
            shield_position.y = ship.y - 15;

            // CÁC TRẠNG THÁI VÀ THÔNG SỐ CỦA KHIÊN

            shield_time = (SHIELD_RECOVERY_TIME - (SDL_GetTicks() - last_shield))/1000;
            if (SDL_GetTicks() - last_shield >= SHIELD_RECOVERY_TIME) shield_time = 0;
            if (shield_on == 1){
                if (SDL_GetTicks() - last_shield > SHIELD_RECOVERY_TIME) shield_time = 0;
                SDL_RenderCopy(renderer, shield_series, &shield_images[shield_count/5], &shield_position);
            }

            if (SDL_GetTicks() - last_shield >= 5000){
                shield_on = 0;
            }

            // KHỞI TẠO VỊ TRÍ RANDOM CHO THIÊN THẠCH VÀ KẺ ĐỊCH

            int asteroid_start_position[4][2] = {{-(SCREEN_WIDTH - ASTEROID_SIZE), rand()%(SCREEN_HEIGHT - ASTEROID_SIZE)},
                                  {SCREEN_WIDTH - ASTEROID_SIZE, rand() % (2*SCREEN_HEIGHT - ASTEROID_SIZE)},
                                  {rand()%(SCREEN_WIDTH - ASTEROID_SIZE), -(SCREEN_HEIGHT - ASTEROID_SIZE)},
                                  {rand()%(SCREEN_WIDTH - ASTEROID_SIZE), (2*SCREEN_HEIGHT - ASTEROID_SIZE)}};
            int enemy_start_position[4][2] = {{40, 40},
                                              {SCREEN_WIDTH - ENEMY_SIZE - 40, 40},
                                              {SCREEN_WIDTH - ENEMY_SIZE - 40, SCREEN_HEIGHT - ENEMY_SIZE - 40},
                                              {40, SCREEN_HEIGHT - ENEMY_SIZE - 40}};

            if (asteroid_time == 0){
                int p = rand()%4;
                asteroid.x = asteroid_start_position[p][0];
                asteroid.y = asteroid_start_position[p][1];
                asteroid.x_speed = (earth_position.x + EARTH_SIZE/2 - asteroid.x)/600;
                asteroid.y_speed = (earth_position.y + EARTH_SIZE/2 - asteroid.y)/600;
                asteroidGroup.push_back(asteroid);
                switch (p)
                {
                    case 0:
                        warning.angle = -90;
                        warning.x = 0;
                        warning.y = asteroid.y;
                        warning.x_speed = 0;
                        warning.y_speed = asteroid.y_speed;
                        break;
                    case 1:
                        warning.angle = 90;
                        warning.x = SCREEN_WIDTH - warning._size;
                        warning.y = asteroid.y;
                        warning.x_speed = 0;
                        warning.y_speed = asteroid.y_speed;
                        break;
                    case 2:
                        warning.angle = 0;
                        warning.x = asteroid.x;
                        warning.y = 0;
                        warning.x_speed = asteroid.x_speed;
                        warning.y_speed = 0;
                        break;
                    case 3:
                        warning.angle = 180;
                        warning.x = asteroid.x;
                        warning.y = SCREEN_HEIGHT - warning._size;
                        warning.x_speed = asteroid.x_speed;
                        warning.y_speed = 0;
                        break;
                }
                warningGroup.push_back(warning);

            }


            if (enemy_time == 0){
                int t = rand() % 4;
                enemy.x = enemy_start_position[t][0];
                enemy.y = enemy_start_position[t][1];
                switch (t)
                {
                case 0:
                    enemy.x_speed = 2;
                    enemy.y_speed = 0;
                    enemy.angle = 0;
                    enemy_bullet.x_speed = 0;
                    enemy_bullet.y_speed = 2;
                    enemy_bullet.angle = 180;
                    break;
                case 1:
                    enemy.x_speed = 0;
                    enemy.y_speed = 2;
                    enemy.angle = 90;
                    enemy_bullet.x_speed = -2;
                    enemy_bullet.y_speed = 0;
                    enemy_bullet.angle = -90;
                    break;
                case 2:
                    enemy.x_speed = -2;
                    enemy.y_speed = 0;
                    enemy.angle = 180;
                    enemy_bullet.x_speed = 0;
                    enemy_bullet.y_speed = -2;
                    enemy_bullet.angle = 0;
                    break;
                case 3:
                    enemy.x_speed = 0;
                    enemy.y_speed = -2;
                    enemy.angle = -90;
                    enemy_bullet.x_speed = 2;
                    enemy_bullet.y_speed = 0;
                    enemy_bullet.angle = 90;
                    break;
                }
                enemyGroup.push_back(enemy);

            }

            // VẼ KẺ ĐỊCH VÀ ĐẠN CỦA CHÚNG

            for (int i = 0; i < enemyGroup.size(); i++){

                if ((enemy_time/20) % 2 == 0) enemyGroup[i].image = enemyImages[0];
                else enemyGroup[i].image = enemyImages[1];
                enemyGroup[i].render(renderer);
                enemyGroup[i].move();
                enemy_bullet.x = enemyGroup[i].x + ENEMY_SIZE/2 - ENEMY_BULLET_SIZE/2;
                enemy_bullet.y = enemyGroup[i].y + ENEMY_SIZE/2 - ENEMY_BULLET_SIZE/2;
                if (enemy_time % 40 == 0) {
                    enemyBulletGroup.push_back(enemy_bullet);
                    Mix_PlayChannel(-1, enemy_bullet_sound, 0);
                }
                if (!enemyGroup[i].inside(40, 40, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40)){
                    enemyGroup.erase(enemyGroup.begin() + i);
                }
            }

            // VẼ ĐẠN KẺ ĐỊCH CHUYỂN ĐỘNG

            for (int j = 0; j < enemyBulletGroup.size(); j++){
                enemyBulletGroup[j].image = laser_images[laser_count/10];
                enemyBulletGroup[j].render(renderer);
                enemyBulletGroup[j].move();

            }

            // VẼ THIÊN THẠCH CHUYỂN ĐỘNG

            for (int i = 0; i < asteroidGroup.size(); i++){
                asteroidGroup[i].render(renderer);
                asteroidGroup[i].move();
            }

            // VẼ CẢNH BÁO CHUYỂN ĐỘNG

            for (int i = 0; i < warningGroup.size(); i++){
                warningGroup[i].image = appearance[0];
                if (asteroidGroup[i].inside(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)){
                    warningGroup[i].image = appearance[1];
                }
                warningGroup[i].render(renderer);
                warningGroup[i].move();
            }

            // VẼ CHIÊU THỨC ĐẶC BIỆT

            for (int i = 0; i < ultimateGroup.size(); i++){
                ultimateGroup[i].render(renderer);
                ultimateGroup[i].move();
                if (ultimateGroup[i].x < earth_position.x - SCREEN_WIDTH && ultimateGroup[i].y  < earth_position.y - SCREEN_HEIGHT){
                    ultimateGroup.erase(ultimateGroup.begin() + i);
                }
            }

            // VA CHẠM GIỮA ĐẠN KẺ ĐỊCH VÀ TÀU / KHIÊN

            for (int i = 0; i < enemyBulletGroup.size(); i++){
                if (collision(enemyBulletGroup[i], shield_position.x + shield_position.w/2, shield_position.y + shield_position.w/2, shield_position.w/2) && shield_on)
                {
                    enemyBulletGroup.erase(enemyBulletGroup.begin() + i);
                }
                if (collision(enemyBulletGroup[i], ship)){
                    if (ship.health > 0){
                        Mix_PlayChannel(-1, damage_sound, 0);
                        ship.health--;
                        health_remain.w = ship._size * ship.health / MAX_HEALTH;
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &health_lost);
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        SDL_RenderFillRect(renderer, &health_remain);
                    }
                    enemyBulletGroup.erase(enemyBulletGroup.begin() + i);
                }
            }

            // VA CHẠM GIỮA THIÊN THẠCH VỚI TÀU / TRÁI ĐẤT

            for (int i = 0; i < asteroidGroup.size(); i++){
                if (collision(asteroidGroup[i], earth_position.x + EARTH_SIZE/2, earth_position.y + EARTH_SIZE/2, EARTH_SIZE/2 - 10) ||
                    collision(asteroidGroup[i], ship) ||
                    (collision(asteroidGroup[i], shield_position.x + shield_position.w / 2, shield_position.y + shield_position.w / 2, shield_position.w / 2 - 5) && shield_on == 1)){
                    if (collision(asteroidGroup[i], ship) && ship.health > 0){
                        Mix_PlayChannel(-1, damage_sound, 0);
                        ship.health--;
                        health_remain.w = ship._size * ship.health / MAX_HEALTH;
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &health_lost);
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        SDL_RenderFillRect(renderer, &health_remain);
                    }
                    if (collision(asteroidGroup[i], shield_position.x + shield_position.w / 2, shield_position.y + shield_position.w / 2, shield_position.w / 2 - 5) && shield_on == 1){
                        Mix_PlayChannel(-1, explosion_sound, 0);
                        score++;
                    }
                    if (collision(asteroidGroup[i], earth_position.x + EARTH_SIZE/2, earth_position.y + EARTH_SIZE/2, EARTH_SIZE/2 - 10)){
                        game_over = 1;
                    }

                    // HIỆU ỨNG NỔ

                    SDL_Rect collision_posision;
                    collision_posision.x = asteroidGroup[i].x - 10;
                    collision_posision.y = asteroidGroup[i].y - 10;
                    collision_posision.w = asteroidGroup[i]._size + 20;
                    collision_posision.h = asteroidGroup[i]._size + 20;
                    asteroidGroup.erase(asteroidGroup.begin() + i);
                    collisionGroup.push_back(collision_posision);
                    int time = 0;
                    collisionTime.push_back(time);
                    warningGroup.erase(warningGroup.begin() + i);

                }
            }

            // VẼ LASER CHUYỂN ĐỘNG

            for (int i = 0; i < laserGroup.size(); i++){
                laserGroup[i].render(renderer);
                laserGroup[i].move();
            }
            for (int i = 0; i < laserGroup.size(); i++){
                if (laserGroup[i].inside(- laserGroup[i]._size - BACKGROUND_WIDTH, - laserGroup[i]._size - BACKGROUND_HEIGHT,
                                         BACKGROUND_WIDTH + laserGroup[i]._size, BACKGROUND_HEIGHT + laserGroup[i]._size) == 0){
                    laserGroup.erase(laserGroup.begin() + i);
                }
            }

            // VA CHẠM GIỮA LASER VÀ THIÊN THẠCH

            for (int i = 0; i < laserGroup.size(); i++){
                for (int j = 0; j < asteroidGroup.size(); j++){
                    if (collision(asteroidGroup[j], laserGroup[i])){
                        Mix_PlayChannel(-1, explosion_sound, 0);
                        score++;

                        // TẦN SUẤT THIÊN THẠCH XUẤT HIỆN VÀ TỐC ĐỘ BẮN LASER TĂNG DẦN

                        if (score > 0 && score % 5 == 0 && asteroid_limited_count > 75){
                            asteroid_limited_count -= 50;
                            laser_recovery_time -= 50;
                        }
                        laserGroup.erase(laserGroup.begin() + i);

                        // HIỆU ỨNG NỔ

                        SDL_Rect collision_posision;
                        collision_posision.x = asteroidGroup[j].x - 10;
                        collision_posision.y = asteroidGroup[j].y - 10;
                        collision_posision.w = asteroidGroup[j]._size + 20;
                        collision_posision.h = asteroidGroup[j]._size + 20;
                        asteroidGroup.erase(asteroidGroup.begin() + j);
                        warningGroup.erase(warningGroup.begin() + j);
                        collisionGroup.push_back(collision_posision);
                        int time = 0;
                        collisionTime.push_back(time);

                    }
                }
            }

            // VA CHẠM GIỮA CHIÊU THỨC ĐẶC BIỆT VÀ THIÊN THẠCH

            for (int i = 0; i < ultimateGroup.size(); i++){
                for (int j = 0; j < asteroidGroup.size(); j++){
                    if (collision(asteroidGroup[j], ultimateGroup[i])){
                        Mix_PlayChannel(-1, explosion_sound, 0);
                        score++;

                        // TẦN SUẤT THIÊN THẠCH XUẤT HIỆN VÀ TỐC ĐỘ BẮN LASER TĂNG DẦN

                        if (score > 0 && score % 5 == 0 && asteroid_limited_count > 75){
                            asteroid_limited_count -= 50;
                            laser_recovery_time -= 50;
                        }

                        // HIỆU ỨNG NỔ

                        SDL_Rect collision_posision;
                        collision_posision.x = asteroidGroup[j].x - 10;
                        collision_posision.y = asteroidGroup[j].y - 10;
                        collision_posision.w = asteroidGroup[j]._size + 20;
                        collision_posision.h = asteroidGroup[j]._size + 20;
                        asteroidGroup.erase(asteroidGroup.begin() + j);
                        warningGroup.erase(warningGroup.begin() + j);
                        collisionGroup.push_back(collision_posision);
                        int time = 0;
                        collisionTime.push_back(time);

                    }
                }

            }

            // VẼ HIỆU ỨNG NỔ

            for (int i = 0; i < collisionGroup.size(); i++){
                SDL_RenderCopy(renderer, explosion_series, &explosion[collisionTime[i]/10], &collisionGroup[i]);
                if (collisionTime[i] == EXPLOSION_FRAMES*10 - 1){
                    collisionGroup.erase(collisionGroup.begin() + i);
                    collisionTime.erase(collisionTime.begin() + i);
                }
            }

            // ĐIỂM SỐ HIỆN TẠI

            currentScoreText.str( "" );
            currentScoreText << "Score : " << score;
            SDL_Surface* currentScoreTextSurface = TTF_RenderText_Solid(font, currentScoreText.str().c_str(), textColor);
            SDL_Texture* currentScoreTextTexture = SDL_CreateTextureFromSurface(renderer, currentScoreTextSurface);
            SDL_FreeSurface(currentScoreTextSurface);
            if (score > 9) currentScoreTextRect.w = 114;
            if (score > 99) currentScoreTextRect.w = 128;
            SDL_RenderCopy(renderer, currentScoreTextTexture, NULL, &currentScoreTextRect);
            SDL_DestroyTexture(currentScoreTextTexture);

            // THỜI GIAN HỒI KHIÊN

            shieldRecoveryText.str( "" );
            if (shield_time > 0 && shield_time < 10) {
                shieldRecoveryTextRect.w = 120;
                shieldRecoveryText << "Shield : " << shield_time;
            }
            else if (shield_time >= 10){
                shieldRecoveryTextRect.w = 134;
                shieldRecoveryText << "Shield : " << shield_time;
            }
            else {
                shieldRecoveryTextRect.w = 184;
                shieldRecoveryText << "Shield : ready ";
            }
            SDL_Surface* shieldRecoveryTextSurface = TTF_RenderText_Solid(font, shieldRecoveryText.str().c_str(), textColor);
            SDL_Texture* shieldRecoveryTextTexture = SDL_CreateTextureFromSurface(renderer, shieldRecoveryTextSurface);
            SDL_FreeSurface(shieldRecoveryTextSurface);
            SDL_RenderCopy(renderer, shieldRecoveryTextTexture, NULL, &shieldRecoveryTextRect);
            SDL_DestroyTexture(shieldRecoveryTextTexture);

            // THỜI GIAN HỒI CHIÊU THỨC ĐẶC BIỆT

            ulti_time = (ULTI_RECOVERY_TIME - (SDL_GetTicks() - last_ulti))/1000;
            if (SDL_GetTicks() - last_ulti >= ULTI_RECOVERY_TIME) ulti_time = 0;
            ultiRecoveryText.str( "" );
            if (ulti_time > 0 && ulti_time < 10) {
                ultiRecoveryTextRect.w = 140;
                ultiRecoveryText << "Ultimate : " << ulti_time;
            }
            else if (ulti_time >= 10){
                ultiRecoveryTextRect.w = 154;
                ultiRecoveryText << "Ultimate : " << ulti_time;
            }
            else {
                ultiRecoveryTextRect.w = 204;
                ultiRecoveryText << "Ultimate : ready ";
            }
            SDL_Surface* ultiRecoveryTextSurface = TTF_RenderText_Solid(font, ultiRecoveryText.str().c_str(), textColor);
            SDL_Texture* ultiRecoveryTextTexture = SDL_CreateTextureFromSurface(renderer, ultiRecoveryTextSurface);
            SDL_FreeSurface(ultiRecoveryTextSurface);
            SDL_RenderCopy(renderer, ultiRecoveryTextTexture, NULL, &ultiRecoveryTextRect);
            SDL_DestroyTexture(ultiRecoveryTextTexture);

            SDL_RenderPresent(renderer);

            for (int i = 0; i < collisionTime.size(); i++){
                collisionTime[i]++;
            }
            background_count++;
            if (background_count == BACKGROUND_FRAMES*5) background_count = 0;
            earth_count++;
            if (earth_count == EARTH_FRAMES * 5) earth_count = 0;
            asteroid_time++;
            if (asteroid_time >= asteroid_limited_count) asteroid_time = 0;
            enemy_time++;
            if (enemy_time >= enemy_limited_time) enemy_time = 0;
            laser_count++;
            if (laser_count >= laser_limited_count) laser_count = 0;
            enemy_motion_time++;
            if (enemy_motion_time >= enemy_motion_limited_time) enemy_motion_time = 0;
            shield_count++;
            if (shield_count >= 60) shield_count = 0;
            if (ship.health == 0){
                game_over = 1;
            }
            while (SDL_PollEvent(&e) != 0) {

                if (e.type == SDL_QUIT){
                    Mix_HaltMusic();
                    return;
                    break;
                }
                if (e.type == SDL_KEYDOWN){
                    switch(e.key.keysym.sym){

                        case SDLK_ESCAPE: break;

                        // BẮN ĐẠN

                        case SDLK_SPACE:

                            laser.angle = ship.angle;
                            laser.x = earth_position.x + EARTH_SIZE/2 + sin(ship.angle * M_PI / 180)*120 - ship._size/2 + laser._size/2;
                            laser.y = earth_position.y + EARTH_SIZE/2 - cos(ship.angle * M_PI / 180)*120 - ship._size/2 + laser._size/2;
                            laser.y_speed = - 5*cos(laser.angle * M_PI / 180);
                            laser.x_speed = 5*sin(laser.angle * M_PI / 180);
                            laser.move();
                            if (SDL_GetTicks() - last_shot >= laser_recovery_time){
                                laserGroup.push_back(laser);
                                last_shot = SDL_GetTicks();
                                Mix_PlayChannel(-1, laser_sound, 0);
                            }

                            break;

                        // DÙNG CHIÊU THỨC ĐẶC BIỆT

                        case SDLK_c:

                            ulti.x = earth_position.x + EARTH_SIZE/2;
                            ulti.y = earth_position.y + EARTH_SIZE/2;
                            ulti._size = 0;
                            if (SDL_GetTicks() - last_ulti >= ULTI_RECOVERY_TIME){
                                ultimateGroup.push_back(ulti);
                                last_ulti = SDL_GetTicks();
                            }
                            break;

                        // DÙNG KHIÊN

                        case SDLK_x:

                            if  (SDL_GetTicks() - last_shield >= SHIELD_RECOVERY_TIME){
                                shield_on = 1;
                                last_shield = SDL_GetTicks();
                                shield_count = 0;
                            }
                            break;

                        default: break;
                    }
                }

                // CỐ ĐỊNH QUỸ ĐẠO CHUYỂN ĐỘNG CỦA TÀU

                if (e.type == SDL_MOUSEMOTION){
                    SDL_GetMouseState(&ship.x, &ship.y);
                    double distanceToEarthCenter = sqrt(((double)ship.x + ship._size/2 - (earth_position.x + EARTH_SIZE/2))*((double)ship.x + ship._size/2 - (earth_position.x + EARTH_SIZE/2))
                                                + ((double)ship.y + ship._size/2 - (earth_position.y + EARTH_SIZE/2))*((double)ship.y + ship._size/2 - (earth_position.y + EARTH_SIZE/2)));
                    ship.angle = asin((ship.x + ship._size/2 - (earth_position.x + EARTH_SIZE/2))/distanceToEarthCenter) * 180 / M_PI;
                    if (ship.y + ship._size/2 > earth_position.y + EARTH_SIZE/2) ship.angle = 180 - ship.angle;
                    if (distanceToEarthCenter != EARTH_SIZE/2 + ship._size/2){
                        ship.x = earth_position.x + EARTH_SIZE/2 + sin(ship.angle * M_PI / 180)*(EARTH_SIZE/2 + ship._size/2) - ship._size/2;
                        ship.y = earth_position.y + EARTH_SIZE/2 - cos(ship.angle * M_PI / 180)*(EARTH_SIZE/2 + ship._size/2) - ship._size/2;
                    }

                }
            }

    }
    if (game_over == 1){
        Mix_HaltMusic();
        Mix_PlayChannel(-1, game_over_sound, 0);
        return;

    }
}
