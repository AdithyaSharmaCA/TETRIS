SOURCE = src/main_server.c src/init.c src/defs.c src/input.c src/draw.c src/piece.c src/board.c src/tetris.c src/server.c
LIBRARY = -lmingw32 -lSDL2 -lSDL2main -lSDL2_mixer -lSDL2_image -lSDL2_net
BUILD = build/tetris_server

all:
	gcc -I deps/Include -L deps/lib -o $(BUILD) $(SOURCE) $(LIBRARY)