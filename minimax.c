#ifndef MINIMAX
#define MINIMAX
#include "minimax.h"

// curr_player = 1 is us, = 0 is enemy
int* dlminimax(struct Board b, char curr_player, int curr_depth) {
	int branching_factor = 10;
	int **moves = calloc(sizeof(int *),branching_factor); /* For any minimax state we have a set of children to explore */
	int i;

	/* For a buffer of 10 moves, set them equal to presets. */
	for(i=0; i<branching_factor; i++) {
		moves[i] = calloc(sizeof(int), 3);
		moves[i][0] = -1; /* X of the move. */
		moves[i][1] = -1; /* Y of the move. */
		moves[i][2] = -100000; /* Value of the move. */
	}
	/* Scan board for the best moves and move them into the move buffer. */
	getBestMoves(b, moves);
	/* Before expanding, see if we need to prune. */
	//abEval(b, moves, curr_player);

	/* Repeat this process for the children of these children; to a predefined depth. */
	if(curr_depth <= MAX_DEPTH) {
		int m;
		/* For each of these moves. */
		if(!(b.dont)){ /* If not pruned. */
			for( m = 0; m < branching_factor ; m++) {
				/* Creat a child on the board. */
				struct Board child = haveChild(b);
				/* Set the location of the move to a tile of the current player. */
				child.board[moves[m][0]][moves[m][1]] = curr_player ? we_are : enemy_is;
				/* Assign weights in the child board w/ new move added. */
				assign_weights(&child);

				/* Recursively call this function on the child. */
				moves[m] = dlminimax(child, !curr_player, curr_depth+1);
			}
		}
	}
	int bestMoveIndex = -1;
	int bestMoveValue = -100000;
	int j;
	for (j=0; j<branching_factor; j++) {
		if(moves[j][2] > bestMoveValue) {
			bestMoveIndex = j;
			bestMoveValue = moves[j][2];
		}
	}
	/* Return first thing in moves */
	return moves[j];
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

void abEval(struct Board b, int** moves, char curr_player){
	int tempa = moves[0][2]; /* Highest possible value that max can get. */
	int tempb = moves[0][2] * -1; /* Largest denial min can force on max. */
	if(we_are == curr_player){
		/* MAX */
		if(tempa > b.parent->alpha){
			/* We have a new alpha. */
			b.parent->alpha = tempa;
		}
		else{
			b.dont = 1;
		}
	}
	else{
		/* MIN */
		if(tempb < b.parent->beta){
			b.parent->beta = tempb;
		}
		else{
			b.dont = 1;
		}
	}
	/* Now pass that value up above */
	if(b.parent != NULL){ /* If there's a parent. */
		abEval((* b.parent), moves, curr_player);
	}
}

struct Board haveChild(struct Board parent) {
	/* Initialize baby */
	struct Board child = {0};
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
