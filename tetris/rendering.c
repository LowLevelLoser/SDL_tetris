#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "./game.h"
#include "./rendering.h"

void render_running_state(SDL_Renderer *renderer, const game_t *game);
void render_grid(SDL_Renderer *renderer); 
void render_right_screen(SDL_Renderer *renderer, const game_t *game);

void render_game(SDL_Renderer *renderer, const game_t *game){
    if(game->state == RUNNING_STATE){
        render_running_state(renderer, game);
        render_right_screen(renderer, game);
	render_grid(renderer);
    }
}

void render_running_state(SDL_Renderer *renderer, const game_t *game){
    SDL_Rect square;
    /*int game->piece_index = game->piece_index;
    int game->tet_rotation = game->tet_rotation;
    int game->tet_placement[0] = game->tet_placement[0];
    int game->tet_placemen[1] = game->tet_placement[1];*/
 
    //render game board
    square.w = CELL_WIDTH;
    square.h = CELL_HEIGHT;
   	for(int x = 0; x < ROWS; ++x)
		for(int y = 0; y < COLUMNS; ++y){
			if(game->play_area[x][y] == SETTLED_SQUARE){
				square.x = y*CELL_WIDTH;
				square.y = x*CELL_HEIGHT;
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, & square);
			}
		}
	

    //render tetrominos
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if(game->tetrominos[game->piece_index][game->tet_rotation][i][j] == FALLING_SQUARE){
                square.x = (j+game->tet_placement[0])*CELL_WIDTH;
                square.y = (i+game->tet_placement[1])*CELL_HEIGHT;
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, & square);
            }
    
}

void render_grid(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int i = 1; i < COLUMNS+1; ++i){
        SDL_RenderDrawLine(renderer, i * CELL_WIDTH, 0, i * CELL_WIDTH, GAME_SCREEN_HEIGHT);
    }
    for(int i = 1; i < ROWS+1; ++i){
        SDL_RenderDrawLine(renderer, 0, i * CELL_HEIGHT, GAME_SCREEN_WIDTH, i * CELL_HEIGHT);
    }
}

void render_right_screen(SDL_Renderer *renderer, const game_t *game){
    SDL_Rect tiny_square;

    tiny_square.w = TINY_CELL_W;
    tiny_square.h = TINY_CELL_H;
    if(game->alt_init == true){
        for(int x = 0; x < 4; x++){
            for(int y = 0; y < 4; y++){
                if(game->tetrominos[game->alt_index][0][y][x] == FALLING_SQUARE){
                    tiny_square.x = (x*TINY_CELL_W)+GAME_SCREEN_WIDTH+148;
                    tiny_square.y = (y*TINY_CELL_H) + 10;
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderFillRect(renderer, & tiny_square);
                }
            }
        }
    }

    for(int i = 0; i < 4; i++){
        for(int x = 0; x < 4; x++){
            for(int y = 0; y < 4; y++){
                if(game->tetrominos[game->cached_index[i]][0][y][x] == FALLING_SQUARE){
                    tiny_square.x = (x*TINY_CELL_W)+GAME_SCREEN_WIDTH+148;
                    tiny_square.y = ((y+i*3)*TINY_CELL_H) + 100;
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderFillRect(renderer, & tiny_square);
                }
            }
        }
    }
}
