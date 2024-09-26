#include "common.h"
#include <cmath>
#include <raylib.h>

Vector2 get_offset_position(float x, float y) {
  return {(float)CELL_OFFSET(x), (float)CELL_OFFSET(y)};
}

float get_angle_relative_to(Vector2 dest, Vector2 origin) {
  float angle = atan2(dest.y - origin.y, dest.x - origin.x);
  return (angle * RAD2DEG);
}

Vector2 get_world_mouse(Camera2D camera) {
  Vector2 mouse = GetMousePosition();
  return GetScreenToWorld2D(mouse, camera);
}
