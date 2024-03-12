#include <raylib.h>

#ifndef COMMON_H
#define COMMON_H
void draw_cell(float x, float y, Color color);
void draw_hover_cell(float x, float y, Color color);
float get_angle_relative_to(Vector2 dest, Vector2 origin);
Vector2 get_world_mouse(Camera2D camera);
void draw_grid();
#endif
