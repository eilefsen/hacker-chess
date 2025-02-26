#include "pawn.h"
#include "../validate.h"
#include <stdio.h>

PawnMove validate_pawn_move(Board *b, Move m, enum Color c, bool should_print) {
	if (!validate_basic(m)) {
		PawnMove out = {false, false, -1};
		return out;
	}
	int one_forward = m.from.y + (1 * c);
	int two_forward = m.from.y + (2 * c);
	bool diagonal = m.to.y == one_forward &&
					(m.from.x == m.to.x + 1 || m.from.x == m.to.x - 1);
	if (diagonal) {
		// pawn takes, valid move
		int enpassant = -1;
		if(m.to.y == 2) {
			const Piece lp = b->pieces[3][m.to.x];
			if (lp.en_passantable) {
				enpassant = 3;
			}
		} else if(m.to.y == 5) {
			const Piece lp = b->pieces[4][m.to.x];
			if (lp.en_passantable) {
				enpassant = 4;
			}
		}
		if (enpassant != -1) {
			if (should_print) {
				puts("Pawn takes!");
			}
			PawnMove out = {true, false, enpassant};
			return out;
		}


		if (validate_takes(b, m.to, c)) {
			if (should_print) {
				puts("Pawn takes!");
			}
			PawnMove out = {true, false, -1};
			return out;
		}
	}

	Piece from_p = b->pieces[m.from.y][m.from.x];
	Piece to_p = b->pieces[m.to.y][m.to.x];

	bool same_rank = m.from.x == m.to.x;
	if (same_rank) {
		bool first_move = !from_p.has_moved;
		if (m.to.y == two_forward && first_move && to_p.kind == None) {
			// pawn moves two squares forward
			if(b->pieces[one_forward][m.to.x].kind == None) {
				PawnMove out = {true, true, -1};
				return out;
			}
		} else if (m.to.y == one_forward) {
			// pawn moves one square forward
			if(to_p.kind == None) {
				PawnMove out = {true, false, -1};
				return out;
			}
		}
	}

	PawnMove out = {false, false, -1};
	return out;
}
