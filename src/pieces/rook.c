#include "rook.h"
#include "../util.h"
#include "../validate.h"
#include "shared.h"
#include <stdio.h>

bool validate_rook_move(BOARD_T(b), Move m, enum Color c, bool should_print) {
	if (!validate_basic(m)) {
		return false;
	}

	Coordinate diff = move_diff(m);
	bool takes = validate_takes(b, m.to, c);

	bool strt = (diff.x && !diff.y) || (diff.y && !diff.x);
	if (!strt) {
		if (should_print) {
			fputs("Rook must move along the same file or rank as it started\n", stderr);
		}
		return false;
	}

	if (!validate_straight_move(b, m, diff, takes)) {
		return false;
	}

	if (takes) {
		if (should_print) {
			puts("Rook takes!");
		}
		return true;
	}

	return b[m.to.y][m.to.x].kind == None;
}
