#include "pawn.h"
#include "../validate.h"
#include <stdio.h>

bool validate_pawn_move(Board *b, Move m, enum Color c) {
	if (!validate_basic(m)) {
		return false;
	}
	int one_forward = m.from.y + (1 * c);
	int two_forward = m.from.y + (2 * c);
	bool diagonal =
		m.to.y == one_forward && (m.from.x == m.to.x + 1 || m.from.x == m.to.x - 1);
	if (diagonal && validate_takes(b, m.to, c)) {
		// pawn takes, valid move
		puts("Pawn takes!");
		return true;
	}

	Piece from_p = b->pieces[m.from.y][m.from.x];
	Piece to_p = b->pieces[m.to.y][m.to.x];

	bool same_file = m.from.x == m.to.x;
	if (same_file) {
		bool first_move = !from_p.has_moved;
		if (m.to.y == two_forward && first_move && to_p.kind == None) {
			// pawn moves two squares forward
			return b->pieces[one_forward][m.to.x].kind == None;
		} else if (m.to.y == one_forward) {
			// pawn moves one square forward
			return to_p.kind == None;
		}
	}

	return false;
}
