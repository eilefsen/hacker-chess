#include <stdio.h>

#include "move.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/rook.h"

void move_piece(Board *b, Move m) {
	// set to piece to the value of from piece
	b->pieces[m.to.y][m.to.x] = b->pieces[m.from.y][m.from.x];
	b->pieces[m.to.y][m.to.x].has_moved = true;

	// set from piece to none/empty
	Piece empty = {None, NoneC, false};
	b->pieces[m.from.y][m.from.x] = empty;
}

// returns true if move sucessful, false if not.
bool make_move(Board *b, Move m, enum Color c) {
	bool is_valid = false;

	Piece from_p = b->pieces[m.from.y][m.from.x];

	if (from_p.color != c) {
		fprintf(
			stderr,
			"Piece belongs to: %s\n",
			from_p.color == White ? "White" : "Black"
		);
		return false;
	}

	struct KingMove king_m = {false, false};

	switch (from_p.kind) {
	case None:
		fputs("Piece is empty", stderr);
		is_valid = false;
		break;
	case Pawn:
		is_valid = validate_pawn_move(b, m, c);
		break;
	case Knight:
		is_valid = validate_knight_move(b, m, c);
		break;
	case Rook:
		is_valid = validate_rook_move(b, m, c);
		break;
	case Bishop:
		is_valid = validate_bishop_move(b, m, c);
		break;
	case Queen:
		is_valid = validate_rook_move(b, m, c) || validate_bishop_move(b, m, c);
		break;
	case King:
		// TODO: check that this works in all cases
		king_m = validate_king_move(b, m, c);
		if (king_m.valid) {
			if (king_m.castle == Short_Castle) {
				// TODO: implement castling
				return true;
			} else if (king_m.castle == Long_Castle) {
				// TODO: implement castling
				return true;
			}
			is_valid = true;
		} else {
			return false;
		}
	}

	if (is_valid) {
		move_piece(b, m);
		return true;
	}
	return false;
}
