#ifndef MINIMAX
#define MINIMAX
#include "minimax.h"

// curr_player = 1 is us, = 0 is enemy
int* dlminimax(Board b, char curr_player, int curr_depth) {
	int **moves = calloc(sizeof(int *),10); /* For any minimax state we have a set of children to explore */
	int i;
	char next_player;
	
	if(curr_player == BLACK){
		next_player = WHITE;
	}else{next_player = BLACK;}

	/* For a buffer of 10 moves, set them equal to presets. */
	for(i=0; i<10; i++) {
		moves[i] = calloc(sizeof(int), 3);
		moves[i][0] = -1; /* X of the move. */
		moves[i][0] = -1; /* Y of the move. */
		moves[i][0] = -100000; /* Value of the move. */
	}
	/* Scan board for the best moves and move them into the move buffer. */
	getBestMoves(b, moves);
	/* Before expanding, see if we need to prune. */
	

	/* Repeat this process for the children of these children; to a predefined depth. */
	if(curr_depth <= MAX_DEPTH) {
		int m;
		/* For each of these moves. */
		if(!dont){ /* If not pruned. */
			for( m = 0; m < 10 ; m++) {
				/* Creat a child on the board. */
				Board child = haveChild(b);
				/* Set the location of the move to a tile of the current player. */
				child.board[moves[m][0]][moves[m][1]] = curr_player;
				/* Assign weights in the child board w/ new move added. */
				assign_weights(child);

				/* Recursively call this function on the child. */
				moves[m] = dlminimax(child, next_player, curr_depth+1);
			}
		}
	}

	/* Return first thing in moves */
	return moves[0];
}

void getBestMoves(Board b, int **moves) {
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

Board haveChild(Board parent) {
	/* Initialize baby */
	Board child = {0};
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
