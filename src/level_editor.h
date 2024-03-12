#include "config.h"
#include "entities.h"
#include <raylib.h>

#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

typedef struct {
  EntityType type;
  int index;
} LevelGridCell;

class LevelEditor {
public:
  LevelGridCell grid[CELL_COUNT][CELL_COUNT];
  LevelEditor();

  void init_grid();
};

void draw_grid();
void render_level_editor(Camera2D *camera);
void handle_level_input(Camera2D *camera, int pressed_key);
Vector2 get_player_position(int level_grid[CELL_COUNT][CELL_COUNT]);
Vector2 get_world_mouse(Camera2D camera);
void load_level_editor(const char *filename);

#endif
