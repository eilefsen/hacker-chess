#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define INPUT_MAX 9

enum Color {
	Black = -1,
	White = 1,
};

enum PieceKind {
	King = 'K',
	Queen = 'Q',
	Bishop = 'B',
	Knight = 'N',
	Rook = 'R',
	Pawn = 'P',
	None = '0'
};

typedef struct Coordinate {
	int x;
	int y;
} Coordinate;

typedef struct Move {
	Coordinate from;
	Coordinate to;
} Move;

// A move command is of the form "[A-H][1-8] [A-H][1-8]", where the first group is the
// piece to move, and the second group is the destination
struct MoveWithErr {
	Move move;
	char *error;
} parse_move(char s[INPUT_MAX]) {
	int y1 = 0, y2 = 0;
	char xc1, xc2;
	if (sscanf(s, "%1c%1d %1c%1d", &xc1, &y1, &xc2, &y2) != 4) {
		const int bufsize = 18 + INPUT_MAX;
		char err[bufsize];
		snprintf(err, bufsize, "Invalid Command: %s", s);

		struct MoveWithErr out = {{{0, 0}, {0, 0}}, NULL};
		return out;
	}

	// subtract 64 to convert A (ASCII 65) to 1, and so on
	int x1 = xc1 - 64;
	int x2 = xc2 - 64;

	Move move = {
		.from = {.x = x1, .y = y1},
		.to = {.x = x2, .y = y2},
	};
	struct MoveWithErr out = {move, NULL};
	return out;
}

typedef struct Board {
	enum PieceKind pieces[8][8];
} Board;

bool validate_in_bounds(Move m) {
	bool same_xy = m.to.x == m.from.x && m.to.y == m.from.y;
	bool inside_x = m.to.x <= 8 && m.to.x >= 0;
	bool inside_y = m.to.y <= 8 && m.to.y >= 0;

	return !same_xy && inside_x && inside_y;
}

bool validate_collide(Board *b, Coordinate pos) {
	switch (b->pieces[pos.y][pos.x]) {
	case None:
		return false;
	case Pawn:
		return true;
	case King:
		return true;
	case Queen:
		return true;
	case Bishop:
		return true;
	case Knight:
		return true;
	case Rook:
		return true;
	}
}

bool is_first_pawn_move(Coordinate pos, enum Color c) {
	if (c == White) {
		return pos.y == 2;
	}
	return pos.y == 7;
}

bool validate_pawn_move(Board *b, Move m, enum Color c) {
	if (!validate_in_bounds(m)) {
		return false;
	}

	bool first_move = is_first_pawn_move(m.from, c);
	bool leap = m.to.y == m.from.y + (2 * c);
	bool one_forward = m.from.y == m.to.y + (1 * c);
	if (!one_forward && !(first_move && leap)) {
		// invalid move, pawn has to move 1 square forward, or 2 (first move).
		return false;
	}

	bool diagonal = m.from.x == m.to.x + 1 || m.from.x == m.to.x - 1;
	if (validate_collide(b, m.to) && diagonal) {
		// pawn takes, valid move
		return true;
	}
	return m.from.x == m.to.x; // true if move is straight ahead
}

void move_piece(Board *b, Move m) {
	b->pieces[m.to.y - 1][m.to.x - 1] = b->pieces[m.from.y - 1][m.from.x - 1];
	b->pieces[m.from.y - 1][m.from.x - 1] = None;
}

// returns true if move sucessful, false if not.
bool move_pawn(Board *b, Move m, enum Color c) {
	if (validate_pawn_move(b, m, c)) {
		move_piece(b, m);
		return true;
	} else {
		return false;
	}
}

bool move_king(Board *b, Move m, enum Color c) {
	// Unimplemented
	return false;
};
bool move_queen(Board *b, Move m, enum Color c) {
	// Unimplemented
	return false;
};
bool move_bishop(Board *b, Move m, enum Color c) {
	// Unimplemented
	return false;
};
bool move_knight(Board *b, Move m, enum Color c) {
	// Unimplemented
	return false;
};
bool move_rook(Board *b, Move m, enum Color c) {
	// Unimplemented
	return false;
};

bool make_move(Board *b, Move m, enum Color c) {
	switch (b->pieces[m.from.y - 1][m.from.x - 1]) {
	case None:
		return false;
	case Pawn:
		return move_pawn(b, m, c);
	case King:
		return move_king(b, m, c);
	case Queen:
		return move_queen(b, m, c);
	case Bishop:
		return move_bishop(b, m, c);
	case Knight:
		return move_knight(b, m, c);
	case Rook:
		return move_rook(b, m, c);
	}
	return false;
}

int main(int argc, char *argv[]) {
	Move moves[100] = {0};

	// clang-format off
	Board board = {.pieces = {
		{Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook},
		{Pawn, Pawn,   Pawn,   Pawn,  Pawn, Pawn,   Pawn,   Pawn},
		{None, None,   None,   None,  None, None,   None,   None},
		{None, None,   None,   None,  None, None,   None,   None},
		{None, None,   None,   None,  None, None,   None,   None},
		{None, None,   None,   None,  None, None,   None,   None},
		{Pawn, Pawn,   Pawn,   Pawn,  Pawn, Pawn,   Pawn,   Pawn},
		{Rook, Knight, Bishop, Queen, King, Bishop, Knight, Rook},
	}};
	// clang-format on

	size_t i = 0;
	while (true) {
		enum Color c = i % 2 == 0 ? White : Black; // white to play if `i` is even
		char in[INPUT_MAX];
		fgets(in, INPUT_MAX, stdin);
		in[strcspn(in, "\n")] = 0; // set final \n in string to null char
		if (strnlen(in, INPUT_MAX) == 0) {
			continue;
		}

		struct MoveWithErr move_maybe = parse_move(in);
		if (move_maybe.error != NULL) {
			fputs(move_maybe.error, stderr);
			continue; // skip rest, dont increment i
		}
		moves[i] = move_maybe.move;

		if (!move_pawn(&board, moves[i], c)) {
			puts("Invalid move!");
		};
		++i;
	}
	printf("%d", moves[0].from.x);

	return 0;
}
