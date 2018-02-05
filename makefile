all: main

#build
main:
	mkdir -p arena
	rm -f ./arena/ai
	gcc -g -Wall ./ai.c -o ./arena/ai


#build and run
exec: main
	echo
	./arena/ai moku


gdb: main
	gdb ./arena/ai moku

game: main
	./arena/ai moku &
	./arena/ai enemy &
	python ./WPI.CS4341/gomoku/referee.py moku enemy

clean:
	rm -f ./*.o
	rm -f ./arena/ai
	rm *.go
	rm end_game
	rm move_file
	rm history_file