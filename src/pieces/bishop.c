#include "bishop.h"
#include "../util.h"
#include "../validate.h"
#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

bool validate_bishop_move(Board *b, Move m, enum Color c, bool should_print) {
	if (!validate_basic(m)) {
		return false;
	}

	Coordinate diff = move_diff(m);
	bool takes = validate_takes(b, m.to, c);

	if (abs(diff.x) != abs(diff.y)) {
		if (should_print) {
			puts("Bishop has to move diagonally");
		}
		return false;
	}

	if (!validate_diagonal_move(b, m, diff, takes)) {
		return false;
	}

	if (takes) {
		if (should_print) {
			puts("Bishop takes!");
		}
		return true;
	}

	return b->pieces[m.to.y][m.to.x].kind == None;
}
