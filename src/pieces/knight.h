#pragma once

#include <stdbool.h>

#include "../types.h"

bool _validate_knight_move_diff(Move m);

bool validate_knight_move(Board *b, Move m, enum Color c, bool should_print);
