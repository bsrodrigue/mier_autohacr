#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#define FPS 30

#define WIN_WIDTH 600
#define WIN_HEIGHT 600

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
float projectile_speed = 8.5;

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

void draw_game_obj(GameObject game_obj) {
  DrawTriangle(game_obj.shape[0], game_obj.shape[1], game_obj.shape[2], WHITE);
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

void handle_input() {
  float velocity = 5;

  if (IsKeyDown(KEY_UP)) {
    player.position = Vector2Add(player.position, {0, -velocity});
  }
  if (IsKeyDown(KEY_DOWN)) {
    player.position = Vector2Add(player.position, {0, velocity});
  }
  if (IsKeyDown(KEY_LEFT)) {
    player.position = Vector2Add(player.position, {-velocity, 0});
  }
  if (IsKeyDown(KEY_RIGHT)) {
    player.position = Vector2Add(player.position, {velocity, 0});
  }
}

void handle_single_press_input(int pressed_key) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse = GetMousePosition();

    Projectile new_projectile;

    new_projectile.pos = player.position;
    new_projectile.direction =
        Vector2Normalize(Vector2Subtract(mouse, player.position));

    new_projectile.id = projectiles.size();

    projectiles.push_back(new_projectile);
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

void render() {
  draw_game_obj(player);
  update_projectiles();
}

int main() {
  init();
  define_shape(&player);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int pressed_key = GetKeyPressed();

    handle_input();
    handle_single_press_input(pressed_key);

    Vector2 mouse = GetMousePosition();

    DrawLineV(player.position, Vector2Zero(), RED);
    DrawLineV(player.position, mouse, BLUE);
    DrawLineV(mouse, Vector2Zero(), RED);

    DrawText(TextFormat("%f", Vector2Angle(player.position, mouse)), 10, 10, 14,
             WHITE);

    render();
    update_shape(&player);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
