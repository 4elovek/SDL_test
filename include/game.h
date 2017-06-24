static struct circle{
	float x;
	float y;
	float r;
	int cost;
	int move_horiz;
	int move_vert;
	float speed_vert;// pixel per sec
	float speed_horiz;
	int active:1;
	int props[3];
};
struct game_options{
	int field_width;
	int field_height;
	int max_circles;
	int piercing:1;
	float circle_rad;
	struct circle *circles;
	Uint32 last_timestamp;
};

static struct game_settings{
	int score;
	struct game_options *go;
	int circles_count;
};

int start_game(struct game_options *forced_go);
int calculate_game(Uint32 current_time);
void shoot(int x, int y);