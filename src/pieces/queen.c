#include <stdio.h>
#include <stdlib.h>

#include "../util.h"
#include "../validate.h"
#include "queen.h"
#include "shared.h"

bool validate_queen_move(BOARD_T(b), Move m, enum Color c, bool should_print) {
	if (!validate_basic(m)) {
		return false;
	}

	Coordinate diff = move_diff(m);
	bool takes = validate_takes(b, m.to, c);

	bool diag = abs(diff.x) == abs(diff.y);
	bool strt = (diff.x && !diff.y) || (diff.y && !diff.x);

	if (diag) {
		if (!validate_diagonal_move(b, m, diff, takes)) {
			return false;
		}
	} else if (strt) {
		if (!validate_straight_move(b, m, diff, takes)) {
			return false;
		}
	} else {
		if (should_print) {
			fputs("Queen has to move either diagonally or straight", stderr);
		}
		return false;
	}

	if (takes) {
		if (should_print) {
			puts("Queen takes!");
		}
		return true;
	}

	return b[m.to.y][m.to.x].kind == None;
}
