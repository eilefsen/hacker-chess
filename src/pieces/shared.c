#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

bool validate_straight_move(Board *b, Move m, Coordinate diff, bool takes) {
	if (!diff.x) {
		for (int i = 0; i < abs(diff.y); ++i) {
			int y = diff.y > 0 ? m.to.y + i : m.to.y - i;

			Piece p = b->pieces[y][m.from.x];

			if (p.kind != None && !(takes && i == 0)) {
				fputs("Obstructed\n", stderr);
				return false;
			}
		}
	} else if (!diff.y) {
		for (int i = 0; i < abs(diff.x); ++i) {
			int x = diff.x > 0 ? m.to.x + i : m.to.x - i;

			Piece p = b->pieces[m.from.y][x];

			if (p.kind != None && !(takes && i == abs(diff.x) - 1)) {
				fputs("Obstructed\n", stderr);
				return false;
			}
		}
	}
	return true;
}

bool validate_diagonal_move(Board *b, Move m, Coordinate diff, bool takes) {
	for (int i = 0; i < abs(diff.y); ++i) {
		int y = diff.y > 0 ? m.to.y + i : m.to.y - i;
		int x = diff.x > 0 ? m.to.x + i : m.to.x - i;

		Piece p = b->pieces[y][x];

		if (p.kind != None && !(takes && i == 0)) {
			fputs("Obstructed\n", stderr);
			return false;
		}
	}
	return true;
}
