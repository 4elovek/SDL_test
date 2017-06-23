#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

static int SCR_FORCED_WIDTH = 1280;
static int SCR_FORCED_HEIGHT = 720;

SDL_Surface *load_surface(char* path);
SDL_Surface *optimize_surface(SDL_Surface *opt, SDL_Surface *formatter);
SDL_Texture *load_texture(char* path, SDL_Renderer *r);
SDL_Texture *draw_text(SDL_Renderer *rend, TTF_Font *font, char *text, SDL_Color *color);

int main(int argc, char* argv[])
{
	//init
	SDL_Window *wind = NULL;
	SDL_Renderer *rend = NULL;
	SDL_Texture *texture = NULL;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	TTF_Font *font = NULL;
	SDL_Color fontColor = {255, 255, 255, 255};
	SDL_Event e;
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("\nSDL error: %s", SDL_GetError());
		return -1;
	}
	wind = SDL_CreateWindow("Main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_FORCED_WIDTH, SCR_FORCED_HEIGHT, SDL_WINDOW_RESIZABLE);
	if((rend = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL){
			printf("\nError at creating renderer: %s", SDL_GetError());
			return -1;
	}
	SDL_SetRenderDrawColor(rend, 0xff, 0x00, 0x0, 0x0);
	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if(!(IMG_Init(img_flags)&img_flags)){
		printf("\nError at IMG_Init: %s", IMG_GetError());
		return -1;
	}
	if(TTF_Init() != 0){
		printf("ttf initialization error: %s", TTF_GetError());
		return -1;
	}
	if((font = TTF_OpenFont("font1.ttf", 14)) == NULL){
		printf("Can't open fonts: %s", TTF_GetError());
	}
	SDL_Texture *text = draw_text(rend, font, "The quick brown fox jumps over the lazy dog.", &fontColor);
	if(text == NULL){
		printf("Can't draw text: %s", TTF_GetError());
	}
	if((texture = load_texture("shodan.jpg", rend)) == NULL){
		printf("\nCan't load texture: %s", SDL_GetError());
		return -1;
	}
	//end init
	SDL_Rect rect = {.x = 0, .y = 0, .w = SCR_FORCED_WIDTH, .h = SCR_FORCED_HEIGHT};
	SDL_Rect textRect = {.x = 0, .y = 0, .w = 0, .h = 0};
	int w = 0;
	int h = 0;
	double angle = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_QueryTexture(text, NULL, NULL, &(textRect.w), &(textRect.h));
	int quit = 0;
	while(!quit){
		while(SDL_PollEvent(&e)){
			quit = (e.type == SDL_QUIT);
			if(e.type == SDL_KEYDOWN){
				if(e.key.keysym.sym == SDLK_LEFT){
					if((rect.x -= 10) < 0){
						rect.x = 0;
					}
				}else if(e.key.keysym.sym == SDLK_RIGHT){
					if((rect.x += 10) + rect.w > w){
						rect.x = w - rect.w;
					}
				}
				if(e.key.keysym.sym == SDLK_UP){
					if((rect.y -= 10) < 0){
						rect.y = 0;
					}
				}else if(e.key.keysym.sym == SDLK_DOWN){
					if((rect.y += 10) + rect.h > h){
						rect.y = h - rect.h;
					}
				}
				if(e.key.keysym.sym == SDLK_a){
					angle += 10;
				}else if(e.key.keysym.sym == SDLK_d){
					angle -= 10;
				}
			}
		}
		SDL_RenderClear(rend);
		SDL_RenderCopyEx(rend, texture, &rect, NULL, angle, NULL, flip);
		SDL_RenderCopyEx(rend, text, NULL, &textRect, angle, NULL, flip);
		SDL_RenderPresent(rend);
	}
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(text);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(wind);
	wind = NULL;
	rend = NULL;
	texture = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface *load_surface(char *path)
{
	SDL_Surface *s = IMG_Load(path);
	if(s == NULL){
		printf("Error loading %s: %s", path, IMG_GetError());
	}
	return s;
}

SDL_Surface *optimize_surface(SDL_Surface *opt, SDL_Surface *formatter)
{
	SDL_Surface *ret = SDL_ConvertSurface(opt, formatter->format, NULL);
	SDL_FreeSurface(opt);
	return ret;
}

SDL_Texture *load_texture(char* path, SDL_Renderer *r)
{
	SDL_Surface *s = load_surface(path);
	SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
	SDL_FreeSurface(s);
	return t;
}
SDL_Texture *draw_text(SDL_Renderer *rend, TTF_Font *font, char *text, SDL_Color *color)
{
	SDL_Surface *surf = TTF_RenderText_Solid(font, text, *color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surf);
	SDL_FreeSurface(surf);
	return texture;
}