#include "ai.h"
#include "assign_weights.c"

#define MAX_DEPTH 5

int* dlminimax(struct Board b, char curr_player, int curr_depth);
void getBestMoves(struct Board b, int **moves);
struct Board haveChild(struct Board parent);
