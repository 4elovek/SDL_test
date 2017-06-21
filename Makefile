all : main.c
	gcc main.c -ID:\SDL\include -LD:\SDL\lib -w -lmingw32 -lSDL2_ttf -lSDL2_image -lSDL2main -lSDL2 -o main.exe