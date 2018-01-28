all: main

#build
main:
	rm -f ./ai
	python3 ./bitarray.py ./ai.c ./ai.cpp
	gcc ./ai.cpp -o ./ai


#build and run
exec: main
	echo
	./ai

clean: cleanup
	rm -f ./*.o
	rm -f ./*.cpp
	rm -f ./ai



