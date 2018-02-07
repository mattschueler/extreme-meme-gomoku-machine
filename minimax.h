#include "ai.h"
#include "assign_weights.c"

#define MAX_DEPTH 5
#define BRANCHING_FACTOR 3

int* dlminimax(struct Board* b, char curr_player, int curr_depth);
void getBestMoves(struct Board* b, int **moves, int curr_player);
void abEval(struct Board* b, int** moves, char curr_player);
Board* haveChild(struct Board* parent);
