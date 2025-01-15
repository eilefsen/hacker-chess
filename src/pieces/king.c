#include <stdio.h>

#include "king.h"

#include "../util.h"
#include "../validate.h"

KingMove validate_king_move(Board *b, Move m, enum Color c) {
	if (!validate_basic(m)) {
		KingMove out = {false, None_Castle};
		return out;
	}

	Piece king_p = b->pieces[m.from.y][m.from.x];
	if (m.to.y == 0 && !king_p.has_moved) {
		Piece r_rook_p = b->pieces[m.from.y][7];
		bool r_empty = b->pieces[m.from.y][5].kind == None &&
					   b->pieces[m.from.y][6].kind == None;
		if (m.to.x == 6 && r_empty && r_rook_p.kind == Rook && !r_rook_p.has_moved) {
			puts("King castles short!");
			KingMove out = {true, Short_Castle};
			return out;
		}

		Piece l_rook_p = b->pieces[m.from.y][0];
		bool l_empty = b->pieces[m.from.y][1].kind == None &&
					   b->pieces[m.from.y][2].kind == None &&
					   b->pieces[m.from.y][3].kind == None;
		if (m.to.x == 2 && l_empty && l_rook_p.kind == Rook && !l_rook_p.has_moved) {
			puts("King castles long!");
			KingMove out = {true, Long_Castle};
			return out;
		}
	}

	Coordinate diff = move_diff(m);
	if (diff.x > 1 || diff.x < -1 || diff.y > 1 || diff.y < -1) {
		fputs("King move has to be one square away.", stderr);
		KingMove out = {false, None_Castle};
		return out;
	}
	if (validate_takes(b, m.to, c) || b->pieces[m.to.y][m.to.x].kind == None) {
		puts("King takes!");
		KingMove out = {true, None_Castle};
		return out;
	}

	// default false
	KingMove out = {false, None_Castle};
	return out;
}
