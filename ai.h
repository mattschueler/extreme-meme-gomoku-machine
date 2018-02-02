/* Include file for all of our structs and prototypes. */
#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define BOARD_SIZE 15

/* Tile Struct */
struct tile {
	/* Marker for color */
	int color;
	/* Marker for the value of the tile based on heurstic */
	int value;
} tile;
