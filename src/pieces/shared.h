#include "../types.h"
#include <stdbool.h>

bool validate_straight_move(Board *b, Move m, enum Color c, Coordinate diff, bool takes);
bool validate_diagonal_move(Board *b, Move m, enum Color c, Coordinate diff, bool takes);
