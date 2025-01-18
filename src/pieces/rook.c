#include "rook.h"
#include "../util.h"
#include "../validate.h"
#include <stdio.h>
#include <stdlib.h>

bool validate_rook_move(Board *b, Move m, enum Color c) {
	if (!validate_basic(m)) {
		return false;
	}

	Coordinate diff = move_diff(m);
	bool takes = validate_takes(b, m.to, c);

	if (!diff.x) {
		for (int i = 0; i < abs(diff.y); ++i) {
			int y = diff.y > 0 ? m.to.y + i : m.to.y - i;

			Piece p = b->pieces[y][m.from.x];

			if (p.kind != None && !(takes && i == abs(diff.y) - 1)) {
				puts("Obstructed");
				return false;
			}
		}
	} else if (!diff.y) {
		for (int i = 0; i < abs(diff.x); ++i) {
			int x = diff.x > 0 ? m.to.x + i : m.to.x - i;

			Piece p = b->pieces[m.from.y][x];

			if (p.kind != None && !(takes && i == abs(diff.x) - 1)) {
				puts("Obstructed");
				return false;
			}
		}
	} else {
		puts("Rook must move along the same file or rank as it started");
		return false;
	}
	if (takes) {
		puts("Rook takes!");
		return true;
	}

	return b->pieces[m.to.y][m.to.x].kind == None;
}
