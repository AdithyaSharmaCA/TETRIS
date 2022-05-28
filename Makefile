SOURCE = src/main.c src/init.c src/defs.c src/input.c src/draw.c src/piece.c src/board.c src/tetris.c
LIBRARY = -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image
BUILD = build/tetris

all:
	gcc -I deps/Include -L deps/lib -o $(BUILD) $(SOURCE) $(LIBRARY)