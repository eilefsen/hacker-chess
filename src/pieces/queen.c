
#include "queen.h"
#include "../util.h"
#include "../validate.h"
#include <stdio.h>
#include <stdlib.h>

bool validate_queen_move(Board *b, Move m, enum Color c) {
	if (!validate_basic(m)) {
		return false;
	}

	Coordinate diff = move_diff(m);
	bool takes = validate_takes(b, m.to, c);

	if (abs(diff.x) != abs(diff.y)) {
		puts("Bishop has to move diagonally");
		return false;
	}

	for (int i = 0; i < abs(diff.y); ++i) {
		int y = diff.y > 0 ? m.to.y + i : m.to.y - i;
		int x = diff.x > 0 ? m.to.x + i : m.to.x - i;

		Piece p = b->pieces[y][x];

		if (p.kind != None && !(takes && i == abs(diff.y) - 1)) {
			puts("Obstructed");
			return false;
		}
	}
	if (takes) {
		puts("Bishop takes!");
		return true;
	}

	return b->pieces[m.to.y][m.to.x].kind == None;
}
