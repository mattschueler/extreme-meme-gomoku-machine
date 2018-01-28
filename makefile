all: main

#build
main:
	rm ./ai
	python3 ./bitarray.py ./ai.c ./ai.cpp
	gcc ./ai.cpp -o ./ai
	make cleanup #that recursive make call

#build and run
exec: main
	echo
	./ai

cleanup:
	rm ./*.cpp

clean: cleanup
	rm ./*.o




