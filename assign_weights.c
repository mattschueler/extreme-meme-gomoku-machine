#include <string.h> //for string comprehension and memset
#include <unistd.h> //for read and close
#include <sys/types.h> // for type declarations
#include <sys/stat.h> // for mode constants
#include <fcntl.h> //for O_* constants
#include <stdio.h> //for printing
#include <stdlib.h> //for malloc
#include <semaphore.h> // for sempahores
#include <pthread.h> // for threads
#include <sys/time.h> //for time
#include <sys/mman.h> // for mmap

#define RED   "\x1B[31m"
#define CYAN   "\x1B[36m" // fancy color FONT markers
#define GREEN   "\x1B[32m"
#define ORANGE "\x1B[33m"
#define RESET "\x1B[0m"

#define    redprint(str, ...) fprintf(stderr,    RED str RESET, ##__VA_ARGS__)
#define   cyanprint(str, ...) fprintf(stderr,   CYAN str RESET, ##__VA_ARGS__)
#define  greenprint(str, ...) fprintf(stderr,  GREEN str RESET, ##__VA_ARGS__)
#define orangeprint(str, ...) fprintf(stderr, ORANGE str RESET, ##__VA_ARGS__)

#define GREY "\x1B[40m" //dark grey background
#define BLACK_F "\x1B[1m" //black font
#define WHITE_F "\x1B[1;37m" //white font

#define blackprint(str, ...) fprintf(stderr, GREY BLACK_F str RESET, ##__VA_ARGS__)
#define whiteprint(str, ...) fprintf(stderr, GREY WHITE_F str RESET, ##__VA_ARGS__)


void print_board(char board[BOARD_SIZE][BOARD_SIZE]){
	for(int i=0; i<BOARD_SIZE; i++){
		printf("\n");
		for(int j=0; j<BOARD_SIZE; j++){
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
}


void assign_weights(char board){
	
	if(we_are == WHITE){
		char enemy = BLACK;
	}else{
		char enemy = WHITE;
	} //we will see if we will use this.

	for(int i=0; i<BOARD_SIZE; i++){
		for(int j=0; j<BOARD_SIZE; j++){


			if(board[i][j] == EMPTY){
				//if the board spot is empty, then it is a possible move for us/the enemy
				//check for '/' and '\'


				//check for '|'
				for(int m=j-4; m<j+4; m++){ //VERTICAL slide-rule checks
					if(m<0){m=0;}//prevent under-bounds
					if(m>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					board[i][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
				}

				//check for '--'
				for(int n=i-4; n<i+4; n++){ //HORIZONTAL slide-rule checks
					if(n<0){n=0;}//prevent under-bounds
					if(n>BOARD_SIZE){break;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					board[n][j]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
				}

				//check for '/'
				for(int m=j-4, int n=i-4; (m<j+4) && (n<i+4); m++, n++){ //POSITIVE DIAGONAL (/) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					board[n][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
				}

				//check for '\'
				for(int m=j+4, int n=i-4; (m>j-4) && (n<i+4); m--, n++){ //NEGATIVE DIAGONAL (\) slide-rule checks
					if((n<0)||(m<0)){continue;}//prevent under-bounds (continuing will increment BOTH m and n simultaneously, keeping our constraint of m on n)
					if((n>BOARD_SIZE)||(m>BOARD_SIZE)){continue;}//prevent over-bounds (since we are going left-to-right, then hitting the right bound should cause us to stop searching)

					board[n][m]; //WE WILL NEED TO DO SOMETHING HERE, LIKELY WITH ANOTHER MATRIX OF "BADNESS" AND "GOODNESS" VALUES.
				}

			}		
		}
	}
}