#include "ai.h"
#include "assign_weights.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char we_are = EMPTY;
char TEAMNAME[32] = {0};

void init_board() {
	srand(time(NULL));
	for(int i=0; i<BOARD_SIZE; i++) {
		for(int j=0; j<BOARD_SIZE; j++) {
			board[i][j] = EMPTY;
		}
	}
}

void getMove(int *col, int *row) {
	int tempCol = -1;
	int tempRow = -1;
	while(1) {
		tempCol = rand()%15;
		tempRow = rand()%15;
		if(board[tempRow][tempCol] == EMPTY) {
			break;
		}
	}
	*col = tempCol;
	*row = tempRow;
}

int main(int argc, char** argv) {
	init_board();
	strcpy(TEAMNAME, argv[1]);
	while(1) {
		print_board(board);
		// check for move file
		FILE *eg = fopen("end_game", "r");
		if(eg != NULL) {
			exit(0);
		}
		char *fname = (char *)calloc(32,1);
		sprintf(fname, "%s.go", TEAMNAME);
		FILE *tf = fopen(fname, "r");
		if(tf != NULL) {
			fclose(tf);

			// get opponent move
			FILE *mv = fopen("move_file","r");
			if(mv != NULL) {
				char opp_move_buf[256] = {0};
				fgets(opp_move_buf, 256, mv);
				fclose(mv);
				if(strlen(opp_move_buf)>0) {
					if(we_are == EMPTY) {
						we_are = BLACK;
					}
					strtok(opp_move_buf, " ");
					char *oppCol = strtok(NULL, " ");
					char *oppRow = strtok(NULL, " ");
					board[atoi(oppRow)-1][oppCol[0]-97] = (we_are%2) ? BLACK : WHITE;
					printf("opponent move: %c %d\n", oppCol[0], atoi(oppRow));
				} else {
					we_are = WHITE;
				}
			} else {
				redprint("ERROR: move file not found?\n");
				exit(1);
			}

			// write our move
			char move_buf[256] = {0};
			int col, row;
			getMove(&col, &row);
			// DO THE MINIMAX HERE
			board[row][col] = we_are;
			sprintf(move_buf, "%s %c %d\n", TEAMNAME, col+97, row+1);
			printf("%s\n", move_buf);
			mv = fopen("move_file","w");
			fwrite(move_buf, strlen(move_buf), 1, mv);
			fclose(mv);
			sleep(1);
		} else {
			sleep(0.25);
		}
	}
}
