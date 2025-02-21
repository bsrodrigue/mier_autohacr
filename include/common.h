#include "config.h"
#include <raylib.h>

#ifndef COMMON_H
#define COMMON_H
float get_angle_relative_to(Vector2 dest, Vector2 origin);
Vector2 get_world_mouse(Camera2D camera);
Vector2 get_absolute_position_from_grid_position(float x, float y);
#endif
