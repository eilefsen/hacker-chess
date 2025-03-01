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


bool detect_check(BOARD_T(b), Coordinate king_pos, enum Color king_color) {
	DEBUG("DEBUG[detect_check]: START\n");
	DEBUG("pos: x:%d, y:%d;\n", king_pos.x, king_pos.y);
	Piece king_piece = b[king_pos.y][king_pos.x] ;

	if(king_piece.kind != King) {
		DEBUG("\tpiece being checked is not a king!: kind: %c\n", king_piece.kind);
		DEBUG("DEBUG[detect_check]: END\n");
		return false;
	}

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece p = b[i][j];
			if (p.kind == None || p.color == king_color) {
				continue;
			}
			DEBUG_CHECK("\tkind: %c; x:%d, y:%d;\n", p.kind, j, i);

			Move m = {
				.from = {.x = j, .y = i},
				.to = king_pos
			};
			if (validate_move(b, m, p.color)) {
				DEBUG("\tCHECK\n");
				DEBUG("DEBUG[detect_check]: END\n");
				return true;
			}
		}
	}
	DEBUG("DEBUG[detect_check]: END\n");
	return false;
}

// Returns true if there is a checkmate
bool detect_checkmate(BOARD_T(b), Coordinate king_pos) {

	enum Color king_color = b[king_pos.y][king_pos.x].color;
	DEBUG_CHECK("DEBUG[detect_checkmate]: START\n");
	DEBUG_CHECK("\tking_pos=x:%dy:%d\n", king_pos.x, king_pos.y);

	BOARD_T(lb);
	MovePtr moves = get_all_valid_moves(b, king_color);

	for (size_t i = 0; i < moves.length; i++) {
		memcpy(lb, b, sizeof lb);
		Move m = moves.moves[i];
		DEBUG_CHECK("\to=x:%dy:%d\n", m.to.x, m.to.y);

		Coordinate kp = king_pos;
		if (lb[m.from.y][m.from.x].kind == King) {
			kp = m.to;
		}

		move_piece(lb, m);
		if (!detect_check(lb, kp, king_color)) {
			DEBUG_CHECK("\t\tNot checkmate!\n");
			return false;
		}
	}

	free(moves.moves);
	DEBUG_CHECK("DEBUG[detect_checkmate]: END\n");
	return true;
}


typedef struct piece_count_t {
	int bishops;
	int rooks;
	int queens;
	int knights;
	int pawns;
} piece_count_t;
#define DEBUG_PIECE_COUNT(count) DEBUG_CHECK("\t\tbishops = %d\n", count.bishops); DEBUG_CHECK("\t\trooks   = %d\n", count.rooks); DEBUG_CHECK("\t\tknights = %d\n", count.knights); DEBUG_CHECK("\t\tqueens  = %d\n", count.queens); DEBUG_CHECK("\t\tpawns   = %d\n", count.pawns)

piece_count_t count_pieces(BOARD_T(b), enum Color c) {
	DEBUG("DEBUG[count_pieces]: START\n");
	piece_count_t count = {0};

	DEBUG_CHECK("\tBEFORE:\n");
	DEBUG_PIECE_COUNT(count);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece p = b[i][j];
			if (p.color != c) {
				continue;
			}
			if (p.kind == Bishop) {
				count.bishops++;
			} else if (p.kind == Rook) {
				count.rooks++;
			} else if (p.kind == Queen) {
				count.queens++;
			} else if (p.kind == Knight) {
				count.knights++;
			} else if (p.kind == Pawn) {
				count.pawns++;
			}
		}
	}
	DEBUG("\tAFTER:\n");
	DEBUG_PIECE_COUNT(count);
	DEBUG("DEBUG[count_pieces]: END\n");
	return count;
}


// Checks all pieces for all possible moves and returns a struct containing a Heap allocated
// Move pointer and length integer.
MovePtr get_all_valid_moves(BOARD_T(b), enum Color c) {
	DEBUG("DEBUG[get_all_valid_moves]: START\n");

	piece_count_t count = count_pieces(b, c);

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
	DEBUG_PIECE_COUNT(count);
	const size_t max_moves =  
		(13 * count.bishops)
		+ (14 * count.rooks)
		+ (8  * count.knights)
		+ (27 * count.queens)
		+ (2  * count.pawns)
		+ 8;
	DEBUG("\tmax_moves = %ld", max_moves);
	errno = 0;
	Move *moves = malloc(max_moves * sizeof *moves);
	if (errno != 0) {
		fprintf(stderr, "ERROR[get_all_valid_moves]: malloc failed with error %d", errno);
		exit(EXIT_FAILURE);
	}

	size_t move_counter = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece p = b[i][j];
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
	DEBUG("DEBUG[get_all_valid_moves]: END\n");
}
