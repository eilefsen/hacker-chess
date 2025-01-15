#include "types.h"

bool validate_in_bounds(Coordinate pos);

bool validate_basic(Move m);

bool validate_takes(Board *b, Coordinate pos, enum Color c);
