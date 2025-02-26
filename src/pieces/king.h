#pragma once

#include <stdbool.h>

#include "../types.h"

enum Castle {
	Short_Castle = 1,
	Long_Castle = -1,
	None_Castle = 0,
};

typedef struct KingMove {
	bool valid;
	enum Castle castle;
} KingMove;

KingMove validate_king_move(Board *b, Move m, enum Color c, bool should_print);
