#include "config.h"
#include <cmath>
#include <raylib.h>

void draw_cell(float x, float y, Color color) {
  DrawRectangle(CELL_OFFSET(x), CELL_OFFSET(y), CELL_SIZE - CELL_PADDING,
                CELL_SIZE - CELL_PADDING, color);
}

void draw_hover_cell(float x, float y, Color color) {
  DrawRectangle(x / CELL_SIZE, y / CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}

float get_angle_relative_to(Vector2 dest, Vector2 origin) {
  float angle = atan2(dest.y - origin.y, dest.x - origin.x);
  return (angle * RAD2DEG);
}

void draw_grid() {
  for (int i = 0; i < CELL_COUNT; i++) {
    DrawLine(CELL_OFFSET(i), 0, CELL_OFFSET(i), LEVEL_HEIGHT, WHITE);
  }

  for (int i = 0; i < CELL_COUNT; i++) {
    DrawLine(0, CELL_OFFSET(i), LEVEL_WIDTH, CELL_OFFSET(i), WHITE);
  }
}

Vector2 get_world_mouse(Camera2D camera) {
  Vector2 mouse = GetMousePosition();
  return GetScreenToWorld2D(mouse, camera);
}
