#ifndef LOGIC_H_
#define LOGIC_H_

#include "game.h"
void run_game(game_t *game);
bool collides(game_t *game);
void keys_pressed(SDL_KeyboardEvent *key, game_t *game);
//void piece_down(game_t *game);
//void piece_left(game_t *game);
//void piece_right(game_t *game);

#endif // DEBUG
