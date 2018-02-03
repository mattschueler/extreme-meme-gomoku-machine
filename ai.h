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


// -----------------------------------------------

// PROJECT SPECIFIC INCLUDES ARE BELOW:
#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define BOARD_SIZE 15


/* Tile Struct */
struct tile {
	/* Marker for color */
	char status;
	/* Marker for the value of the tile based on heurstic */
	short value;
} tile;