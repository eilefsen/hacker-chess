#include "../macro.h"
#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

bool validate_straight_move(BOARD_T(b), Move m, Coordinate diff, bool takes) {
	DEBUG("DEBUG[validate_straight_move]: start\n");
	DEBUG("\tkind: %c, x:%d y:%d\n", 
		b[m.from.y][m.from.x].kind, m.from.x, m.from.y
	);
	if (!diff.x) {
		for (int i = 0; i < abs(diff.y); ++i) {
			int y = diff.y > 0 ? m.to.y + i : m.to.y - i;

			Piece p = b[y][m.from.x];

			if (p.kind != None && !(takes && i == 0)) {
				DEBUG("\t\tObstructed! x:%d y:%d\n", m.from.x, y);
				return false;
			}
		}
	} else if (!diff.y) {
		for (int i = 0; i < abs(diff.x); ++i) {
			int x = diff.x > 0 ? m.to.x + i : m.to.x - i;

			Piece p = b[m.from.y][x];

			if (p.kind != None && !(takes && i == abs(diff.x) - 1)) {
				DEBUG("\t\tObstructed! x:%d y:%d\n", x, m.from.y);
				return false;
			}
		}
	}
	DEBUG("DEBUG[validate_straight_move]: END\n");
	return true;
}

bool validate_diagonal_move(BOARD_T(b), Move m, Coordinate diff, bool takes) {
	DEBUG("DEBUG[validate_diagonal_move]: START\n");
	DEBUG("\tkind: %c, x:%d y:%d\n", 
		b[m.from.y][m.from.x].kind, m.from.x, m.from.y
	);
	for (int i = 0; i < abs(diff.y); ++i) {
		int y = diff.y > 0 ? m.to.y + i : m.to.y - i;
		int x = diff.x > 0 ? m.to.x + i : m.to.x - i;

		Piece p = b[y][x];

		if (p.kind != None && !(takes && i == 0)) {
			DEBUG("\t\tObstructed! kind:%c x:%d y:%d\n", p.kind, x, y);
			return false;
		}
	}
	DEBUG("DEBUG[validate_diagonal_move]: END\n");
	return true;
}
