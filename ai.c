#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct tile **board;

void initBoard() {
	board = (tile **)malloc(BOARD_SIZE * sizeof(tile *));
	for (int i=0; i<BOARD_SIZE; i++) {
		board[i] = (tile *)malloc(BOARD_SIZE * sizeof(tile));
		for (int j=0; j<BOARD_SIZE; j++) {
			board[i][j].status = EMPTY;
			board[i][j].value = 0;
		}
	}
}

void checkBoard() {
	for(int i = 0; i < BOARD_SIZE; i++){
		for(int j = 0; j < BOARD_SIZE; j++){
			if((board[i][j].status == WHITE) || (board[i][j].status == BLACK)){
				printf("%d tile at %d, %d\n", board[i][j].status, i, j);
			}
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
			// get opponent move
			FILE *mv = fopen("move_file","r");
			if (mv != NULL) {
				char opp_move_buf[256] = {0};
				fgets(opp_move_buf, 256, mv);
				printf("opponent move: %s\n", opp_move_buf);
				fseek(mv, 0, SEEK_SET);
				fclose(mv);
			} else {
				printf("move file not found?\n");
				exit(1);
			}

			// write our move
			mv = fopen("move_file","w");
			char move_buf[256] = {0};
			sprintf(move_buf, "meme_machine %c %i\n", 'A', 12);
			fwrite(move_buf, strlen(move_buf), 1, mv);
			fclose(mv);
			remove("meme_machine.go");
			fclose(tf);
			sleep(0.25);
		} else {
			printf("move file not found?\n");
			exit(1);
		}
	}
}
