#include <stdbool.h>

#include "types.h"

bool validate_in_bounds(Move m);

bool validate_takes(Board *b, Coordinate pos, enum Color c);

void move_piece(Board *b, Move m);

// returns true if move sucessful, false if not.
bool make_move(Board *b, Move m, enum Color c);

void print_move(Move m);
void print_coordinate(Coordinate pos);
