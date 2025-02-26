#include <stddef.h>
#include <stdio.h>

#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/queen.h"
#include "pieces/rook.h"

#include "types.h"
#include "validate.h"

bool validate_in_bounds(Coordinate pos) {
	bool inside_x = pos.x < 8 && pos.x >= 0;
	bool inside_y = pos.y < 8 && pos.y >= 0;
	return inside_x && inside_y;
}

bool validate_basic(Move m) {
	bool same_xy = m.to.x == m.from.x && m.to.y == m.from.y;
	return !same_xy && validate_in_bounds(m.to);
}

bool validate_takes(Board *b, Coordinate pos, enum Color c) {
	Piece p = b->pieces[pos.y][pos.x];
	return p.kind != None && p.color != c;
}

bool validate_move(Board *b, Move m, enum Color c) {
	struct KingMove king_m = {false, false};

	switch (b->pieces[m.from.y][m.from.x].kind) {
	case None:
		return false;
	case Pawn:
		return validate_pawn_move(b, m, c, false);
	case Knight:
		return validate_knight_move(b, m, c, false);
	case Rook:
		return validate_rook_move(b, m, c, false);
	case Bishop:
		return validate_bishop_move(b, m, c, false);
	case Queen:
		return validate_queen_move(b, m, c, false);
	case King:
		// TODO: check that this works in all cases
		king_m = validate_king_move(b, m, c, false);
		if (king_m.valid) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}
