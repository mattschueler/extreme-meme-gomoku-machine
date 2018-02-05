all: main

#build
main:
	mkdir -p arena
	rm -f ./arena/ai
	gcc -g -Wall ./ai.c -o ./arena/ai


#build and run
exec: main
	echo
	./arena/ai


gdb: main
	gdb ./arena/ai

clean:
	rm -f ./*.o
	rm -f ./arena/ai
