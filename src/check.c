#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "check.h"

#include "types.h"
#include "validate.h"
#include "macro.h"
#include "move.h"
#include "pieces/king.h"


bool detect_check(Board *b, Coordinate king_pos) {
	DEBUG_CHECK("DEBUG[detect_check]: START\n");
	DEBUG_CHECK("\tking_pos: x:%d, y:%d;\n", king_pos.x, king_pos.y);

	enum Color king_color = b->pieces[king_pos.y][king_pos.x].color;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece p = b->pieces[i][j];
			if (p.kind == None || p.color == king_color) {
				continue;
			}
			DEBUG_CHECK("\tkind: %c; x:%d, y:%d;\n", p.kind, j, i);

			Move m = {
				.from = {.x = j, .y = i},
				.to = king_pos
			};
			if (validate_move(b, m, p.color)) {
				DEBUG_CHECK("\tCHECK\n");
				return true;
			}
		}
	}
	DEBUG_CHECK("DEBUG[detect_check]: END\n");
	return false;
}

// Returns true if there is a checkmate
bool detect_checkmate(Board *b, Coordinate king_pos) {

	enum Color king_color = b->pieces[king_pos.y][king_pos.x].color;
	DEBUG_CHECK("DEBUG[detect_checkmate]: START\n");
	DEBUG_CHECK("\tking_pos=x:%dy:%d\n", king_pos.x, king_pos.y);

	Board lb;
	MovePtr moves = get_all_valid_moves(b, king_color);

	for (size_t i = 0; i < moves.length; i++) {
		memcpy(lb.pieces, b->pieces, sizeof b->pieces);
		Move m = moves.moves[i];
		DEBUG_CHECK("\to=x:%dy:%d\n", m.to.x, m.to.y);

		Coordinate kp = king_pos;
		if (lb.pieces[m.from.y][m.from.x].kind == King) {
			kp = m.to;
		}

		move_piece(&lb, m);
		if (!detect_check(&lb, kp)) {
			DEBUG_CHECK("\t\tNot checkmate!\n");
			return false;
		}
	}

	free(moves.moves);
	DEBUG_CHECK("DEBUG[detect_checkmate]: END\n");
	return true;
}


// Checks all pieces for all possible moves and returns a struct containing a Heap allocated
// Move pointer and length integer.
MovePtr get_all_valid_moves(Board *b, enum Color c) {
	DEBUG_CHECK("DEBUG[get_all_valid_moves]: START\n");

	int bishop_count = 0;
	int rook_count = 0;
	int queen_count = 0;
	int knight_count = 0;
	int pawn_count = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece p = b->pieces[i][j];
			if (p.kind == None || p.color != c) {
				continue;
			}
			if (p.kind == Bishop) {
				bishop_count++;
			} else if (p.kind == Rook) {
				rook_count++;
			} else if (p.kind == Queen) {
				queen_count++;
			} else if (p.kind == Knight) {
				knight_count++;
			} else if (p.kind == Pawn) {
				pawn_count++;
			}
		}
	}

	// Bishops can visit a max of 13 different squares on the same move,
	// multiply amount of bishops by 13.
	//
	// Rooks can visit a max of 14 different squares on the same move,
	// multiply amount of rooks by 14.
	//
	// Knights can visit a max of 8 different squares on the same move,
	// multiply amount of knights by 8.
	//
	// Queens can visit a max of 27 different squares on the same move,
	// multiply amount of queens by 27.
	//
	// Pawns can visit a max of 2 different squares on the same move,
	// multiply amount of pawns by 2.
	//
	// Kings can visit a max of 8 different squares on the same move,
	// add 8 to total (there is always a king, and only one).
	const size_t max_moves =  
		(13 * bishop_count)
		+ (14 * rook_count)
		+ (8 * knight_count)
		+ (queen_count * 27)
		+ (pawn_count * 2)
		+ 8;
	errno = 0;
	Move *moves = malloc(max_moves * sizeof *moves);
	if (errno != 0) {
		DEBUG_CHECK("ERROR[get_all_valid_moves]: malloc failed with error %d", errno);
		exit(EXIT_FAILURE);
	}

	size_t move_counter = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece p = b->pieces[i][j];
			if (p.kind == None || p.color != c) {
				continue;
			} 
			DEBUG_CHECK("\tkind: %c; x:%d, y:%d;\n", p.kind, j, i);
			Coordinate from = {.x = j, .y = i};

			if (p.kind == King) {
				// OPTIMIZATION. function would work without this if block
				// King specific move set
				const Move king_moves[8] = {
					{from, .to = {from.x+1, from.y}},
					{from, .to = {from.x-1, from.y}},
					{from, .to = {from.x,   from.y+1}},
					{from, .to = {from.x,   from.y-1}},
					{from, .to = {from.x+1, from.y+1}},
					{from, .to = {from.x+1, from.y-1}},
					{from, .to = {from.x-1, from.y+1}},
					{from, .to = {from.x-1, from.y-1}},
				};
				for (int idx = 0; idx < 8; ++idx) {
					Move m = king_moves[idx];
					if (validate_move(b, m, c)) {
						moves[move_counter++] = m;
						DEBUG_CHECK("\t\tValid move: from x:%d y:%d; to x:%d y:%d\n", m.from.x, m.from.y, m.to.x, m.to.y);
					}
				}
				continue;
			} else if (p.kind == Knight) {
				// OPTIMIZATION. function would work without this if block
				// Knight specific move set
				const Move knight_moves[8] = {
					{from, .to = {.x = from.x +  1, .y = from.y +  2}},
					{from, .to = {.x = from.x +  1, .y = from.y + -2}},
					{from, .to = {.x = from.x + -1, .y = from.y +  2}},
					{from, .to = {.x = from.x + -1, .y = from.y + -2}},
					{from, .to = {.x = from.x +  2, .y = from.y +  1}},
					{from, .to = {.x = from.x +  2, .y = from.y + -1}},
					{from, .to = {.x = from.x + -2, .y = from.y +  1}},
					{from, .to = {.x = from.x + -2, .y = from.y + -1}},
				};
				for (int idx = 0; idx < 8; ++idx) {
					Move m = knight_moves[idx];
					if (validate_move(b, m, c)) {
						moves[move_counter++] = m;
						DEBUG_CHECK("\t\tValid move: from x:%d y:%d; to x:%d y:%d\n", m.from.x, m.from.y, m.to.x, m.to.y);
					}
				}
				continue;
			} else if (p.kind == Pawn) {
				// OPTIMIZATION. function would work without this if block
				// Pawn specific move set
				// multiply y by the color to invert if black
				Move pawn_moves[2] = {
					{from, .to = {.x = from.x, .y = from.y + (1 * c)}},
					{from, .to = {.x = from.x, .y = from.y + (1 * c)}},
				};
				for (int idx = 0; idx < 2; ++idx) {
					Move m = pawn_moves[idx];
					if (validate_move(b, m, c)) {
						moves[move_counter++] = m;
						DEBUG_CHECK("\t\tValid move: from x:%d y:%d; to x:%d y:%d\n", m.from.x, m.from.y, m.to.x, m.to.y);
					}
				}
				continue;
			}

			// now iterate the whole board again
			for (int k = 0; k < 8; ++k) {
				for (int l = 0; l < 8; ++l) {
					Move m = {.from = from, .to = {.x = l, .y = k}};
					if (validate_move(b, m, c)) {
						moves[move_counter++] = m;
						DEBUG_CHECK("\t\tValid move: from x:%d y:%d; to x:%d y:%d\n", m.from.x, m.from.y, m.to.x, m.to.y);
					}
				}
			}
		}
	}
	MovePtr out = {.moves = moves, .length = move_counter};
	return out;
	DEBUG_CHECK("DEBUG[get_all_valid_moves]: END\n");
}
