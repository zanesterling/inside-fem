.PHONY: run clean debug

main: main.cpp
	g++ main.cpp -o main -std=c++20 -g

run: main
	./main

clean:
	rm -rf *.o main

debug: main
	lldb main
