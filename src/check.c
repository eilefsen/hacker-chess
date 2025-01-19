#include <stdbool.h>
#include <stddef.h>

#include "types.h"
#include "validate.h"

bool detect_check(Board *b, Coordinate king_pos) {
	enum Color king_color = b->pieces[king_pos.y][king_pos.x].color;
	for (size_t i = 0; i < 8; ++i) {
		for (size_t j = 0; j < 8; ++j) {
			Piece p = b->pieces[i][j];
			if (p.kind == None || p.color == king_color) {
				continue;
			}

			Move m = {
				.from = {.x = j, .y = i},
				.to = king_pos
			};

			if (validate_move(b, m, p.color)) {
				return true;
			}
		}
	}
	return false;
}
