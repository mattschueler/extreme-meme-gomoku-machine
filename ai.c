#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char board[BOARD_SIZE][BOARD_SIZE] = {'\0'};
const char we_are = WHITE;// assume we are the white player be default (?)

void init_board(){
	for(int i=0; i<BOARD_SIZE; i++){
		for(int j=0; j<BOARD_SIZE; j++){
		board[i][j] = EMPTY;
		}
	}
}

int main(int argc, char** argv) {
	while(1) {
		// check for move file
		FILE *eg = fopen("end_game", "r");
		if (eg != NULL) {
			exit(0);
		}
		FILE *tf = fopen("meme_machine.go", "r");
		if (tf != NULL) {
			fclose(tf);

			// get opponent move
			FILE *mv = fopen("move_file","r");
			if (mv != NULL) {
				char opp_move_buf[256] = {0};
				fgets(opp_move_buf, 256, mv);
				strtok(opp_move_buf, " ");
				char *row = strtok(NULL, " ");
				char *col = strtok(NULL, " ");
				printf("opponent move: %s %s\n", row, col);
				fclose(mv);
			} else {
				printf("move file not found?\n");
				exit(1);
			}

			// write our move
			mv = fopen("move_file","w");
			char move_buf[256] = {0};
			// DO THE MINIMAX HERE
			sprintf(move_buf, "meme_machine %c %i\n", 'A', 12);
			fwrite(move_buf, strlen(move_buf), 1, mv);
			fclose(mv);
			sleep(1);
		} else {
			sleep(0.25);
		}
	}
}
