#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <raylib.h>
#include <raymath.h>

#define FPS 30

#define WIN_WIDTH 600
#define WIN_HEIGHT 600

void rotate_point(Vector2 *point, Vector2 center, float angle) {
  // Translate the point to the origin
  point->x -= center.x;
  point->y -= center.y;

  // Apply rotation
  float x = point->x * cos(angle) - point->y * sin(angle);
  float y = point->x * sin(angle) + point->y * cos(angle);

  // Translate back
  point->x = x + center.x;
  point->y = y + center.y;
}

float half_len = 15;
float angle = 0;
Vector2 position = {(float)WIN_WIDTH / 2, (float)WIN_HEIGHT / 2};
Vector2 triangle_body[3] = {Vector2Add(position, {0, -half_len}),
                            Vector2Add(position, {-half_len, half_len}),
                            Vector2Add(position, {half_len, half_len})};

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

void rotate_triangle(float angle) {
  for (uint8_t i = 0; i < 3; i++) {
    rotate_point(&triangle_body[0], position, angle);
  }
}

void update_triangle_body() {
  triangle_body[0] = Vector2Add(position, {0, -half_len});
  triangle_body[1] = Vector2Add(position, {-half_len, half_len});
  triangle_body[2] = Vector2Add(position, {half_len, half_len});
}

void handle_input() {
  float velocity = 5;

  if (IsKeyDown(KEY_UP)) {
    position = Vector2Add(position, {0, -velocity});
  }
  if (IsKeyDown(KEY_DOWN)) {
    position = Vector2Add(position, {0, velocity});
  }
  if (IsKeyDown(KEY_LEFT)) {
    position = Vector2Add(position, {-velocity, 0});
  }
  if (IsKeyDown(KEY_RIGHT)) {
    position = Vector2Add(position, {velocity, 0});
  }

  if (IsKeyDown(KEY_SPACE)) {
    rotate_triangle(10);
  }
}

void render() {
  // Player
  update_triangle_body();
  DrawTriangle(triangle_body[0], triangle_body[1], triangle_body[2], WHITE);
}

int main() {
  init();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    handle_input();

    render();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
