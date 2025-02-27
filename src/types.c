#include "types.h"

#include <stdbool.h>

Piece PieceNew(enum PieceKind kind, enum Color c) {
	Piece out = {kind, c, false, false};
	return out;
}
Piece NoneNew() {
	Piece out = {None, NoneC, false, false};
	return out;
}
