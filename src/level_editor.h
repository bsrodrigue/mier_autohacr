#include "config.h"
#include "entities.h"
#include "save.h"
#include <raylib.h>

#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#define TYPE_COUNT 7

void draw_grid();
void render_level_editor(Camera2D *camera);
void handle_editor_input(Camera2D *camera, int pressed_key);
Vector2 get_player_position(int level_grid[CELL_COUNT][CELL_COUNT]);
Vector2 get_world_mouse(Camera2D camera);
void load_level_editor(const char *filename);

// Their positions are already infered from their grid indices
typedef struct {
  WallType type;
  bool free;
} EditorWall;

typedef struct {
  EnemyType type;
  bool free;
} EditorEnemy;

typedef struct {
  Vector2 destination;
  bool free;
} EditorWarpzone;

typedef struct {
  EntityType type;
  int index;
} EditorGridCell;

class LevelEditor {
public:
  const char *filename;

  // ------------------- Grid ----------------------------------
  EditorGridCell grid[CELL_COUNT][CELL_COUNT];

  // You can extend the editor entities how you want
  EditorEnemy enemies[MAX_FENEMIES];
  EditorWall walls[MAX_FWALLS];
  EditorWarpzone warpzones[MAX_FWARPZONES];

  int current_entity_index = 1;
  EntityType types[TYPE_COUNT] = {EMPTY,      BWALL,          UBWALL,  PLAYER,
                                  BASE_ENEMY, SENTRY_A_ENEMY, WARPZONE};

  LevelEditor();
  LevelEditor(const char *filename);

  void init_editor();
  void load_level_data();
  void create_level_data();
  void save_level();

  Vector2 get_player_position();

  template <typename T> int get_free_editor_entity(T entities[100]);
  void place_entity(EditorGridCell *cell, EntityType type);

  void next_type();
};

#endif
