#ifndef MINIMAX
#define MINIMAX
#include "minimax.h"

// curr_player = 1 is us, = 0 is enemy
int* dlminimax(struct Board b, char curr_player, int curr_depth) {
	if(curr_depth==0) {
		print_board(b.board);
	}
	int **moves = calloc(sizeof(int *),BRANCHING_FACTOR); /* For any minimax state we have a set of children to explore */
	int i;
	/* For a buffer of 10 moves, set them equal to presets. */
	for(i=0; i<BRANCHING_FACTOR; i++) {
		moves[i] = calloc(sizeof(int), 3);
		moves[i][0] = -1; /* X of the move. */
		moves[i][1] = -1; /* Y of the move. */
		moves[i][2] = -100000; /* Value of the move. */
	}
	/* Scan board for the best moves and move them into the move buffer. */
	getBestMoves(b, moves, curr_player);
	/* Before expanding, see if we need to prune. */
	//abEval(b, moves, curr_player);

	/* Repeat this process for the children of these children; to a predefined depth. */
	if(curr_depth <= MAX_DEPTH) {
		int m;
		/* For each of these moves. */
		if(!(b.dont)){ /* If not pruned. */
			for( m = 0; m < BRANCHING_FACTOR ; m++) {
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

	int j;
	int tempCols[BRANCHING_FACTOR] = {-1};
	int tempRows[BRANCHING_FACTOR] = {-1};
	int range = 0;
	int best;
	if(curr_player) {
		best = -100000;
	} else {
		best = 100000;
	}
	/*for(int k=0;k<BRANCHING_FACTOR;k++) {
		printf("%d %d,%d,%d\t", curr_depth, moves[k][0], moves[k][1], moves[k][2]);
	}
	printf("\n");*/
	for(j = 0; j < BRANCHING_FACTOR; j++){
		if(curr_player) {
			if(moves[j][2]>best) {
				range = 0;
				tempRows[range]=moves[j][0];
				tempCols[range]=moves[j][1];
				best = moves[j][2];
			}
		} else {
			if(moves[j][2]<best){
				range = 0;
				tempRows[range]=moves[j][0];
				tempCols[range]=moves[j][1];
				best = moves[j][2];
			}
		}
		if(moves[j][2]==best) {
			range++;
			tempRows[range]=moves[j][0];
			tempCols[range]=moves[j][1];
		}
	}
	int which_move = rand()%range;
	int *bestMove = (int *)calloc(sizeof(int), 3);
	bestMove[0] = tempCols[which_move];
	bestMove[1] = tempRows[which_move];
	bestMove[2] = best;
	return bestMove;
}

void getBestMoves(struct Board b, int **moves, int curr_player) {
	int i, j, k, l;
	for(i=0; i<BOARD_SIZE; i++) {
		for(j=0; j<BOARD_SIZE; j++) {
			if(b.board[i][j]==EMPTY) {
				for(k=0; k<BRANCHING_FACTOR; k++) {
					if(curr_player) {
						if(b.weights[i][j]>moves[k][2]) {
							for(l=BRANCHING_FACTOR-1; l>k; l--) {
								moves[l][0] = moves[l-1][0];
								moves[l][1] = moves[l-1][1];
								moves[l][2] = moves[l-1][2];
							}
							moves[k][0] = i;
							moves[k][1] = j;
							moves[k][2] = b.weights[i][j];
						}
					} else {
						if(b.weights[i][j]<moves[k][2]) {
							for(l=BRANCHING_FACTOR-1; l>k; l--) {
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
