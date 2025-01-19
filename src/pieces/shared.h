#include <stdbool.h>

#include "../types.h"

bool validate_straight_move(Board *b, Move m, Coordinate diff, bool takes);
bool validate_diagonal_move(Board *b, Move m, Coordinate diff, bool takes);
