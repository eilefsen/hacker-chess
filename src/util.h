#include "types.h"

void get_piece_name(char buf[static 7], enum PieceKind pk);

// does not leave a newline
void print_move(Move m);
// does not leave a newline
void print_coordinate(Coordinate pos);

Coordinate move_diff(Move m);
