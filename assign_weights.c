#include "ai.h"
#define no_print

struct slices {
	char horizontal[9];
	char   vertical[9];
	char   pos_diag[9];
	char   neg_diag[9];
} typedef slices;

#ifndef no_print
void print_board(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for(i=0; i<BOARD_SIZE; i++){
		printf("\n");
		for(j=0; j<BOARD_SIZE; j++){
			switch(board[i][j]){
				case BLACK:
					blackprint("B");
					continue;
				case WHITE:
					whiteprint("W");
					continue;
				case EMPTY:
					blackprint(" ");
					continue;
			}
		}
	}
	printf("\n");
	fflush(stdout);
	reset_print_board(board);
}
void reset_print_board(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	for(i=0; i<BOARD_SIZE; i++){
		printf("\b");
		for(j=0; j<BOARD_SIZE; j++){
			printf("\b");
		}
	}
}
#else
void print_board(char board[BOARD_SIZE][BOARD_SIZE]){return;}
void reset_print_board(char board[BOARD_SIZE][BOARD_SIZE]){return;}
#endif


void assign_weights(char board[BOARD_SIZE][BOARD_SIZE]){
/*
	if(we_are == WHITE){
		char enemy = BLACK;
	}else{
		char enemy = WHITE;
	} //we will see if we will use this.
*/
	int i, j;
	for(i=0; i<BOARD_SIZE; i++){
		for(j=0; j<BOARD_SIZE; j++){
			if(board[i][j] == EMPTY){
				//if the board spot is empty, then it is a possible move for us/the enemy
				int m,n;

				slices considerations;

				//check for '|'
				for(m=j-4; m<j+4; m++){ //VERTICAL slide-rule checks
					if(m<0){m=0;}//prevent under-bounds
					if(m>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.vertical) = board[i][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.vertical))++;
				}

				//check for '--'
				for(n=i-4; n<i+4; n++){ //HORIZONTAL slide-rule checks
					if(n<0){n=0;}//prevent under-bounds
					if(n>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.horizontal) = board[n][j]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.horizontal))++;
				}

				//check for '/'
				for(m=j-4, n=i-4; (m<j+4) && (n<i+4); m++, n++){ //POSITIVE DIAGONAL (/) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.pos_diag) = board[n][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.pos_diag))++;
				}

				//check for '\'
				for(m=j+4, n=i-4; (m>j-4) && (n<i+4); m--, n++){ //NEGATIVE DIAGONAL (\) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.neg_diag) = board[n][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.neg_diag))++;
				}
				/* check for patterns and add weights */
			}
		}
	}
}

void sliceSubstringResolver(char* superstring, int x, int y, int setting){}
void setWeights(int pattern, int x, int y, int setting){}