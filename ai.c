#include "ai.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct tile board[BOARD_SIZE][BOARD_SIZE];

void checkBoard(){
	int i;
	for(i = 0; i < 15; i++){
		int j;
		for(j = 0; j < 15; j++){
			if((board[i][j].color == WHITE) || (board[i][j].color == BLACK)){
				printf("%d tile at %d, %d\n", board[i][j].color, i, j);
			}
		}
	}
}

int main(int argc, char** argv){
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
		sleep(0.25);
	}
}
}
