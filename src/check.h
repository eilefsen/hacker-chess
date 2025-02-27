#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "types.h"
#include "macro.h"

bool detect_check(BOARD_T(b), Coordinate king_pos);
bool detect_checkmate(BOARD_T(b), Coordinate king_pos);

typedef struct MovePtr {
	Move *moves;
	size_t length;
} MovePtr;
MovePtr get_all_valid_moves(BOARD_T(b), enum Color c);

