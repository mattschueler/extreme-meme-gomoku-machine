all: main

#build
main:
	mkdir -p arena
	rm -f ./arena/ai
	gcc -g -Wall ./ai.c -o ./arena/ai
	gcc -g ./minimax.c -o ./minimax

#build and run
exec: main
	echo
	./arena/ai moku


gdb: main
	gdb ./arena/ai moku

game: clean main
	python ./WPI.CS4341/gomoku/referee.py moku enemy
	./arena/ai moku &
	./arena/ai enemy &

ref:
	git clone https://github.com/samogden/WPI.CS4341.git

clean:
	rm -f ./*.o
	rm -f ./arena/ai
	rm -f *.go
	rm -f end_game
	rm -f move_file
	rm -f history_file