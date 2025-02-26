#include <stdio.h>
#include <string.h>

#include "util.h"

void get_piece_name(char s[static 8], enum PieceKind pk) {
	switch (pk) {
	case None:
		strncpy(s, "None", 7);
		break;
	case King:
		strncpy(s, "King", 7);
		break;
	case Queen:
		strncpy(s, "Queen", 7);
		break;
	case Rook:
		strncpy(s, "Rook", 7);
		break;
	case Bishop:
		strncpy(s, "Bishop", 7);
		break;
	case Knight:
		strncpy(s, "Knight", 7);
		break;
	case Pawn:
		strncpy(s, "Pawn", 7);
		break;
	}
}

void print_move(Move m) {
	printf("From: ");
	print_coordinate(m.from);
	printf(", To: ");
	print_coordinate(m.to);
}
void print_coordinate(Coordinate pos) {
	printf("x = %d y= %d", pos.x, pos.y);
}

Coordinate move_diff(Move m) {
	Coordinate diff = {.x = m.from.x - m.to.x, .y = m.from.y - m.to.y};
	return diff;
}
