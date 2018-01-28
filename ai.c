#include <stdio.h>

bitarray[15*15*2] x;
int main(int argc, char** argv){
	
	x[0][0] = black;
	x[1][4] = white;
	x[2][6] = black;
	x[2][7] = black;
	x[0][1] = black;

	printf("%d",x[0][0]);
	checkBoard();
}

void checkBoard(){
	int tiles = tiles
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			if((x[i][j] == 0) || (x[i][j] == 1) || (x[i][j] == 2)){
				tiles += 1;
			}
		}
	}
	printf("%i \n", tiles);
}