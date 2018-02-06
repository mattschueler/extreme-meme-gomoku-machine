#include "ai.h"
#include "assign_weights.c"

int i = 0;

void dlminimax(struct Board b){
	int whoturn = 1; /* 0 for us, 1 for the enemy. */
	int m;
	char** gamestate = b.board;
	assign_weights(b); /* Now b.weights should be a list of all the weights. */
	int moves[10][2] = getBestMoves(gamestate);
	i++;
	for(m = 0; m < 10 , i < MINIMAX_DEPTH; m++){
		int tempmove[2] = moves[m];
		Board child = haveChild(b);

		dlminimax();
	}
}

int** getBestMoves(struct Board b){
	int moveset[10][3] = {-1,-1,-100000}; // x coord, y coord, score
	for(int i=0; i<BOARD_SIZE; i++) {
		for(int j=0; j<BOARD_SIZE; j++) {
			for(int k=0; k<10; k++) {
				if(board.weights[i][j]>moveset[k][2]) {
					for(int l=10; l>k; l--) {
						moveset[l][0] = moveset[l-1][0];
						moveset[l][1] = moveset[l-1][1];
						moveset[l][2] = moveset[l-1][2];
					}
					moveset[k][0] = i;
					moveset[k][1] = j;
					moveset[k][2] = board.weights[i][j];
				}
			}
		}
	}
	return moveset;
}

Board haveChild(struct Board parent){
	/* Initialize baby */
	Board child;
	/* Set parameters */
	child.parent = parent;
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
	/* Set best move. */
	board.bestmove = getBestMoves(child);
	board.dont = 0; /* Starts out not pruned. */
}