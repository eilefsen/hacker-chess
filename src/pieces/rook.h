#pragma once

#include <stdbool.h>

#include "../types.h"

bool validate_rook_move(Board *b, Move m, enum Color c, bool should_print);
