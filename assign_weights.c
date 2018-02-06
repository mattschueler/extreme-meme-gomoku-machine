#ifndef ASSIGN_C
#define ASSIGN_C
#include "ai.h"
//#define no_print

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
	return;
	if(we_are == BLACK){return;}

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
}

void logPrintWeights(struct Board b) {
	int i,j;
	for(i=0;i<BOARD_SIZE;i++) {
		for(j=0;j<BOARD_SIZE;j++) {
			if(b.board[i][j]==we_are) {
				blackprint("X");
			} else if(b.board[i][j]==enemy_is) {
				blackprint("O");
			} else {
				switch(b.weights[i][j]) {
					case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
						redprint("%d", b.weights[i][j]);
						continue;
					default:
						switch(b.weights[i][j]/10) {
							case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
								orangeprint("%d", b.weights[i][j]/10);
								continue;
							default:
								switch(b.weights[i][j]/100) {
									case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
										greenprint("%d", b.weights[i][j]/100);
										continue;
									default:
										cyanprint("%d", b.weights[i][j]/1000);
										continue;
								}
								continue;
						}
						continue;
				}
			}
		}
		printf("\n");
	}
}

/*void print_board(char board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	if(we_are == BLACK){return;}

	for(i=0; i<BOARD_SIZE; i++){
		printf("\n");
		for(j=0; j<BOARD_SIZE; j++){
			blackprint(" %d ", board[i][j]);
			fflush(stdout);
		}
	}
	printf("\n");
	fflush(stdout);
}*/

#else
void logPrintWeights(struct Board b){return;}
void print_board(char b[BOARD_SIZE][BOARD_SIZE]){return;}
#endif

char convert(char c){
	switch(c){
		case EMPTY:
			return 'E';

		case WHITE:
			if(we_are == WHITE)return 'X';
			else return 'Y';

		case BLACK:
			if(we_are == WHITE)return 'Y';
			else return 'X';
	}
	return 0;
}

void assign_weights(struct Board *b){
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
			if(b->board[i][j] == EMPTY){
				//if the board spot is empty, then it is a possible move for us/the enemy
				int m,n;

				slices considerations = {0};

				int k = 0;
				//check for '|'
				for(m=j-4; m<=j+4; m++){ //VERTICAL slide-rule checks
					if(m<0){continue;}//prevent under-bounds
					if(m>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					considerations.vertical[k] = convert(b->board[i][m]);
					k++;
				}
				//*(considerations.vertical)=*(considerations.vertical)-9;

				k = 0;
				//check for '--'
				for(n=i-4; n<=i+4; n++){ //HORIZONTAL slide-rule checks
					if(n<0){continue;}//prevent under-bounds
					if(n>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					considerations.horizontal[k] = convert(b->board[n][j]);
					k++;
				}
				//*(considerations.horizontal)=*(considerations.horizontal)-9;

				k = 0;
				//check for '/'
				for(m=j-4, n=i-4; (m<=j+4) && (n<=i+4); m++, n++){ //POSITIVE DIAGONAL (/) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					considerations.pos_diag[k] = convert(b->board[n][m]);
					k++;
				}
				//*(considerations.pos_diag)=*(considerations.pos_diag)-9;

				k = 0;
				//check for '\'
				for(m=j+4, n=i-4; (m>=j-4) && (n<=i+4); m--, n++){ //NEGATIVE DIAGONAL (\) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					considerations.neg_diag[k] = convert(b->board[n][m]);
					k++;
				}
				//*(considerations.neg_diag)=*(considerations.neg_diag)-9;

				//convenient break-point for checking "considerations"
				/* Logan likes C-style comments. */
				/* Begin pattern recognition of slices and assign weights. */
				b->weights[i][j] = sliceResolver(considerations);
			}
			else{weights[i][j]=0;}//if the board is not empty, then the weight here should be 0.
		}
	}
	//logPrintWeights(board);
}

/*performs an in-place right-roll of a string (eg 1234 -> 4123 -> 4312 -> 2341 -> 1234)*/
void roll(char* string, int len){
	int i;
	char temp = string[len-1];
	for(i = len-1; i>=0; i--){
		string[i] = string[i-1];
	}
	string[0]=temp;
}


void print_considerations(slices considerations){
	slices toprint = {0};
	if(we_are == BLACK){
		return;
	}
	memcpy(&toprint, &considerations, sizeof(slices));
	char* slice;
	int step;

	for(step=0; step<4; step++){

		switch(step){
			case 0:
			slice = toprint.horizontal;
				break;
			case 1:
			slice = toprint.vertical;
				break;
			case 2:
			slice = toprint.pos_diag;
				break;
			case 3:
			slice = toprint.neg_diag;
				break;
		}
		streplace(slice, 5, EMPTY, 'E');
		if(we_are == BLACK){
			streplace(slice, 5, BLACK, 'X');
			streplace(slice, 5, WHITE, 'Y');
		}else{
			streplace(slice, 5, BLACK, 'Y');
			streplace(slice, 5, WHITE, 'X');
		}
	}


	orangeprint("\nhorizontal:%9s|", toprint.horizontal);
	orangeprint("|vertical:%9s|"  , toprint.vertical);
	orangeprint("|pos_diag:%9s|"  , toprint.pos_diag);
	orangeprint("|neg_diag:%9s|\n", toprint.neg_diag);

}

int contains(char* string, int len, char sub){
	int i;
	for(i=0; i<len; i++){
		if(string[i]==sub){
			return 1;
		}
	}
	return 0;
}

/* Slice Substring Solver: Will parse through the slice and search for */
/* Calls findPattern and updateWeights. Setting will be passed into Set Weights. */
int sliceResolver(slices considerations){
	/* W/ blocksize = 5, parse through a 9 length string. */
	int weight = 0;//initially, we don't care to go here

	//print_considerations(considerations);

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
			memcpy(substring, slice+slice_offset, 5);

			if(contains(substring, 5, (char) 0)){
				continue;//ignore checking "edge-case" substrings, which may be less than length 5 (ie some of the 9 are off-board)
			}

			weight += findWeight(substring);

		}
	}
	return weight;
}

void streplace(char* string, int len, char find, char replace){
	int i;
	for(i=0; i<len; i++){
		if(string[i] == find){string[i] = replace;}
	}
}

void strswap(char* string, int len, char a, char b){
	int i;
	for(i=0; i<len; i++){
		if(string[i] == a){string[i] = b;continue;}
		if(string[i] == b){string[i] = a;continue;}
	}
}

void streverse(char* string, int len){
	int i;
	char temp = ' ';
	for(i = 0; i<len/2; i++){
		temp = string[i];
		string[i] = string[len-i-1];
		string[len-i-1] = temp;
	}
}

/* Find Pattern: a function designed to compare the input string to any of the patterns */
/* That could result in a win. */
/* we convolve the substring on itself in this function*/
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
				if(!strncmp(string,"EEEEE", 5)){return 1;}

				/*1*/
				if(!strncmp(string,"EEEEX", 5)){return 6-inv;}

				/*2*/
				if(!strncmp(string,"EEEXX", 5)){return 32-inv;}
				if(!strncmp(string,"EEXEX", 5)){return 32-inv;}

				/*3*/
				if(!strncmp(string,"EEXXX", 5)){return 128-inv;}
				if(!strncmp(string,"EXEXX", 5)){return 128-inv;}


				/*4*/
				if(!strncmp(string,"EXXXX", 5)){return 4096-inv;}
				/*---*/
				/*2 CORRUPTION 1*/
				if(!strncmp(string,"EEEXY", 5)){return 2-inv;}
				if(!strncmp(string,"EEXEY", 5)){return 2-inv;}

				/*3 CORRUPTION 1*/
				if(!strncmp(string,"YEEXX", 5)){return 24-inv;}
				if(!strncmp(string,"YXEEX", 5)){return 24-inv;}
				if(!strncmp(string,"YEXXE", 5)){return 24-inv;}
				if(!strncmp(string,"YEXEX", 5)){return 24-inv;}

				/*4 CORRUPTION 1*/
				if(!strncmp(string,"YXXXE", 5)){return 72-inv;}
				if(!strncmp(string,"YXXEX", 5)){return 72-inv;}

				/*4 CORRUPTION 2*/
				if(!strncmp(string,"EYYXX", 5)){return 16-inv;}
				if(!strncmp(string,"EYXYX", 5)){return 0-inv;} //this pattern is ALWAYS "salted earth" -- can't be rolled or reversed to form anything useful
				if(!strncmp(string,"YEYXX", 5)){return 3-inv;}

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
