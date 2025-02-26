#include <stdio.h>

#include "king.h"

#include "../util.h"
#include "../validate.h"

KingMove validate_king_move(Board *b, Move m, enum Color c, bool should_print) {
	if (!validate_basic(m)) {
		KingMove out = {false, None_Castle};
		return out;
	}

	Piece king_p = b->pieces[m.from.y][m.from.x];
	if (!king_p.has_moved && (m.to.y == 7 || m.to.y == 0)) {
		Piece r_rook_p = b->pieces[m.from.y][7];
		bool r_empty = b->pieces[m.from.y][5].kind == None &&
					   b->pieces[m.from.y][6].kind == None;
		if (m.to.x == 6 && r_empty && r_rook_p.kind == Rook && !r_rook_p.has_moved) {
			KingMove out = {true, Short_Castle};
			return out;
		}

		Piece l_rook_p = b->pieces[m.from.y][0];
		bool l_empty = b->pieces[m.from.y][1].kind == None &&
					   b->pieces[m.from.y][2].kind == None &&
					   b->pieces[m.from.y][3].kind == None;
		if (m.to.x == 2 && l_empty && l_rook_p.kind == Rook && !l_rook_p.has_moved) {
			KingMove out = {true, Long_Castle};
			return out;
		}
	}

	Coordinate diff = move_diff(m);
	if (diff.x > 1 || diff.x < -1 || diff.y > 1 || diff.y < -1) {
		if (should_print) {
			fputs("King move has to be one square away.\n", stderr);
		}
		KingMove out = {false, None_Castle};
		return out;
	}
	if (validate_takes(b, m.to, c) || b->pieces[m.to.y][m.to.x].kind == None) {
		if (should_print) {
			puts("King takes!");
		}
		KingMove out = {true, None_Castle};
		return out;
	}

	// default false
	KingMove out = {false, None_Castle};
	return out;
}
