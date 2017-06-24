all : main.c game.c
	gcc main.c game.c  -Iinclude -Llib -w -lmingw32 -lSDL2_ttf -lSDL2_image -lSDL2main -lSDL2 -o main.exe