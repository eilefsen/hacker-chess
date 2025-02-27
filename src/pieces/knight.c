#include <stddef.h>
#include <stdio.h>

#include "knight.h"

#include "../util.h"
#include "../validate.h"

bool _validate_knight_move_diff(Move m) {
	Coordinate diff = move_diff(m);
	Coordinate ideal[8] = {
		{ .x = 1,	 .y = 2},
		{ .x = 1, .y = -2},
		{.x = -1,  .y = 2},
		{.x = -1, .y = -2},
		{ .x = 2,	 .y = 1},
		{ .x = 2, .y = -1},
		{.x = -2,  .y = 1},
		{.x = -2, .y = -1},
	};

	for (size_t i = 0; i < 8; ++i) {
		if (diff.x == ideal[i].x && diff.y == ideal[i].y) {
			return true;
		}
	}
	return false;
}

bool validate_knight_move(BOARD_T(b), Move m, enum Color c, bool should_print) {
	if (!validate_basic(m)) {
		return false;
	}
	if (!_validate_knight_move_diff(m)) {
		if (should_print) {
			fputs("Knight move does not fit pattern.", stderr);
		}
		return false;
	}

	if (validate_takes(b, m.to, c)) {
		if (should_print) {
			puts("Knight takes!");
		}
		return true;
	}
	return b[m.to.y][m.to.x].kind == None;
}
