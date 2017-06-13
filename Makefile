all : main.c
	gcc main.c -ID:\SDL\include -LD:\SDL\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2_image -lSDL2main -lSDL2 -o main.exe