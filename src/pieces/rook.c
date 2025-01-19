#include "rook.h"
#include "../util.h"
#include "../validate.h"
#include "shared.h"
#include <stdio.h>

bool validate_rook_move(Board *b, Move m, enum Color c) {
	if (!validate_basic(m)) {
		return false;
	}

	Coordinate diff = move_diff(m);
	bool takes = validate_takes(b, m.to, c);

	bool strt = (diff.x && !diff.y) || (diff.y && !diff.x);
	if (!strt) {
		fputs("Rook must move along the same file or rank as it started\n", stderr);
		return false;
	}

	if (!validate_straight_move(b, m, diff, takes)) {
		return false;
	}

	if (takes) {
		puts("Rook takes!");
		return true;
	}

	return b->pieces[m.to.y][m.to.x].kind == None;
}
