#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "types.h"

void move_piece(BOARD_T(b), Move m);

// returns true if move sucessful, false if not.
bool make_move(BOARD_T(b), Move m, enum Color c, Coordinate king_pos);

void print_move(Move m);
void print_coordinate(Coordinate pos);
