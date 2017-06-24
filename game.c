#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "SDL.h"
#include "game.h"

static struct circle default_circle = { .x = 0, .y = 0, .r = 32.0f, .cost = 10,
		.move_horiz = 0, .move_vert = 0, .active = 0, .speed_vert = 100, .speed_horiz = 100};
static struct game_settings gs = { .score = 0, .circles_count = 0 };

int start_game(struct game_options *forced_go)
{
	gs.go = forced_go;
	if (!(gs.go->circles = malloc(sizeof(struct circle) * gs.go->max_circles))){
		fprintf(stderr, "Can't allocate memory");
		return -1;
	}
	srand(time(NULL));
	for (int i = 0; i < gs.go->max_circles; ++i){
		gs.go->circles[i] = default_circle;
		gs.go->circles[i].r = gs.go->circle_rad;
		gs.go->circles[i].move_horiz = rand()%3 - 1;
		gs.go->circles[i].move_vert = rand()%3 - 1;
	}
	return 0;
}

int calculate_game(Uint32 current_time)
{
	for (int i = 0; i < gs.go->max_circles; ++i){
		if (!(gs.go->circles[i].active)) {
			gs.go->circles[i].x = (float)(rand() % gs.go->field_width);
			gs.go->circles[i].y = (float)(rand() % gs.go->field_height);
			gs.go->circles[i].active = 1;
		}else{
			gs.go->circles[i].x += gs.go->circles[i].move_horiz * gs.go->circles[i].speed_horiz * (current_time - gs.go->last_timestamp) / 1000;
			gs.go->circles[i].y += gs.go->circles[i].move_vert * gs.go->circles[i].speed_vert * (current_time - gs.go->last_timestamp) / 1000;
			if(gs.go->circles[i].x < 0 || gs.go->circles[i].x > gs.go->field_width || gs.go->circles[i].y < 0 || gs.go->circles[i].x > gs.go->field_height){
				gs.go->circles[i].active = 0;
				gs.score -= gs.go->circles[i].cost;
			}
		}
	}
	gs.go->last_timestamp = current_time;
}

void shoot(int x, int y)
{
	for (int i = 0; i < gs.go->max_circles; ++i){
		if (gs.go->circles[i].active && sqrt(pow((x - gs.go->circles[i].x), 2) + pow((y - gs.go->circles[i].y), 2)) < gs.go->circles[i].r){
			gs.go->circles[i].active = 0;
			gs.score += gs.go->circles[i].cost;
			if (!(gs.go->piercing)){
				return;
			}
		}
	}
}