.PHONY: run clean

run: main
	./main

clean:
	rm -rf *.o main

main: main.cpp
	g++ main.cpp -o main
