#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "check.h"
#include "move.h"
#include "types.h"
#include "macro.h"

#define INPUT_MAX 9

typedef struct ParseMoveResult {
	Move move;
	bool error;
} ParseMoveResult;

struct CharResult {
	char val;
	bool error;
} convert_cmd_letter(char c) {
	if (c >= 65 && c <= 72) {
		struct CharResult out = {c - 65, false};
		return out;
	}
	if (c >= 97 && c <= 104) {
		struct CharResult out = {c - 97, false};
		return out;
	}
	struct CharResult out = {c, true};
	return out;
}

// A move command is of the form "[A-H><1-8>[A-H][1-8]", where the first group is the
// piece to move, and the second group is the destination
ParseMoveResult parse_move(char s[INPUT_MAX]) {
	int y1 = 0, y2 = 0, x1 = 0, x2 = 0;
	char xc1, xc2;
	// TODO: Implement a more robust parse verification
	if (sscanf(s, "%1c%1d%1c%1d", &xc1, &y1, &xc2, &y2) != 4) {
		ParseMoveResult out = {
			.move = {{0, 0}, {0, 0}},
			.error = true
		};
		return out;
	}

	struct CharResult xc1_conv = convert_cmd_letter(xc1);
	struct CharResult xc2_conv = convert_cmd_letter(xc2);

	if (xc1_conv.error || xc2_conv.error) {
		ParseMoveResult out = {
			.move = {{0, 0}, {0, 0}},
			.error = true
		};
		return out;
	}
	x1 = xc1_conv.val, x2 = xc2_conv.val;

	ParseMoveResult out = {
		{.from = {.x = x1, .y = y1 - 1}, .to = {.x = x2, .y = y2 - 1}},
		false
	};
	return out;
}

void draw(Board b) {
	puts(",---,---,---,---,---,---,---,---,");
	for (size_t i = 8; i-- > 0;) { // this type of for statement avoids underflow
		for (size_t j = 0; j < 8; ++j) {
			if (b.pieces[i][j].kind == None) {
				printf("|   ");
			} else {
				Piece p = b.pieces[i][j];
				printf("| %c ", p.color == White ? p.kind : p.kind ^ 32);
			}
		}
		printf("| %zu\n", i + 1);
		if (i != 0) {
			puts("|---|---|---|---|---|---|---|---|");
		}
	}
	puts("'---'---'---'---'---'---'---'---'");
	puts("  A   B   C   D   E   F   G   H");
}

int main(void) {
	Move moves[100] = {0};

	// clang-format off
	Board board = {.pieces = {
		{{Rook, White, false, false}, {Knight, White, false, false}, {Bishop, White, false, false}, {Queen, White, false, false}, {King, White, false, false}, {Bishop, White, false, false}, {Knight, White, false}, {Rook, White, false, false}},
		{{Pawn, White, false, false}, {Pawn,   White, false, false}, {Pawn,   White, false, false}, {Pawn,  White, false, false}, {Pawn, White, false, false}, {Pawn,   White, false, false}, {Pawn,   White, false}, {Pawn, White, false, false}},
		{{None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false, false}, {None,  NoneC, false, false}, {None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false}, {None, NoneC, false, false}},
		{{None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false, false}, {None,  NoneC, false, false}, {None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false}, {None, NoneC, false, false}},
		{{None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false, false}, {None,  NoneC, false, false}, {None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false}, {None, NoneC, false, false}},
		{{None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false, false}, {None,  NoneC, false, false}, {None, NoneC, false, false}, {None,   NoneC, false, false}, {None,   NoneC, false}, {None, NoneC, false, false}},
		{{Pawn, Black, false, false}, {Pawn,   Black, false, false}, {Pawn,   Black, false, false}, {Pawn,  Black, false, false}, {Pawn, Black, false, false}, {Pawn,   Black, false, false}, {Pawn,   Black, false}, {Pawn, Black, false, false}},
		{{Rook, Black, false, false}, {Knight, Black, false, false}, {Bishop, Black, false, false}, {Queen, Black, false, false}, {King, Black, false, false}, {Bishop, Black, false, false}, {Knight, Black, false}, {Rook, Black, false, false}},
	}};
	// clang-format on

	int i = 0;
	enum Color c = White;
	draw(board);

	bool check = false;
	Coordinate black_king_pos = {4,7};
	Coordinate white_king_pos = {4,0};

	while (true) {
		c = i % 2 == 0 ? White : Black; // white to play if `i` is even
		printf("%s to play:\n", c == White ? "White" : "Black");

		for (int j = 0; j < 8; ++j) {
			for (int k = 0; k < 8; ++k) {
				if (board.pieces[j][k].color == c) {
					board.pieces[j][k].en_passantable = false; // reset en_passantable
				}
			}
		}

		// get input
		char in[INPUT_MAX];
		fgets(in, INPUT_MAX, stdin);
		in[strcspn(in, "\n")] = 0; // set final \n in string to null char
		if (strnlen(in, INPUT_MAX) == 0) {
			continue;
		}

		ParseMoveResult move_maybe = parse_move(in);
		if (move_maybe.error) {
			fprintf(stderr, "Invalid Command: %s\n", in);
			continue; // skip rest, dont increment i
		}
		Move m = move_maybe.move;
		Piece p = board.pieces[m.from.y][m.from.x];
		if (!make_move(&board, m, c)) {
			fprintf(stderr, "Invalid move: %c %s\n", p.kind, in);
			continue;
		};
		if (p.kind == 'K') {
			if (c == White) {
				white_king_pos = m.to;
			} else {
				black_king_pos = m.to;
			}
		}
		moves[i] = m;
		draw(board);
		check = detect_check(&board, c == White ? black_king_pos : white_king_pos);
		if (check) {
			if (detect_checkmate(&board, c == White ? black_king_pos : white_king_pos)) {
				printf("Checkmate! %s wins!\n", c == White ? "white" : "black");
				return 0;
			}
			puts("Check!");
		}
		++i;
	}
	printf("%d", moves[0].from.x);

	return 0;
}
