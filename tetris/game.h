#ifndef GAME_H_
#define GAME_H_

#define SCREEN_WIDTH 860  // scrn width for game board
#define SCREEN_HEIGHT 860 // scrn height for game

#define ROWS 20
#define COLUMNS 10
#define GAME_SCREEN_WIDTH 430  // scrn width for game board
#define GAME_SCREEN_HEIGHT 860 // scrn height for game
#define CELL_WIDTH (GAME_SCREEN_WIDTH / COLUMNS)
#define CELL_HEIGHT (GAME_SCREEN_HEIGHT / ROWS)
#define TINY_CELL_W 30
#define TINY_CELL_H 30


#define RUNNING_STATE 0
#define GAME_OVER_STATE 1
#define QUIT_STATE 2
// substates
#define START_SCREEN 0
#define FALLING 1
#define PLACING 2
#define LINE_DESTROYING 3

#define EMPTY 0
#define FALLING_SQUARE 1
#define SETTLED_SQUARE 2

#define CLEAR_BAR SETTLED_SQUARE*COLUMNS
// insert struct
// maybe an enum
typedef struct {
    int play_area[ROWS][COLUMNS];
    const int tetrominos[7][4][4][4];
    int cached_index[4];
    int tet_placement[2];
    int piece_index;
    int delay_in_ms;
    int tet_rotation;
    int alt_index;
    //int collision_codes[9];
    int score;
    int line_cleared;
    int down_time;
    int state;

    char score_c[50];
    char lines_c[50];

    int alt_init;
} game_t;

#endif // !DEBUG
