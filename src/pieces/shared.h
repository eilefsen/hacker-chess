#pragma once

#include <stdbool.h>

#include "../types.h"

bool validate_straight_move(BOARD_T(b), Move m, Coordinate diff, bool takes);
bool validate_diagonal_move(BOARD_T(b), Move m, Coordinate diff, bool takes);
