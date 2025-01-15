#include <stddef.h>
#include <stdio.h>

#include "knight.h"

#include "../util.h"
#include "../validate.h"

bool _validate_knight_move_diff(Move m) {
	Coordinate diff = move_diff(m);
	Coordinate ideal[8] = {
		{.x = 1, .y = 2},
		{.x = 1, .y = -2},
		{.x = -1, .y = 2},
		{.x = -1, .y = -2},
		{.x = 2, .y = 1},
		{.x = 2, .y = -1},
		{.x = -2, .y = 1},
		{.x = -2, .y = -1},
	};

	for (size_t i = 0; i < 8; ++i) {
		if (diff.x == ideal[i].x && diff.y == ideal[i].y) {
			return true;
		}
	}
	return false;
}

bool validate_knight_move(Board *b, Move m, enum Color c) {
	if (!validate_basic(m)) {
		return false;
	}
	if (!_validate_knight_move_diff(m)) {
		fputs("Knight move does not fit pattern.", stderr);
		return false;
	}

	if (validate_takes(b, m.to, c) || b->pieces[m.to.y][m.to.x].kind == None) {
		puts("Knight takes!");
		return true;
	}
	return false;
}
