#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#define FPS 60

#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000

#define CELL_SIZE 25
#define CELL_COUNT (int)(WIN_HEIGHT / CELL_SIZE)
#define CELL_PADDING 5

typedef enum {
  UNKNOWN = -1,
  MENU = 0,
  GAME,
  LEVEL_EDITOR,
} Screen;

typedef enum { BREAKABLE, UNBREAKABLE } WallType;

Screen current_screen = GAME;

float velocity = 5;
float shooting_range = 25 * 5;
float last_enemy_shoot = 0;
float shooting_interval = 1;

typedef struct {
  WallType type;
  float health;
  Vector2 position;
} Wall;

std::vector<Wall> walls;

Wall create_ubreakable_wall(Vector2 position) {
  Wall wall;
  wall.type = UNBREAKABLE;
  wall.health = 0;
  wall.position = position;

  return wall;
}

void draw_cell(float x, float y, Color color) {
  DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE - CELL_PADDING,
                CELL_SIZE - CELL_PADDING, color);
}

void generate_walls() {
  for (float i = 0; i < CELL_COUNT; i++) {
    Wall wall_up = create_ubreakable_wall({i, 0});
    Wall wall_down = create_ubreakable_wall({i, CELL_COUNT - 1});
    walls.push_back(wall_up);
    walls.push_back(wall_down);
  }

  for (float i = 0; i < CELL_COUNT; i++) {
    Wall wall_up = create_ubreakable_wall({0, i});
    Wall wall_down = create_ubreakable_wall({CELL_COUNT - 1, i});
    walls.push_back(wall_up);
    walls.push_back(wall_down);
  }
}

void load_level() { generate_walls(); }

void draw_arena() {
  for (int i = 0; i < walls.size(); i++) {
    Vector2 position = walls[i].position;
    draw_cell(position.x, position.y, WHITE);
  }
}

typedef struct {
  Vector2 position;
  Vector2 velocity;
  Vector2 direction;
  std::vector<Vector2> shape;
} GameObject;

typedef struct {
  int id;
  Vector2 pos;
  Vector2 direction;
  bool is_shooting;
} Projectile;

float half_len = 15;
float projectile_speed = 12.5;

GameObject player = {.position{(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2},
                     .velocity{0, 0},
                     .direction{0, 0}};

std::vector<Projectile> projectiles;

void define_shape(GameObject *game_obj) {
  // For the moment we will handle triangles only
  game_obj->shape.push_back(Vector2Add(game_obj->position, {0, -half_len}));
  game_obj->shape.push_back(
      Vector2Add(game_obj->position, {-half_len, half_len}));
  game_obj->shape.push_back(
      Vector2Add(game_obj->position, {half_len, half_len}));
}

void update_shape(GameObject *game_obj) {
  game_obj->shape[0] = Vector2Add(game_obj->position, {0, -half_len});
  game_obj->shape[1] = Vector2Add(game_obj->position, {-half_len, half_len});
  game_obj->shape[2] = Vector2Add(game_obj->position, {half_len, half_len});
}

void draw_game_obj(GameObject game_obj, Color color) {
  DrawTriangle(game_obj.shape[0], game_obj.shape[1], game_obj.shape[2], color);
}

void die(const char *message) {
  perror(message);
  exit(1);
}

void init() {
  InitWindow(WIN_WIDTH, WIN_HEIGHT, "NieR: AutoHacker");

  if (!IsWindowReady())
    die("Failed to initialize window\n");

  SetTargetFPS(FPS);

  ShowCursor();
}

void player_shoot(int pressed_key) {
  Vector2 mouse = GetMousePosition();

  Projectile new_projectile;

  new_projectile.pos = player.position;
  new_projectile.direction =
      Vector2Normalize(Vector2Subtract(mouse, player.position));

  new_projectile.id = projectiles.size();

  projectiles.push_back(new_projectile);
}

void handle_single_press_input(int pressed_key) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    player_shoot(pressed_key);
}

bool check_wall_collision(Vector2 position) {
  for (int i = 0; i < walls.size(); i++) {
    Wall wall = walls[i];
    Rectangle rect;

    rect.x = wall.position.x * CELL_SIZE;
    rect.y = wall.position.y * CELL_SIZE;

    rect.width = CELL_SIZE;
    rect.height = CELL_SIZE;

    // DEBUG:
    // DrawRectangleRec(rect, RED);

    if (CheckCollisionPointRec(position, rect)) {
      // DEBUG:
      // TraceLog(LOG_INFO, "WALL_COLLISION");
      return true;
    }
  }

  return false;
}

void handle_player_movement() {
  Vector2 vertical_movement = {0, 0};
  Vector2 horizontal_movement = {0, 0};

  bool can_move_vertical = true;
  bool can_move_horizontal = true;

  if (IsKeyDown(KEY_UP)) {
    vertical_movement = {0, -velocity};
  } else if (IsKeyDown(KEY_DOWN)) {
    vertical_movement = {0, velocity};
  }

  if (IsKeyDown(KEY_LEFT)) {
    horizontal_movement = {-velocity, 0};
  } else if (IsKeyDown(KEY_RIGHT)) {
    horizontal_movement = {velocity, 0};
  }

  Vector2 next_vertical_position =
      Vector2Add(player.position, vertical_movement);

  if (check_wall_collision(next_vertical_position)) {
    can_move_vertical = false;
  }

  Vector2 next_horizontal_position =
      Vector2Add(player.position, horizontal_movement);

  if (check_wall_collision(next_horizontal_position)) {
    can_move_horizontal = false;
  }

  if (can_move_vertical) {
    player.position = Vector2Add(player.position, vertical_movement);
  }

  if (can_move_horizontal) {
    player.position = Vector2Add(player.position, horizontal_movement);
  }
}

void handle_game_input(int pressed_key) {
  handle_single_press_input(pressed_key);
  handle_player_movement();
}

void handle_global_input() {}

void handle_input(int pressed_key) {
  handle_global_input();

  switch (current_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    handle_game_input(pressed_key);
    break;
  case LEVEL_EDITOR:
    break;
  }
}

void update_projectiles() {
  for (int i = 0; i < projectiles.size(); i++) {
    Vector2 projectile_pos =
        Vector2Add(projectiles[i].pos,
                   Vector2Multiply(projectiles[i].direction,
                                   {projectile_speed, projectile_speed}));
    DrawCircleV(projectile_pos, 10, BLUE);
    projectiles[i].pos = projectile_pos;
  }
}

void render_game() {
  draw_arena();
  draw_game_obj(player, WHITE);
}

void render_game_debug() {
  Vector2 mouse = GetMousePosition();
  DrawLineV(player.position, mouse, BLUE);
}

void render() {
  switch (current_screen) {
  case UNKNOWN:
    break;
  case MENU:
    break;
  case GAME:
    render_game_debug();
    render_game();
    break;
  case LEVEL_EDITOR:
    break;
  }
}

int main() {
  init();
  load_level();
  define_shape(&player);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    int pressed_key = GetKeyPressed();

    handle_input(pressed_key);

    render();
    update_shape(&player);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
