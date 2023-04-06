#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "./game.h"
#include "./rendering.h"
#include "./logic.h"

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

void *fallingThread(void *game_buf){
    run_game(game_buf);
}

int main(int arc, char *argv[]){
    //Initializes SDL returns an error if it cant
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    //initializes a window
    SDL_Window *window = SDL_CreateWindow("Tetris",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, //position
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
    // return error if it cannot create window
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    //abstract object that does rendereing in the window black magic
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC);
    //error
    if(renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(TTF_Init() == -1){
        fprintf(stderr, "Could not initialize SDL2_ttf, error: ", TTF_GetError());
    }

    //Load fonts file and set font size
    TTF_Font* ourFont = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 30);
    //confirm it was loaded
    if(ourFont == NULL){
        printf("Could not load font\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    const int r = rand() % 7;
    const int e = EMPTY;
    const int f = FALLING_SQUARE;
    game_t game_buf = {
        .play_area = {EMPTY},

        .tetrominos =
        {
            {
                {// I piece
                    {e,e,e,e},
                    {f,f,f,f},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,f,e},
                    {e,e,f,e},
                    {e,e,f,e},
                    {e,e,f,e},
                },
                {
                    {e,e,e,e},
                    {e,e,e,e},
                    {f,f,f,f},
                    {e,e,e,e},
                },
                {
                    {e,f,e,e},
                    {e,f,e,e},
                    {e,f,e,e},
                    {e,f,e,e},
                }
            },
            {// left L piece
                {
                    {f,e,e,e},
                    {f,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,f,e},
                    {e,f,e,e},
                    {e,f,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {f,f,f,e},
                    {e,e,f,e},
                    {e,e,e,e},
                },
                {
                    {e,f,e,e},
                    {e,f,e,e},
                    {f,f,e,e},
                    {e,e,e,e},
                }
            },
            {//Right L piece
                {
                    {e,e,f,e},
                    {f,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,e,e},
                    {e,f,e,e},
                    {e,f,f,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {f,f,f,e},
                    {f,e,e,e},
                    {e,e,e,e},
                },
                {
                    {f,f,e,e},
                    {e,f,e,e},
                    {e,f,e,e},
                    {e,e,e,e},
                }
            },
            {//Square piece
                {
                    {e,f,f,e},
                    {e,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,f,e},
                    {e,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,f,e},
                    {e,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,f,e},
                    {e,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                }
            },
            {//S piece
                {
                    {e,f,f,e},
                    {f,f,e,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,e,e},
                    {e,f,f,e},
                    {e,e,f,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {e,f,f,e},
                    {f,f,e,e},
                    {e,e,e,e},
                },
                {
                    {f,e,e,e},
                    {f,f,e,e},
                    {e,f,e,e},
                    {e,e,e,e},
                }
            },
            {//T piece
                {
                    {e,f,e,e},
                    {f,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,e,e},
                    {e,f,f,e},
                    {e,f,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {f,f,f,e},
                    {e,f,e,e},
                    {e,e,e,e},
                },
                {
                    {e,f,e,e},
                    {f,f,e,e},
                    {e,f,e,e},
                    {e,e,e,e},
                }
            },
            {//Z piece
                {
                    {f,f,e,e},
                    {e,f,f,e},
                    {e,e,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,f,e},
                    {e,f,f,e},
                    {e,f,e,e},
                    {e,e,e,e},
                },
                {
                    {e,e,e,e},
                    {f,f,e,e},
                    {e,f,f,e},
                    {e,e,e,e},
                },
                {
                    {e,f,e,e},
                    {f,f,e,e},
                    {f,e,e,e},
                    {e,e,e,e},
                }
            },
        },

        //.collision_codes = {0,0,0,0,0,0,0,0,0},
        .tet_rotation = 0,
        .piece_index = r,
        .score = 0,
        .tet_placement = {3,0},
        .cached_index = {0},
        .alt_index = 0,
        .alt_init = false,
        .down_time = 200,
        .line_cleared = 0,

        .score_c = "score: 0",
        .lines_c = "lines: 0"
    };

    pthread_t falling_thread_id;
    if (pthread_create(&falling_thread_id, NULL, fallingThread, &game_buf)) return 1;
    
    //pthread_t render_thread_id;
    //if (pthread_create(&render_thread_id, NULL, renderThread, &rend)) return 1;

 /*   int down = 0;
    int left = 0;
    int right = 0;*/
    SDL_Rect rect1;
    SDL_Rect rect2;
    SDL_Texture *texture1;
    SDL_Texture *texture2;

    //checks if you haven't quit the window
    SDL_Event event; // this has the events
    while(game_buf.state != QUIT_STATE){
        while (SDL_PollEvent(&event)){
            //const Uint8* state = SDL_GetKeyboardState(NULL);
            switch (event.type){
                case SDL_QUIT:
                    game_buf.state = QUIT_STATE;
                    break;
                case SDL_KEYDOWN:
                    //if(game_buf.state == RUNNING_STATE)
                    keys_pressed(&event.key, &game_buf);
                    break;
                default: {}
            }
        }
        SDL_SetRenderDrawColor(renderer, 0/*R*/, 0/*G*/, 0/*B*/, 255); //makes the background black
        SDL_RenderClear(renderer); //makes rendereing visible double buffering
        render_game(renderer, &game_buf);
        get_text_and_rect(renderer, GAME_SCREEN_WIDTH+10, 780, game_buf.score_c, ourFont, &texture1, &rect1);
        get_text_and_rect(renderer, GAME_SCREEN_WIDTH+10, 810, game_buf.lines_c, ourFont, &texture2, &rect2);

        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
        SDL_RenderPresent(renderer);
    }

    if (pthread_join(falling_thread_id, NULL) != 0){
        printf("failed to join thread");
        return 2;
    }
    SDL_DestroyWindow(window);
    TTF_CloseFont(ourFont);
    SDL_Quit();

    return EXIT_SUCCESS;
}

