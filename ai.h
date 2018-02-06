#ifndef AI_H
#define AI_H
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
//#include "minimax.c"

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


// -----------------------------------------------

// PROJECT SPECIFIC INCLUDES ARE BELOW:
#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define BOARD_SIZE 15

struct slices {
	char horizontal[9];
	char   vertical[9];
	char   pos_diag[9];
	char   neg_diag[9];
} typedef slices;

struct Board {
	struct Board* parent; /* Parent of this board. */
	struct Board* children; /* Children of this board. */
	int alpha, beta;

	char board[BOARD_SIZE][BOARD_SIZE]; /* A snapshot of the current board. */
	int weights[BOARD_SIZE][BOARD_SIZE]; /* Set of weights accomanying the board. */
	int dont; /* If 1 this board has been pruned. */
} typedef Board;

void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void reset_print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void assign_weights(struct Board board);
int sliceResolver(slices considerations);
void streplace(char* string, int len, char find, char replace);
void strswap(char* string, int len, char a, char b);
void streverse(char* string, int len);
void roll(char* string, int len);
int findWeight(char* string);

char we_are = EMPTY;
char enemy_is = EMPTY;

char board[BOARD_SIZE][BOARD_SIZE] = {EMPTY};
int weights[BOARD_SIZE][BOARD_SIZE] = {0};


#endif
