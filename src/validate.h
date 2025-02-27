#pragma once

#include "types.h"

bool validate_in_bounds(Coordinate pos);

bool validate_basic(Move m);

bool validate_takes(BOARD_T(b), Coordinate pos, enum Color c);

bool validate_move(BOARD_T(b), Move m, enum Color c);
