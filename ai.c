#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define BOARD_SIZE 15

char board[BOARD_SIZE][BOARD_SIZE] = {0};
/*
void checkBoard(){
	int tiles = 0;
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			if((board[i][j] == 0) || (board[i][j] == 1) || (board[i][j] == 2)){
				tiles += 1;
			}
		}
	}
	printf("%i \n", tiles);
}
*/
int main(int argc, char** argv){
/*
	board[0][0] = BLACK;
	board[1][4] = WHITE;
	board[2][6] = BLACK;
	board[2][7] = BLACK;
	board[0][1] = BLACK;

	printf("%d",board[0][0]);
	checkBoard();
*/
while(1) {
	FILE *tf = fopen("meme-machine.go", "r");
	if (tf != NULL) {
		printf("turnfile was found\n");
		FILE *mv = fopen("move_file","r+");
		/*char opp_move_buf[256] = {0};
		fgets(mv, opp_move_buf);*/
		char move_buf[256] = {0};
		sprintf(move_buf, "meme-machine %c %i", 'A', 12);
		fputs(move_buf, mv);
		fclose(mv);
		remove("meme-machine.go");
		fclose(tf);
		sleep(0.25);
	} else {
		printf("turnfile was not found\n");
		sleep(5);
	}
}
}
