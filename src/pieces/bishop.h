#pragma once

#include "../types.h"
#include <stdbool.h>

bool validate_bishop_move(BOARD_T(b), Move m, enum Color c, bool should_print);
