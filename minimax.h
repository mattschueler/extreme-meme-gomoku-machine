#include "ai.h"
#include "assign_weights.c"

#define MAX_DEPTH 5

int* dlminimax(struct Board b, char curr_player, int curr_depth);
void getBestMoves(struct Board b, int **moves);
int abEval(struct Board b, int** moves, char curr_player);
Board haveChild(struct Board parent);
