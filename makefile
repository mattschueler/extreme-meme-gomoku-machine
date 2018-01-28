all: main

main:
	python3 ./bitarray.py ./ai.c ./ai.cpp
	gcc ./ai.cpp -o ./ai

clean:
	#todo




