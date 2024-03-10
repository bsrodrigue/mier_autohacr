#include "config.h"
#include <raylib.h>

#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

void draw_grid();
void render_level_editor(Camera2D *camera);
void handle_level_input(Camera2D *camera, int pressed_key);
Vector2 get_player_position(int level_grid[CELL_COUNT][CELL_COUNT]);
Vector2 get_world_mouse(Camera2D camera);
void load_level_editor(const char *filename);

#endif
