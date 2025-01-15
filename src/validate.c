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
