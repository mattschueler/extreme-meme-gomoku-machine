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

				//convenient break-point for checking "considerations"
				/* Logan likes C-style comments. */
				/* Begin pattern recognition of slices and assign weights. */
				sliceSubstringResolver(considerations.horizontal, i, j, 1); /* Just a composite function for it */
				sliceSubstringResolver(considerations.vertical, i, j, 2);
				sliceSubstringResolver(considerations.pos_diag, i, j, 3);
				sliceSubstringResolver(considerations.neg_diag, i, j, 4);
			}
		}
	}
}

/* Slice Substring Solver: Will parse through the slice and search for */
/* Calls findPattern and updateWeights. Setting will be passed into Set Weights. */
void sliceSubstringResolver(char* superstring, int x, int y, int setting){
	/* W/ blocksize = 5, parse through a 9 length string. */
	char substring[5];
	int a, b, start;
	/* Iterate through the superstring in chunks of 5. */
	for(start = 0; start < 5; start++){
		/* Calculate the new x and y based on starting position. */
		int newx = x - (4 - start);
		int newy = y - (4 - start);
		/* Grab the next 5 characters and put them into the substring. */
		for(a = start, b = 0; (a<start+4) && (b<5); a++, b++){
			substring[b] = superstring[a];
			/* Now that we have the substring, test it w/ find pattern. */
			int pat = findPattern(substring);
			/* Adjust score board w/ pattern values. */
			/* Working on horizontal. */
			if(setting == 1){
				/* We're only changing the x while we parse so we need to pass in the updated x. */
				setWeights(pat, newx, y, setting);
			}
			/* Vertical. */
			else if(setting == 2){
				/* Same as above, but with new y. */
			}
			/* Diagonal. */
			else{
				/* For the diagonals we need to pass in both of the new x and y values. */
				setWeights(pat, newx, newy, setting);
			}
		}
	}
}

/* Find Pattern: a function designed to compare the input string to any of the patterns */
/* That could result in a win. */
int findPattern(char* string){
	/* There are several states, or patterns, that lead up to a win. */
	/* Let's only care about combinations greater than 3 across a 5 tile window */
	/* We should have a string for each of these. */
	char* tttee = "TTTEE"; /* 1 */ char* ettte = "ETTTE"; /* 2 */ char* eettt = "EETTT"; /* 3 */ 
	char* teett = "TEETT"; /* 4 */ char* tteet = "TTEET"; /* 5 */ char* ttete = "TTETE"; /* 6 */
	char* etett = "ETETT"; /* 7 */ char* tetet = "TETET"; /* 8 */ char* tttte = "TTTTE"; /* 9 */
	char* etttt = "ETTTT"; /* 10 */ char* ttett = "TTETT"; /* 11 */ char* tttet = "TTTET"; /* 12 */
	char* tettt = "TETTT"; /* 13 */ char* win = "TTTTT"; /* 14 */
	/* Win state */ /* Could use a faster compare if time is a real issue. Shouldn't be. */
	if(strcmp(string, win)){return 14;} /* Comparing test string to the win state string. If true, return pattern #14 */
	else if(strcmp(string, tettt)){return 13;} /* And so on for all 14 states. */
	else if(strcmp(string, tttet)){return 12;} else if(strcmp(string, ttett)){return 11;}
	else if(strcmp(string, etttt)){return 10;} else if(strcmp(string, tttte)){return 9;}
	else if(strcmp(string, tetet)){return 8;} else if(strcmp(string, etett)){return 7;}
	else if(strcmp(string, ttete)){return 6;} else if(strcmp(string, tteet)){return 5;}
	else if(strcmp(string, teett)){return 4;} else if(strcmp(string, eettt)){return 3;}
	else if(strcmp(string, ettte)){return 2;} else if(strcmp(string, tttee)){return 1;}
	/* If no pattern found */
	else{
		return -1;
	}
}

/* Set Weights: Will take a pattern type, a pair of coordinates, and a setting type. */
/* Parses through score table and increments values. */
void setWeights(int pattern, int x, int y, int setting){
	/* If pattern matters. */
	if(pattern != -1){
		if(setting == 1){

		}
		else if(setting == 2){

		}
		else if(setting == 3){

		}
		else if(setting == 4){

		}
		else{
			printf("What the hell are you inputting for setting? Only 1, 2, 3, or 4.");
		}
	}
	/* If it doesn't matter, don't do anything. */
}
