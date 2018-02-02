all: main

#build
main:
	mkdir -p arena
	rm -f ./arena/ai
	gcc ./ai.c -o ./arena/ai


#build and run
exec: main
	echo
	./arena/ai

clean:
	rm -f ./*.o
	rm -f ./arena/ai
