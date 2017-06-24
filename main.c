#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include "game.h"

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
	SDL_Texture *circle = NULL;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	TTF_Font *font = NULL;
	SDL_Color fontColor = { 255, 255, 255, 255 };
	SDL_Event e;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "\nSDL error: %s", SDL_GetError());
		return -1;
	}
	wind = SDL_CreateWindow("Main", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCR_FORCED_WIDTH, SCR_FORCED_HEIGHT,
			SDL_WINDOW_RESIZABLE);

	if ((rend = SDL_CreateRenderer(wind, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == NULL) {
		fprintf(stderr, stderr, "\nError at creating renderer: %s",
				SDL_GetError());
		return -1;
	}
	SDL_SetRenderDrawColor(rend, 0x00, 0x00, 0xff, 0x00);

	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags)) {
		fprintf(stderr, stderr, "\nError at IMG_Init: %s", IMG_GetError());
		return -1;
	}

	if (TTF_Init() != 0) {
		fprintf(stderr, "ttf initialization error: %s", TTF_GetError());
		return -1;
	}
	if ((font = TTF_OpenFont("fonts/font1.ttf", 14)) == NULL) {
		fprintf(stderr, "Can't open fonts: %s", TTF_GetError());
	}
	SDL_Texture *text = draw_text(rend, font,
			"The quick brown fox jumps over the lazy dog.", &fontColor);
	if (text == NULL) {
		fprintf(stderr, "Can't draw text: %s", TTF_GetError());
	}

	if ((circle = load_texture("images/circle.png", rend)) == NULL) {
		fprintf(stderr, "\nCan't load texture: %s", SDL_GetError());
		return -1;
	}
	SDL_Rect rect = {.x = 0, .y = 0, .w = SCR_FORCED_WIDTH, .h = SCR_FORCED_HEIGHT};
	SDL_QueryTexture(text, NULL, NULL, &(rect.w), &(rect.h));
	SDL_Rect circle_rect = { .x = 0, .y = 0, .w = 6, .h = 6 };
	SDL_QueryTexture(circle, NULL, NULL, &(circle_rect.w), &(circle_rect.h));
	//end init

	//starting game
	struct game_options go = { SCR_FORCED_WIDTH, SCR_FORCED_HEIGHT, 10, 0,
			circle_rect.w / 2.0, 0, 0};
	start_game(&go);
	int quit = 0;
	while (!quit) {
		while(SDL_PollEvent(&e)){
			quit = (e.type == SDL_QUIT);
			if(e.type == SDL_MOUSEBUTTONDOWN){
				shoot(e.button.x, e.button.y);
			}
		}
		calculate_game(SDL_GetTicks());
		SDL_RenderClear(rend);
		for (int i = 0; i < go.max_circles; ++i) {
			if (go.circles[i].active) {
				circle_rect.w = go.circles[i].r * 2;
				circle_rect.h = go.circles[i].r * 2;
				circle_rect.x = go.circles[i].x - go.circles[i].r;
				circle_rect.y = go.circles[i].y - go.circles[i].r;
				SDL_RenderCopyEx(rend, circle, NULL, &circle_rect, 0, NULL,
						flip);
			}
		}
		// SDL_RenderCopyEx(rend, text, NULL, &textRect, 0, NULL, flip);
		SDL_RenderPresent(rend);
	}
	//end game

	//clearing
	SDL_DestroyTexture(circle);
	SDL_DestroyTexture(text);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(wind);
	wind = NULL;
	rend = NULL;
	circle = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface *load_surface(char *path)
{
	SDL_Surface *s = IMG_Load(path);
	if (s == NULL) {
		fprintf(stderr, "Error loading %s: %s", path, IMG_GetError());
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
