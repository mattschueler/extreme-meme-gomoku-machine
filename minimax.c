#ifndef MINIMAX
#define MINIMAX
#include "minimax.h"

Board *to_free[999];
int to_freec = 0;

void cleanup(){
	int i = 0;
	for(i=0;i<to_freec;i++){
		free(to_free[i]);
	}
}

// curr_player = 1 is us, = 0 is enemy
int* dlminimax(Board* b, char curr_player, int curr_depth) {
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
		if(!(b->dont)){ /* If not pruned. */
			for( m = 0; m < BRANCHING_FACTOR ; m++) {
				/* Creat a child on the board. */
				Board* child = haveChild(b);
				/* Set the location of the move to a tile of the current player. */
				child->board[moves[m][0]][moves[m][1]] = curr_player ? we_are : enemy_is;
				/* Assign weights in the child board w/ new move added. */
				assign_weights(child);
				/* Recursively call this function on the child. */
				moves[m][2] = dlminimax(child, !curr_player, curr_depth+1)[2];
			}
		}
	}

	int j;
	int tempCols[BRANCHING_FACTOR] = {-1};
	int tempRows[BRANCHING_FACTOR] = {-1};
	int range = 0;
	int best;
	if(curr_player) {
		/* Max */
		best = -100000; /* We're trying to go as high as possible. */
	} else {
		/* Min */
		best = 100000; /* They want the opposite. */
	}
	/*for(int k=0;k<BRANCHING_FACTOR;k++) {
		printf("%d %d,%d,%d\t", curr_depth, moves[k][0], moves[k][1], moves[k][2]);
	}
	printf("\n");*/
	/* For each kid. */
	for(j = 0; j < BRANCHING_FACTOR; j++){
		/* If it's not pruned, we're choosing, and we have a value greater than max. */
		if(curr_player && moves[j][2] > best){
			/* This is the new first element of our best moves. */
			range = 0; /* So our range of operation is resitricted when choosing a number randomly. */
			/* Store the position of this new element in temporary holders. */
			tempRows[range]=moves[j][0];
			tempCols[range]=moves[j][1];
			/* Set as new best. */
			best = moves[j][2];
		} 
		/* Otherwise we're the other player and we're trying to minimize. */
		else if(!curr_player && moves[j][2] < best){
			/* If we've found a value lower than the current lowest. */
			range = 0; /* Set operational range to first index. */
			/* Store the location of this new low into our temps. */
			tempRows[range]=moves[j][0];
			tempCols[range]=moves[j][1];
			/* Set best to new best. */
			best = moves[j][2];
		}
		/* If we're either the max or min and our best move isn't better. */
		else if(moves[j][2] == best) {
			/* Increase the operation range of the random. */
			range++;
			/* Store that value next to the other largest number. */
			tempRows[range]=moves[j][0];
			tempCols[range]=moves[j][1];
		}
		/* Nothing new to report, just continue. */
		else{
			continue ;
		}
	}
	/* Now pick a random index across the operational range. */
	int which_move = rand()%range;
	/* Grab a new best move to return. */
	int *bestMove = (int *)calloc(sizeof(int), 3);
	/* Move temp values at random index into best move. */
	bestMove[0] = tempRows[which_move];
	bestMove[1] = tempCols[which_move];
	bestMove[2] = best;
	/* Return a best move. */
	return bestMove;
}

void getBestMoves(Board* b, int **moves, int curr_player) {
	int i, j, k, l;
	for(i=0; i<BOARD_SIZE; i++) {
		for(j=0; j<BOARD_SIZE; j++) {
			if(b->board[i][j]==EMPTY) {
				for(k=0; k<BRANCHING_FACTOR; k++) {
					if(curr_player) {
						if(b->weights[i][j]>moves[k][2]) {
							for(l=BRANCHING_FACTOR-1; l>k; l--) {
								moves[l][0] = moves[l-1][0];
								moves[l][1] = moves[l-1][1];
								moves[l][2] = moves[l-1][2];
							}
							moves[k][0] = i;
							moves[k][1] = j;
							moves[k][2] = b->weights[i][j];
						}
					} else {
						if(b->weights[i][j]<moves[k][2]) {
							for(l=BRANCHING_FACTOR-1; l>k; l--) {
								moves[l][0] = moves[l-1][0];
								moves[l][1] = moves[l-1][1];
								moves[l][2] = moves[l-1][2];
							}
							moves[k][0] = i;
							moves[k][1] = j;
							moves[k][2] = b->weights[i][j];
						}
					}
				}
			}
		}
	}
}

void abEval(Board* b, int** moves, char curr_player){
	int tempa = moves[0][2]; /* Highest possible value that max can get. */
	int tempb = moves[0][2] * -1; /* Largest denial min can force on max. */
	if(we_are == curr_player){
		/* MAX */
		if(tempa > b->parent->alpha){
			/* We have a new alpha. */
			b->parent->alpha = tempa;
		}
		else{
			b->dont = 1;
		}
	}
	else{
		/* MIN */
		if(tempb < b->parent->beta){
			b->parent->beta = tempb;
		}
		else{
			b->dont = 1;
		}
	}
	/* Now pass that value up above */
	if(b->parent != NULL){ /* If there's a parent. */
		abEval(b->parent, moves, curr_player);
	}
}

Board* haveChild(struct Board* parent) {
	/* Initialize baby */
	Board* child = malloc(sizeof(board));
	memset(board,'\0',sizeof(Board)); // zero out the board

	to_freec++;
	to_free[to_freec] = child;
	/* Set parameters */
	child->parent = parent;
	child->children = NULL;
	child->alpha = parent->alpha;
	child->beta = parent->beta;
	/* Copy board and weights. */
	int i, j; /* Iterators. */
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			/* One-by-one copy. */
			child->board[i][j] = parent->board[i][j];
			child->weights[i][j] = parent->weights[i][j];
		}
	}
	child->dont = 0; /* Starts out not pruned. */
	return child;
}
#endif
