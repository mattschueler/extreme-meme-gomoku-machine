#include "ai.h"
#include "assign_weights.c"

int i = 0;

void dlminimax(Board b){
	int whoturn = 1; /* 0 for us, 1 for the enemy. */
	int m;
	char** gamestate = b.board;
	int moves[10][2] = getBestMoves(gamestate);
	i++;
	for(m = 0; m < 10 , i < MINIMAX_DEPTH; m++){
		int tempmove[2] = moves[m];
		Board child =

		dlminimax();
	}
}

int** getBestMoves(struct Board board){
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
