all:
	gcc -I src/Include -Lsrc/lib -o main main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image