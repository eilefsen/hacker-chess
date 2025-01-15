#include <stdio.h>

#include "util.h"

void print_move(Move m) {
	printf("From: ");
	print_coordinate(m.from);
	printf(", To: ");
	print_coordinate(m.to);
}
void print_coordinate(Coordinate pos) { printf("x = %d y= %d", pos.x, pos.y); }

Coordinate move_diff(Move m) {
	Coordinate diff = {.x = m.from.x - m.to.x, .y = m.from.y - m.to.y};
	return diff;
}
