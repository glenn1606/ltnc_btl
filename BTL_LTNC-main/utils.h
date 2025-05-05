#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <SDL.h>
#include <SDL_ttf.h>
using namespace std;

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer);

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);


void quitSDL(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);


void waitUntilKeyPressed();


#endif // UTILS_H_INCLUDED
