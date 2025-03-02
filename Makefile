.PHONY: run clean

main: main.cpp
	g++ main.cpp -o main -std=c++20

run: main
	./main

clean:
	rm -rf *.o main
