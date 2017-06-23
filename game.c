#include <stdlib.h>
#include <stdio.h>

static struct circle{
	int x;
	int y;
	float r;
	int cost;
	int move_horiz;
	int move_vert;
	int active:1;
	int props[3];
	
} default_circle = {.x = 0, .y = 0, .r = 20.0f, .active = 0};

struct game_options{
	int field_width;
	int field_height;
	int max_circles;
	int fps_cap;
	int piercing:1;
};
static struct game_settings{
	int score;
	struct game_options go;
	int circles_count;
	struct circle *circles;
	
} gs = {.score = 0, .max_circles = 10, .circles = NULL, .circles_count = 0};

int start_game()
{
	gs.go = forced_go;
	if(!(gs.circles = malloc(sizeof(struct circle) * gs.go.max_circles))){
		fprintf(stderr, "Can't allocate memory");
		return -1;
	}
	for(int i = 0; i < gs.max_circles; ++i){
		gs.circles[i] = default_circle;
	}
	
}

void shoot(int x, int y)
{
	for (int i = 0; i < gs.go.max_circles; ++i){
		 if(gs.circles[i].active && sqrt(pow((x - gs.circles[i].x), 2) + pow((y - gs.circles[i].y), 2)) < gs.circles[i].r){
			 gs.circles[i].active = 0;
			 gs.score += gs.circles[i].cost;
			 if(!gs.go.piercing)
				 return;
		 }
	}

}








