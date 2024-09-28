#include "config.h"
#include "enemy.h"
#include "entities.h"
#include "item.h"
#include "wall.h"
#include <raylib.h>
#include <sys/types.h>

#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#define TYPE_COUNT 8

#define ENTITY_DROPDOWN_HEIGHT 25
#define ENTITY_DROPDOWN_WIDTH 300

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
  ItemUsage usage;
} EditorItem;

typedef struct {
} EditorGate;

typedef struct {
  EntityType type;
  union {
    EditorWall wall;
    EditorEnemy enemy;
    EditorWarpzone warpzone;
    EditorPlayer player;
    EditorItem item;
    EditorGate gate;
  };
} EditorGridCell;

typedef struct {
  ItemEffect effect;
  ItemUsage usage;
} ItemParams;

class LevelEditor {
public:
  const char *filename;

  // ------------------------[ Editor State ]------------------------------ //

  bool origin_warpzone_placed = false;
  bool can_change_entity = true;
  bool placing_mode = false;

  // ------[ Entity Dropdown ]----- //

  bool entity_dropdown_is_open = false;
  EntityType current_entity = EMPTY_ENTITY;

  // --- [Items]
  ItemParams item_params;

  // ----------------------------[ Grid ]---------------------------------- //

  EditorGridCell grid[CELL_COUNT][CELL_COUNT];

  LevelEditor();
  LevelEditor(const char *filename);

  void init_editor();
  void load_level_data();
  void create_level_data();
  void save_level();

  Vector2 get_player_position();

  template <typename T> int get_free_editor_entity(T entities[100]);
  void place_entity(Vector2 mouse);
};

void render_level_editor(Camera2D *camera);
void handle_editor_input(Camera2D *camera, int pressed_key);
void load_level_editor(const char *filename);
void render_entity_dropdown();

Vector2 get_player_position(EditorGridCell grid[CELL_COUNT][CELL_COUNT]);

#endif
