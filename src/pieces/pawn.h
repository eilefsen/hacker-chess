#pragma once

#include <stdbool.h>

#include "../types.h"

typedef struct PawnMove {
	bool valid;
	bool en_passantable;
	int takes_en_passant; // y coordinate of piece to take. -1 if N/A.
} PawnMove;
PawnMove validate_pawn_move(Board *b, Move m, enum Color c, bool should_print);
