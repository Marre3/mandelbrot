all: main
.PHONY: clean

CC := g++
CFLAGS := -I/opt/homebrew/Cellar/sdl2/2.28.5/include/SDL2 -std=c++20 -Wall -Wextra
LDFLAGS := -lSDL2 -L/opt/homebrew/Cellar/sdl2/2.28.5/lib

main: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main $(LDFLAGS)

run: main
	./main

clean:
	rm -f main
