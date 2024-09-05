#include "config.h"
#include "enemy.h"
#include "entities.h"
#include "item.h"
#include "wall.h"
#include <raylib.h>

#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#define TYPE_COUNT 8

// Their positions are already infered from their grid indices
typedef struct {
} EditorPlayer;

typedef struct {
  WallType type;
} EditorWall;

typedef struct {
  EnemyType type;
} EditorEnemy;

typedef struct {
  Vector2 destination;
} EditorWarpzone;

typedef struct {
  ItemEffect effect;
} EditorItem;

typedef struct {
  EntityType type;
  union {
    EditorWall wall;
    EditorEnemy enemy;
    EditorWarpzone warpzone;
    EditorPlayer player;
    EditorItem item;
  };
} EditorGridCell;

class LevelEditor {
public:
  const char *filename;

  // ------------------------[ Editor State ]------------------------------ //

  bool origin_warpzone_placed = false;
  bool can_change_entity = true;

  // ----------------------------[ Grid ]---------------------------------- //

  EditorGridCell grid[CELL_COUNT][CELL_COUNT];

  int current_entity_index = 1;
  EntityType types[TYPE_COUNT] = {EMPTY,      BWALL,          UBWALL,   PLAYER,
                                  BASE_ENEMY, SENTRY_A_ENEMY, WARPZONE, ITEM};

  LevelEditor();
  LevelEditor(const char *filename);

  void init_editor();
  void load_level_data();
  void create_level_data();
  void save_level();

  Vector2 get_player_position();

  template <typename T> int get_free_editor_entity(T entities[100]);
  void place_entity(Vector2 mouse);

  void next_type();
};

void draw_grid();
void render_level_editor(Camera2D *camera);
void handle_editor_input(Camera2D *camera, int pressed_key);
Vector2 get_player_position(EditorGridCell grid[CELL_COUNT][CELL_COUNT]);
Vector2 get_world_mouse(Camera2D camera);
void load_level_editor(const char *filename);

#endif
