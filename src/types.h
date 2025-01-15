#pragma once
#include <stdbool.h>

enum Color {
	Black = -1,
	NoneC = 0,
	White = 1,
};

enum PieceKind {
	King = 'K',
	Queen = 'Q',
	Bishop = 'B',
	Knight = 'N',
	Rook = 'R',
	Pawn = 'P',
	None = ' '
};

typedef struct Piece {
	enum PieceKind kind;
	enum Color color;
	bool has_moved;
} Piece;

typedef struct Board {
	struct Piece pieces[8][8];
} Board;

typedef struct Coordinate {
	int x;
	int y;
} Coordinate;

typedef struct Move {
	Coordinate from;
	Coordinate to;
} Move;
