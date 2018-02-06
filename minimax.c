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

int** getBestMoves(char** board){
	int moveset[10][2];

}