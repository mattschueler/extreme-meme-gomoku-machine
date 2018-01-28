all: main

#build
main:
	rm -f ./ai
	gcc ./ai.c -o ./ai


#build and run
exec: main
	echo
	./ai

clean:
	rm -f ./*.o
	rm -f ./ai
