SOURCE = src/main_client.c src/init.c src/defs.c src/input.c src/draw.c src/piece.c src/board.c src/tetris.c src/client.c
LIBRARY = -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_net
BUILD = build/tetris_client

all:
	gcc -I deps/Include -L deps/lib -o $(BUILD) $(SOURCE) $(LIBRARY)