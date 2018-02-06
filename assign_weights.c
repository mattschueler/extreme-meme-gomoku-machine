#ifndef ASSIGN_C
#define ASSIGN_C
#include "ai.h"
#define no_print

/* (defined in ai.h, but here for reference)
struct slices {
	char horizontal[9];
	char   vertical[9];
	char   pos_diag[9];
	char   neg_diag[9];
} typedef slices;
*/

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


void assign_weights(struct Board board){
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
			if(board.board[i][j] == EMPTY){
				//if the board spot is empty, then it is a possible move for us/the enemy
				int m,n;

				slices considerations;

				//check for '|'
				for(m=j-4; m<j+4; m++){ //VERTICAL slide-rule checks
					if(m<0){m=0;}//prevent under-bounds
					if(m>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.vertical) = board.board[i][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.vertical))++;
				}

				//check for '--'
				for(n=i-4; n<i+4; n++){ //HORIZONTAL slide-rule checks
					if(n<0){n=0;}//prevent under-bounds
					if(n>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.horizontal) = board.board[n][j]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.horizontal))++;
				}

				//check for '/'
				for(m=j-4, n=i-4; (m<j+4) && (n<i+4); m++, n++){ //POSITIVE DIAGONAL (/) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.pos_diag) = board.board[n][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.pos_diag))++;
				}

				//check for '\'
				for(m=j+4, n=i-4; (m>j-4) && (n<i+4); m--, n++){ //NEGATIVE DIAGONAL (\) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					*(considerations.neg_diag) = board.board[n][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
					(*(considerations.neg_diag))++;
				}

				//convenient break-point for checking "considerations"
				/* Logan likes C-style comments. */
				/* Begin pattern recognition of slices and assign weights. */
				board.weights[i][j] = sliceResolver(considerations);
			}
		}
	}
}

/*performs an in-place right-roll of a string (eg 1234 -> 4123 -> 4312 -> 2341 -> 1234)*/
void roll(char* string, int len){
	redprint("%s",string);
	char first = string[len];
	strncpy(string+1, string, len-1);
	string[0] = first;
	redprint("%s",string);
}

/* Slice Substring Solver: Will parse through the slice and search for */
/* Calls findPattern and updateWeights. Setting will be passed into Set Weights. */
int sliceResolver(slices considerations){
	/* W/ blocksize = 5, parse through a 9 length string. */
	int weight = 0;//initially, we don't care to go here

	char* slice;
	char substring[5];

	int slice_offset;
	int step;

	for(step=0; step<4; step++){

		switch(step){
			case 0:
			slice = considerations.horizontal;
				break;
			case 1:
			slice = considerations.vertical;
				break;
			case 2:
			slice = considerations.pos_diag;
				break;
			case 3:
			slice = considerations.neg_diag;
				break;
		}

		for(slice_offset = 0; slice_offset<5; slice_offset++){
			strncpy(substring, slice+slice_offset, 5);

			weight += findWeight(substring);
		}
	}
	return weight;
}

void streplace(char* string, int len, char find, char replace){
	for(; len>0; len--){
		if(string[len-1] == find){string[len-1] = replace;}
	}
}

void strswap(char* string, int len, char a, char b){
	for(; len>0; len--){
		if(string[len-1] == a){string[len-1] = b;continue;}
		if(string[len-1] == b){string[len-1] = a;continue;}
	}
}

void streverse(char* string, int len){
	char temp;
	int i;
	for(i = 0; i<(len/2); i++){
		temp = string[i];
		string[i] = string[len-i];
		string[len-1] = temp;
	}
}

/* Find Pattern: a function designed to compare the input string to any of the patterns */
/* That could result in a win. */
int findWeight(char* string){
	int roll_attempts = 5;
	int inverse_attempts = 2;
	int reverse_attempts = 2;
	int inv = -1;//whether we have inversed yet or not. inversing our match incurs a penalty of 2, yet makes the search space complete.
	streplace(string, 5, EMPTY, 'E');
	if(we_are == BLACK){
		streplace(string, 5, BLACK, 'X');
		streplace(string, 5, WHITE, 'Y');
	}else{
		streplace(string, 5, BLACK, 'Y');
		streplace(string, 5, WHITE, 'X');
	}

	for(inverse_attempts=2; inverse_attempts>0; inverse_attempts--){
		for(reverse_attempts=2; reverse_attempts>0; reverse_attempts--){
			for(roll_attempts=5; roll_attempts>0; roll_attempts--){

				/*0*/
				if(strcmp(string,"EEEEE")){return 0;}

				/*1*/
				else if(strcmp(string,"EEEEX")){return 2-inv;}

				/*2*/
				else if(strcmp(string,"EEEXX")){return 16-inv;}
				else if(strcmp(string,"EEXEX")){return 16-inv;}

				/*3*/
				else if(strcmp(string,"EEXXX")){return 64-inv;}
				else if(strcmp(string,"EXEXX")){return 64-inv;}

				/*4*/
				else if(strcmp(string,"EXXXX")){return 1024-inv;}
				/*---*/
				/*2 CORRUPTION 1*/
				else if(strcmp(string,"EEEXY")){return 2-inv;}
				else if(strcmp(string,"EEXEY")){return 2-inv;}

				/*3 CORRUPTION 1*/
				else if(strcmp(string,"EEXXY")){return 16-inv;}
				else if(strcmp(string,"EEXYX")){return 16-inv;}
				else if(strcmp(string,"EYEXX")){return 16-inv;}
				else if(strcmp(string,"EXEYX")){return 16-inv;}
				else if(strcmp(string,"EXEXY")){return 16-inv;}

				/*4 CORRUPTION 1*/
				else if(strcmp(string,"EYXXX")){return 256-inv;}
				else if(strcmp(string,"EXYXX")){return 16-inv;}

				/*4 CORRUPTION 2*/
				else if(strcmp(string,"EYYXX")){return 16-inv;}
				else if(strcmp(string,"EYXYX")){return 2-inv;}

				roll(string, 5);
			}

		streverse(string, 5);//reverse the string
		}

		strswap(string, 5, 'X','Y');
		inv = 1;//if we had to inverse the search string to match, then we are looking at sub-optimal, potentially over-defensive moves, so we should subtract a bit of points to prefer aggressive, tie-breaking/game-winning moves.
	}

	redprint("ERROR: IN FINDPATTERN; DID NOT RECOGNIZE PATTERN %s\n", string);
	return ((uint) 1)>>1; //the most negative value (but we should never reach this point)
}
#endif
