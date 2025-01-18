#include "shared.h"
#include "../types.h"
#include "../util.h"
#include "../validate.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool validate_diagonal_move(Board *b, Move m, enum Color c, Coordinate diff, bool takes) {
	if (abs(diff.x) != abs(diff.y)) {
		char s[7];
		get_piece_name(s, b->pieces[m.from.y][m.from.x].kind);
		printf("Invalid move pattern for %s", s);
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

	return b->pieces[m.to.y][m.to.x].kind == None;
}

bool validate_straight_move(Board *b, Move m, enum Color c, Coordinate diff, bool takes) {
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
		char s[7];
		get_piece_name(s, b->pieces[m.from.y][m.from.x].kind);
		printf("Invalid move pattern for %s", s);
		return false;
	}

	return b->pieces[m.to.y][m.to.x].kind == None;
}
