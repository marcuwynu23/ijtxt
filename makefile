SRC = $(wildcard src/*.c)
INCLUDE = $(wildcard include/*.h)
DIST = dist/main.exe

all: $(SRC) $(INCLUDE) $(DIST)
	gcc -o $(DIST) -Iinclude $(SRC)