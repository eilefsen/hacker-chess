#include "types.h"

bool validate_in_bounds(Coordinate pos);

bool validate_basic(Move m);

bool validate_takes(Board *b, Coordinate pos, enum Color c);

bool detect_check(Board *b, Coordinate king_pos);

bool validate_move(Board *b, Move m, enum Color c);
