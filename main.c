#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
static int width = 800;
static int height = 600;
int Init_SDL(SDL_Window **w, SDL_Surface **surf);
int Quit_SDL(SDL_Window *w);
SDL_Surface* LoadSurface(char* path);
SDL_Surface* OptimizeSurface(SDL_Surface *opt, SDL_Surface *formatter);
int main(int argc, char* argv[])
{
	SDL_Window* wind = NULL;
	SDL_Surface* surf= NULL;
	if(Init_SDL(&wind, &surf)){
		int quit = 0;
		SDL_Event e;
		SDL_Surface *img = OptimizeSurface(LoadSurface("shodan.jpg"), surf);
		while(!quit){
			SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 0xff, 0x00, 0x00));
			SDL_BlitSurface(img, NULL, surf, NULL);
			SDL_UpdateWindowSurface(wind);
			while(SDL_PollEvent(&e) != 0){
					quit = (e.type == SDL_Quit);
			}
		}
	}
	SDL_Delay(2000);
	Quit_SDL(&wind);
}
SDL_Surface* LoadSurface(char* path){
	SDL_Surface* s = IMG_Load(path);
	if(s == NULL){
		printf("Error loading %s: %s", path, IMG_GetError());
	}
	return s;
}
SDL_Surface* OptimizeSurface(SDL_Surface *opt, SDL_Surface *formatter){
	SDL_Surface* ret = SDL_ConvertSurface(opt, formatter->format, NULL);
	SDL_FreeSurface(opt);
	return ret;
}
int Init_SDL(SDL_Window **w, SDL_Surface **surf){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL error: %s", SDL_GetError());
		return 0;
	}
	int img_flags = IMG_INIT_JPG||IMG_INIT_PNG;
	if(!(IMG_Init(img_flags)&img_flags)){
		printf("Error at IMG_Init: %s", IMG_GetError());
	}
	*w = SDL_CreateWindow("Main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	*surf = SDL_GetWindowSurface(*w);
	return 1;
}

int Quit_SDL(SDL_Window *w){
	SDL_DestroyWindow(w);
	w = NULL;
	IMG_Quit();
	SDL_Quit();
	return 0;
}