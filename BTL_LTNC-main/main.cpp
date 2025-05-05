#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "params.h"
#include "objects.h"
#include "collisions.h"
#include "game.h"
#include "utils.h"
#include <SDL_mixer.h>

using namespace std;


int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    if (TTF_Init() == -1){
        cout << "Unable to initialize SDL2_ttf : " << TTF_GetError();
    }
    int textSize = 50;
    TTF_Font* font = TTF_OpenFont("GravityBold8.ttf", textSize);
    if (font == NULL){
        cout << "Unable to load font " << TTF_GetError();
    }
    SDL_Texture* intro_background = loadTexture("intro_background.jpg", renderer);
    SDL_Texture* lose = NULL;
    SDL_Texture* game_over_bg = loadTexture("game_over.jpg", renderer);
    SDL_Color introColor = {0,0,0};
    SDL_Color white = {255, 255, 255};
    SDL_Texture* instructions = loadTexture("instructions.jpg", renderer);
    int score = 0;


    stringstream playButtonText, quitButtonText, scoreText;

    SDL_Rect playButtonTextRect;
    playButtonTextRect.x = SCREEN_WIDTH/2 - 60;
    playButtonTextRect.y = SCREEN_HEIGHT/2 - 22;
    playButtonTextRect.h = 36;
    playButtonTextRect.w = 120;
    string playText = "PLAY";

    SDL_Rect quitButtonTextRect;
    quitButtonTextRect.x = SCREEN_WIDTH/2 - 50;
    quitButtonTextRect.y = SCREEN_HEIGHT/2 + 62;
    quitButtonTextRect.h = 36;
    quitButtonTextRect.w = 100;

    SDL_Rect scoreTextRect;


    SDL_RenderPresent(renderer);
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        cout <<  "Unable to initialize SDL2_mixer : " << Mix_GetError();
    }
    Mix_Music* music = Mix_LoadMUS("music.mp3");
    if (music == NULL){
        cout << "Unable to load music " << Mix_GetError();
    }
    Mix_Chunk* button_click = Mix_LoadWAV("Modern2.wav");
    if (button_click == NULL){
        cout << "Unable to load button sound " << Mix_GetError();
    }

    SDL_Texture* playButton = NULL;
    SDL_Texture* quitButton = NULL;
    SDL_Texture* button_up = loadTexture("button.png", renderer);
    SDL_Texture* button_down = loadTexture("button_press.png", renderer);

    SDL_Rect play_button_position;
    play_button_position.x = SCREEN_WIDTH/2 - 96;
    play_button_position.y = SCREEN_HEIGHT/2 - 32;
    play_button_position.h = 64;
    play_button_position.w = 192;

    SDL_Rect quit_button_position;
    quit_button_position.x = SCREEN_WIDTH/2 - 96;
    quit_button_position.y = SCREEN_HEIGHT/2 + 52;
    quit_button_position.h = 64;
    quit_button_position.w = 192;

    quitButtonText.str( "" );
    quitButtonText << "Quit";
    SDL_Surface* quitButtonSurface = TTF_RenderText_Solid(font, quitButtonText.str().c_str(), introColor);
    SDL_Texture* quitButtonTextTexture = SDL_CreateTextureFromSurface(renderer, quitButtonSurface);
    SDL_FreeSurface(quitButtonSurface);

    SDL_Event event;
    while (event.type != SDL_QUIT){
        SDL_Delay(1);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, intro_background, NULL, NULL);

        playButtonText.str( "" );
        playButtonText << playText;
        SDL_Surface* playButtonSurface = TTF_RenderText_Solid(font, playButtonText.str().c_str(), introColor);
        SDL_Texture* playButtonTextTexture = SDL_CreateTextureFromSurface(renderer, playButtonSurface);
        SDL_FreeSurface(playButtonSurface);

        scoreText.str( "" );
        scoreText << "Your score : " << score;
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.str().c_str(), white);
        SDL_Texture* scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        SDL_FreeSurface(scoreSurface);


        SDL_RenderCopy(renderer, lose, NULL, NULL);
        SDL_RenderCopy(renderer, playButton, NULL, &play_button_position);
        SDL_RenderCopy(renderer, playButtonTextTexture, NULL, &playButtonTextRect);
        SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);
        SDL_RenderCopy(renderer, quitButton, NULL, &quit_button_position);
        SDL_RenderCopy(renderer, quitButtonTextTexture, NULL, &quitButtonTextRect);

        SDL_DestroyTexture(playButtonTextTexture);
        SDL_DestroyTexture(scoreTextTexture);

        while (SDL_PollEvent(&event) != 0){

            if (event.type == SDL_QUIT){
                SDL_Quit();
                break;
            }
            if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN){
                int x, y;
                SDL_GetMouseState(&x, &y);

                bool inside_play = true, inside_quit = true;

                if( x < play_button_position.x )
                {
                    inside_play = false;
                }

                if( x > play_button_position.x + play_button_position.w )
                {
                    inside_play = false;
                }

                if( y < play_button_position.y )
                {
                    inside_play = false;
                }

                if( y > play_button_position.y + play_button_position.h )
                {
                    inside_play = false;
                }

                if (!inside_play){
                    playButton = button_up;
                }

                if( inside_play )
                {

                    playButton = button_down;
                    switch( event.type )
                    {
                        case SDL_MOUSEMOTION:
                        break;

                        case SDL_MOUSEBUTTONDOWN:
                            Mix_PlayChannel(-1, button_click, 0);
                            SDL_DestroyTexture(playButtonTextTexture);
                            SDL_RenderCopy(renderer, instructions, NULL, NULL);
                            SDL_RenderPresent(renderer);
                            waitUntilKeyPressed();
                            game(window, renderer, font, music, score);
                            scoreTextRect.x = SCREEN_WIDTH/2 - 150;
                            scoreTextRect.y = SCREEN_HEIGHT/2 + 112;
                            scoreTextRect.h = 36;
                            scoreTextRect.w = 300;
                            playText = "REPLAY";
                            play_button_position.y = SCREEN_HEIGHT/2 + 182;
                            playButtonTextRect.x = SCREEN_WIDTH/2 - 70;
                            playButtonTextRect.w = 140;
                            playButtonTextRect.y = SCREEN_HEIGHT/2 + 192;
                            quit_button_position.y = SCREEN_HEIGHT/2 + 262;
                            quitButtonTextRect.y = SCREEN_HEIGHT/2 + 272;
                            lose = game_over_bg;

                            break;

                    }
                }

                if( x < quit_button_position.x )
                {
                    inside_quit = false;
                }
                if( x > quit_button_position.x + quit_button_position.w )
                {
                    inside_quit = false;
                }
                if( y < quit_button_position.y )
                {
                    inside_quit = false;
                }
                if( y > quit_button_position.y + quit_button_position.h )
                {
                    inside_quit = false;
                }
                if (!inside_quit){
                    quitButton = button_up;
                }
                if( inside_quit )
                {
                    quitButton = button_down;
                    switch( event.type )
                    {
                        case SDL_MOUSEMOTION:
                        break;

                        case SDL_MOUSEBUTTONDOWN:

                            SDL_DestroyTexture(quitButtonTextTexture);
                            SDL_Quit();
                            return 0;
                            break;
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);
    }
    return 0;
}

