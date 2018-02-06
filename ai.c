#include "ai.h"
#include "assign_weights.c"
#include "minimax.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char TEAMNAME[32] = {0};
struct Board globalBoard;

void initWeights(struct Board board){
	int i, j;
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			board.weights[i][j] = 1;
		}
	}
	for(i = 4; i < BOARD_SIZE-5; i++){
		for(j = 4; j < BOARD_SIZE-5; j++){
			board.weights[i][j] = 4;
		}
	}
}

void init_board(struct Board board) {
	srand(time(NULL));
	board.parent = NULL;
	board.children = NULL;
	board.alpha = -100000;
	board.beta = 100000;
	int i, j;
	for(i=0; i<BOARD_SIZE; i++) {
		for(j=0; j<BOARD_SIZE; j++) {
			board.board[i][j] = EMPTY;
		}
	}
	initWeights(board);
	board.dont = 0;
}


void getMove(struct Board board, int *col, int *row) {
	int i,j;
	int tempCols[BOARD_SIZE*BOARD_SIZE] = {-1};
	int tempRows[BOARD_SIZE*BOARD_SIZE] = {-1};
	int range = 0;
	int max = -1;
	assign_weights(board);

	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
			if(( board.weights[i][j]>max )&&( board.board[i][j]==EMPTY )){
				range = 0;
				tempRows[range]=i;
				tempCols[range]=j;
				max = board.weights[i][j];
			}
			if((board.weights[i][j]==max)&&(board.board[i][j]==EMPTY)){
				range++;
				tempRows[range]=i;
				tempCols[range]=j;
			}
		}
	}
	int which_move = rand()%range;
	*col = tempCols[which_move];
	*row = tempRows[which_move];
}

int main(int argc, char** argv) {
	init_board(globalBoard);

	if(argc<2){
		redprint("ERROR: MUST SUPPLY A TEAMNAME\n");
		exit(1);
	}

	strcpy(TEAMNAME, argv[1]);
	while(1) {
		// check for move file
		FILE *eg = fopen("end_game", "r");
		if(eg != NULL) {
			greenprint("GAME EXITED");
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
						enemy_is = WHITE;
					}
					strtok(opp_move_buf, " ");
					char *oppCol = strtok(NULL, " ");
					char *oppRow = strtok(NULL, " ");
					globalBoard.board[atoi(oppRow)-1][oppCol[0]-97] = enemy_is;
					printf("opponent move: %c %d\n", oppCol[0], atoi(oppRow));
				} else {
					we_are = WHITE;
					enemy_is = BLACK;
				}
			} else {
				redprint("ERROR: move file not found?\n");
				exit(1);
			}

			// write our move
			char move_buf[256] = {0};
			int col, row;
			getMove(globalBoard, &col, &row);
			// DO THE MINIMAX HERE
			globalBoard.board[row][col] = we_are;
			sprintf(move_buf, "%s %c %d\n", TEAMNAME, col+97, row+1);
			printf("%s\n", move_buf);
			mv = fopen("move_file","w");
			fwrite(move_buf, strlen(move_buf), 1, mv);
			fclose(mv);
			print_board(globalBoard.board);
			sleep(1);
		} else {
			sleep(0.25);
		}
	}
}
