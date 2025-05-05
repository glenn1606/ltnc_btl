#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include "params.h"
using namespace std;

struct Spaceship
{
    int x;
    int y;
    int _size;
    int health = MAX_HEALTH;
    SDL_Texture* image;
    double angle;
    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect _spaceship;
        _spaceship.x = x;
        _spaceship.y = y;
        _spaceship.h = _size;
        _spaceship.w = _size;
        SDL_RenderCopyEx(renderer, image, NULL, &_spaceship, angle, NULL, flip);
    }
};
struct Asteroid
{
    double x;
    double y;
    double x_speed;
    double y_speed;
    double _size = 30;
    SDL_Texture* image;
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect _asteroid;
        _asteroid.x = (int)x;
        _asteroid.y = (int)y;
        _asteroid.h = (int)_size;
        _asteroid.w = (int)_size;
        SDL_RenderCopy(renderer, image, NULL, &_asteroid);
    }
    void move()
    {
        x += x_speed;
        y += y_speed;
    }
    bool inside(int minX, int minY, int maxX, int maxY)
    {
        return ((minX <= x && minY <= y) && (x + _size <= maxX && y + _size <= maxY));
    }
};

struct Laser
{
    double x;
    double y;
    double x_speed;
    double y_speed;
    int _size = 15;
    SDL_Texture* image;
    double angle;
    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect _laser;
        _laser.x = (int)x;
        _laser.y = (int)y;
        _laser.h = _size;
        _laser.w = _size;
        SDL_RenderCopyEx(renderer, image, NULL, &_laser, angle, NULL, flip);
    }
    void move()
    {
        x += x_speed;
        y += y_speed;
    }
    bool inside(int minX, int minY, int maxX, int maxY)
    {
        return ((minX <= x && minY <= y) && (x + _size <= maxX && y + _size <= maxY));
    }
};
struct Warning
{
    double x;
    double y;
    int _size = 20;
    double x_speed;
    double y_speed;
    SDL_Texture* image;
    double angle;
    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect _warning;
        _warning.x = x;
        _warning.y = y;
        _warning.h = _size;
        _warning.w = _size;
        SDL_RenderCopyEx(renderer, image, NULL, &_warning, angle, NULL, flip);
    }
    void move()
    {
        x += x_speed;
        y += y_speed;
    }
};
struct Ultimate
{
    int x;
    int y;
    int _size;
    int x_speed = -3;
    int y_speed = -3;
    int _size_speed = 6;
    SDL_Texture* image;
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect _ulti;
        _ulti.x = x;
        _ulti.y = y;
        _ulti.h = _size;
        _ulti.w = _size;
        SDL_RenderCopy(renderer, image, NULL, &_ulti);
    }
    void move()
    {
        x += x_speed;
        y += y_speed;
        _size += _size_speed;
    }
};

struct Enemy
{
    double x;
    double y;
    double x_speed;
    double y_speed;
    int _size;
    SDL_Texture* image;
    double angle;
    vector<Laser> bullet;
    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
    void render(SDL_Renderer* renderer)
    {
        SDL_Rect _enemy;
        _enemy.x = (int)x;
        _enemy.y = (int)y;
        _enemy.h = _size;
        _enemy.w = _size;
        SDL_RenderCopyEx(renderer, image, NULL, &_enemy, angle, NULL, flip);
    }
    void move()
    {
        x += x_speed;
        y += y_speed;
    }
    bool inside(int minX, int minY, int maxX, int maxY)
    {
        return ((minX <= x && minY <= y) && (x + _size <= maxX && y + _size <= maxY));
    }
};

#endif // OBJECTS_H_INCLUDED
