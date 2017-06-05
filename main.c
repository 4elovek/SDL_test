#include <SDL.h>
#include <stdio.h>
#define width 800
#define height 600
int init_sdl(SDL_Window **w, SDL_Surface **surf);
int quit_sdl(SDL_Window *w, SDL_Surface **surf);
int main(int argc, char* argv[])
{
	SDL_Window* wind = NULL;
	SDL_Surface* surf= NULL;
	if(init_sdl(&wind, &surf)){
		SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0xff, 0x00, 0x00));
		SDL_UpdateWindowSurface(wind);	
	}
	SDL_Delay(2000);
	quit_sdl(&wind, &surf);
}

int init_sdl(SDL_Window **w, SDL_Surface **surf){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL error: %s", SDL_GetError());
		return 0;
	}
	*w = SDL_CreateWindow("Main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	*surf = SDL_GetWindowSurface(*w);
	return 1;
}

int quit_sdl(SDL_Window *w, SDL_Surface **surf){
	SDL_DestroyWindow(w);
	SDL_Quit();
	return 0;
}