#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "./game.h"
#include "./logic.h"
#include "rendering.h"


void delay(int ms);
void settle_piece(game_t *game);
bool edge_correct(game_t *game);
bool overlaps(game_t *game);
int find_lines(game_t *game);
void clear_lines(game_t *game);
void reset_placement(game_t *game);
void print_piece(game_t *game);
bool collides(game_t *game);
void next_piece(game_t *game);
void switch_piece(game_t *game);

void piece_down(game_t *game);
void piece_left(game_t *game);
void piece_right(game_t *game);
void piece_spin(game_t *game);
void reset_game(game_t *game);

int temp_lines = 0;
long pause;
bool settled = false;
bool can_switch = true;

void run_game(game_t *game) {
    for(int i = 0; i < 4; i++){
        srand(rand());
        game->cached_index[i] = rand()%7;
    }
    next_piece(game);
    reset_placement(game);
    while (game->state != QUIT_STATE) {
        if (game->state == RUNNING_STATE) {
            delay(game->down_time);
            piece_down(game);
        }
    }
    //printf("returned\n");
    return;
}

void keys_pressed(SDL_KeyboardEvent *key, game_t *game) {
    collides(game);
    switch (key->keysym.sym) {
        case SDLK_LEFT:
            piece_left(game);
            //game->tet_placement[0]--;
            //collision_detector(game, true);
            //if (game->collision_codes[1]) {
            //  game->tet_placement[0]++;
            //  for (int i = 0; i < 9; i++)
            //    game->collision_codes[i] = 0;
            //}
            break;
        case SDLK_RIGHT:
            piece_right(game);
            break;
        case SDLK_DOWN:
            piece_down(game);
            break;//*/
        case SDLK_UP:
            piece_spin(game);
            break;
        case SDLK_SPACE:
            settled = false;
            while(settled == false){
                piece_down(game);
            }
            break;
        case SDLK_c:
            if(can_switch == true)
                switch_piece(game);
            break;
        case SDLK_r:
            reset_game(game);
            break;
        case SDLK_KP_DIVIDE:
            game->piece_index = (game->piece_index + 1) % 7;
            break;
        default: {}
    }
    //delay(10);
    //while()
}

/*================================ movement functions start ==================================*/
void switch_piece(game_t *game){
    int c = game->piece_index;
    if(game->alt_init == true){
        game->piece_index = game->alt_index;
        game->alt_index = c;
    }
    else{
        game->alt_index = c;
        game->alt_init = true;
        game->piece_index = game->cached_index[0];
        next_piece(game);
    }
    reset_placement(game);
    can_switch = false;
}

void piece_down(game_t *game){
    game->tet_placement[1]++;
    if(collides(game)== true){
        if(game->tet_placement[1] <= 1){
            game->state = GAME_OVER_STATE;
            //printf("game over\n");
        }
        game->tet_placement[1]--;
        settle_piece(game);
        //printf("down collision\n");
    }
    print_piece(game);
    //settled = false;
    //reset_overlaps();
}

void piece_left(game_t *game){
    game->tet_placement[0]--;
    while(collides(game) == true){
        game->tet_placement[0]++;
        //printf("left collision\n");
    }
    //reset_overlaps();
}

void piece_right(game_t *game){
    game->tet_placement[0]++;
    while(collides(game) == true){
        game->tet_placement[0]--;
        //printf("right collision\n");
    }
    //reset_overlaps();
}

void piece_spin(game_t *game) {
    if(game->tet_placement[1] == -1)
        game->tet_placement[1]++;
    int c = game->tet_rotation;
    game->tet_rotation = (game->tet_rotation+1)%4;
    pause += 10;
    if(collides(game) == true){
        if(edge_correct(game) == true){
            game->tet_rotation = c;
            //printf("spin collision\n");
        }
    }
}

/*================================ movement functions end ====================================*/

void settle_piece(game_t *game) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (game->tetrominos[game->piece_index][game->tet_rotation][i][j] ==
                    FALLING_SQUARE)
                game->play_area[game->tet_placement[1] + i]
                    [game->tet_placement[0] + j] = SETTLED_SQUARE;
        }
    }
    settled = true;
    game->piece_index = game->cached_index[0];
    next_piece(game);
    reset_placement(game);
    //printf("\npiece: %d\n", game->piece_index);
    clear_lines(game);
    can_switch = true;
}


void delay(int ms) {
    clock_t now, then;
    pause = ms * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

bool collides(game_t *game){
    bool collides = overlaps(game);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][i][j] == FALLING_SQUARE){
                if(game->tet_placement[0]+j >= COLUMNS || game->tet_placement[0]+j < 0){
                    //overlaps[i][j] = 1;
                    //hits++;
                    collides = true;
                    return true;
                }
                else if(game->tet_placement[1]+i >= ROWS){
                    //overlaps[i][j] = 1;
                    //hits++;
                    collides = true;
                    return true;
                }
            }
        }
    }
    return collides;
}

bool edge_correct(game_t *game){
    bool fixed = false;
    /*memmove(//overlaps_diff, //overlaps, sizeof(//overlaps));
    while (collides(game) == true){
        //correction based on comparison
        memmove(//overlaps_diff, //overlaps, sizeof(//overlaps));
        // comparison
    }*/
    for(int i = 0; i < 2; i++)
        if(collides(game) == true)
            game->tet_placement[0]--;
    if(collides(game) == true)
        game->tet_placement[0] = game->tet_placement[0] + 2;

    for(int i = 0; i < 2; i++)
        if(collides(game) == true)
            game->tet_placement[0]++;
    if(collides(game) == true)
        game->tet_placement[0] = game->tet_placement[0] - 2;

    for(int i = 0; i < 2; i++)
        if(collides(game) == true)
            game->tet_placement[1]--;
    if(collides(game) == true)
        game->tet_placement[1] = game->tet_placement[1] + 2;

    //reset_overlaps();
    return collides(game);
}

bool overlaps(game_t *game){
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            if(game->tetrominos[game->piece_index][game->tet_rotation][y][x] == FALLING_SQUARE
               && game->play_area[game->tet_placement[1]+y][game->tet_placement[0]+x] ==  SETTLED_SQUARE){
                //printf("hit at x:%d y:%d \n\n", game->tet_placement[0]+x, game->tet_placement[1]+y);
                return true;
            }
        }
    }

    return false;
}

void clear_lines(game_t *game){
    int line = find_lines(game);
    if(line == -1){
        game->line_cleared += temp_lines;
        game->score += 50*temp_lines*temp_lines;
        temp_lines = 0;
        snprintf(game->score_c, 50, "score: %d", game->score);
        snprintf(game->lines_c, 50, "lines: %d", game->line_cleared);
        return;
    }

    for(int x = 0; x < COLUMNS; x++)
        game->play_area[0][x] = EMPTY;

    if(line > 0){
        for(int x = 0; x < COLUMNS; x++)
            for(int y = line; y > 0; y--){
                game->play_area[y][x] = game->play_area[y-1][x];
            }
        temp_lines++;
    }
    clear_lines(game);
}

int find_lines(game_t *game){
    int total = 0;
    for(int y = 0; y < ROWS; y++){
        for(int x = 0; x < COLUMNS; x++){
            total += game->play_area[y][x];
        }
        if(total == CLEAR_BAR){
            //printf("line %d\n", y);
            return y;
        }
        total = 0;
    }
    total = 0;
    return -1;
}
void next_piece(game_t *game){
    srand(rand());
    int r = rand()%7;
    if(r == game->cached_index[3])
        r = (r+1)%7;
    for(int i = 0; i < 4; i++){
        //printf("i%d=rand:%d\n", i, rand());
        if(i == 3)
            game->cached_index[i] = r;
        else{
            game->cached_index[i] = game->cached_index[i+1];
        }
        //printf("cac %d\n", game->cached_index[i]);
    }
}

void reset_placement(game_t *game){
    switch(game->piece_index){
        case 0:
            game->tet_placement[1] = -1;
            break;
        default:
            game->tet_placement[1] = 0;
    }
    game->tet_rotation = 0;
    game->tet_placement[0] = 3;
}

void print_piece(game_t *game){
    //printf("\npi:%d, rot:%d\n", game->piece_index, game->tet_rotation);
    for(int x = 0; x < 4; x++){
        for(int y = 0; y < 4; y++){
            //printf("%d, ", game->tetrominos[game->piece_index][game->tet_rotation][x][y]);
        }
        //printf("\n");
    }
}


void reset_game(game_t *game){
    memset(game->play_area, EMPTY, sizeof(int)*ROWS*COLUMNS);
    game->state = RUNNING_STATE;
    game->score = 0;
    game->line_cleared = 0;
    strcpy(game->score_c, "score: 0");
    strcpy(game->lines_c, "lines: 0");
    game->alt_init = false;
    reset_placement(game);
}
