#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

void game(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Music* music, int &score);

#endif // GAME_H_INCLUDED
