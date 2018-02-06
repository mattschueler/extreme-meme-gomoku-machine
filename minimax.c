#ifndef MINIMAX
#define MINIMAX
#include "minimax.h"

// curr_player = 1 is us, = 0 is enemy
int* dlminimax(struct Board b, char curr_player, int curr_depth) {
	int **moves = calloc(sizeof(int *),10);
	for(int i=0; i<10; i++) {
		moves[i] = calloc(sizeof(int), 3);
		moves[i][0] = -1;
		moves[i][0] = -1;
		moves[i][0] = -100000;
	}
	getBestMoves(b, moves);
	if(curr_depth <= MAX_DEPTH) {
		for(int m = 0; m < 10 ; m++) {
			Board child = haveChild(b);
			child.board[moves[m][0]][moves[m][1]] = curr_player ? we_are : enemy_is;
			assign_weights(child);
			moves[m] = dlminimax(child, !curr_player, curr_depth+1);
		}
	}
	return moves[0];
}

void getBestMoves(struct Board b, int **moves) {
	int i, j, k, l;
	for(i=0; i<BOARD_SIZE; i++) {
		for(j=0; j<BOARD_SIZE; j++) {
			for(k=0; k<10; k++) {
				if(b.weights[i][j]>moves[k][2]) {
					for(l=10; l>k; l--) {
						moves[l][0] = moves[l-1][0];
						moves[l][1] = moves[l-1][1];
						moves[l][2] = moves[l-1][2];
					}
					moves[k][0] = i;
					moves[k][1] = j;
					moves[k][2] = b.weights[i][j];
				}
			}
		}
	}
}

struct Board haveChild(struct Board parent) {
	/* Initialize baby */
	struct Board child;
	/* Set parameters */
	child.parent = &parent;
	child.children = NULL;
	child.alpha = parent.alpha;
	child.beta = parent.beta;
	/* Copy board and weights. */
	int i, j; /* Iterators. */
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			/* One-by-one copy. */
			child.board[i][j] = parent.board[i][j];
			child.weights[i][j] = parent.weights[i][j];
		}
	}
	child.dont = 0; /* Starts out not pruned. */
	return child;
}
#endif
