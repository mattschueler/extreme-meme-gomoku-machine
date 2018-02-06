#include "ai.h"
#include "assign_weights.c"

int i = 0;
int mmdepth = 5;

void dlminimax(struct Board b){
	int whoturn = 0; /* 0 us, 1 enemy */
	int m;
	assign_weights(b); /* Now b.weights should be a list of all the weights. */
	int** moves = getBestMoves(b);
	i++;
	for(m = 0; m < 10 , i < mmdepth; m++){
		int** tempmove = moves[m];
		Board child = haveChild(b);
		int x = tempmove[1];
		int y = tempmove[2];
		if(whoturn = we_are){
			child.board[x][y] = we_are;
			whoturn += 1;
		}
		else{
			child.board[x][y] = we_are;
			whoturn -= 1;
		}
		dlminimax(child);
	}
}

int** getBestMoves(struct Board b){
	int moveset[10][3] = {-1,-1,-100000}; // x coord, y coord, score
	int i, j, k, l;
	for(i=0; i<BOARD_SIZE; i++) {
		for(j=0; j<BOARD_SIZE; j++) {
			for(k=0; k<10; k++) {
				if(b.weights[i][j]>moveset[k][2]) {
					for(l=10; l>k; l--) {
						moveset[l][0] = moveset[l-1][0];
						moveset[l][1] = moveset[l-1][1];
						moveset[l][2] = moveset[l-1][2];
					}
					moveset[k][0] = i;
					moveset[k][1] = j;
					moveset[k][2] = b.weights[i][j];
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
	child.bestmove = parent.bestmove;
	child.dont = 0; /* Starts out not pruned. */
}
