#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "config.h"
#include "enemy.h"
#include "entities.h"
#include "item.h"
#include "wall.h"
#include <raylib.h>
#include <sys/types.h>
#include <variant>

constexpr int TYPE_COUNT = 8;
constexpr float ENTITY_DROPDOWN_HEIGHT = 25;
constexpr float ENTITY_DROPDOWN_WIDTH = 300;

// Forward Declarations
struct EditorPlayer;
struct EditorWall;
struct EditorVoid;
struct EditorEnemy;
struct EditorWarpzone;
struct EditorItem;
struct EditorGate;

// Their positions are already infered from their grid indices
struct EditorPlayer {};

struct EditorVoid {
  int voidness;
};

struct EditorWall {
  WallType type;
};

struct EditorEnemy {
  EnemyType type;
};

struct EditorWarpzone {
  Vector2 destination;
};

struct EditorItem {
  ItemEffect effect;
  ItemUsage usage;
};

struct EditorGate {};

struct EditorGridCell {
  EntityType type;
  std::variant<EditorVoid, EditorWall, EditorEnemy, EditorWarpzone,
               EditorPlayer, EditorItem, EditorGate>
      entity;
};

using ItemParams = EditorItem;

class LevelEditor {
public:
  const char *filename;

  // ------------------------[ Editor State ]------------------------------ //

  bool origin_warpzone_placed = false;
  bool can_change_entity = true;
  bool placing_mode = false;

  // ------[ Entity Dropdown ]----- //

  bool entity_dropdown_is_open = false;
  EntityType current_entity = PLAYER_ENTITY;

  // ----[Items]
  ItemParams item_params;

  // ----------------------------[ Grid ]---------------------------------- //

  EditorGridCell grid[CELL_COUNT][CELL_COUNT];

  LevelEditor();
  explicit LevelEditor(const char *filename);

  void init_editor();
  void load_level_data();
  void create_level_data();
  void save_level();

  Vector2 get_player_position();

  void place_entity(Vector2 mouse);
};

void render_level_editor(Camera2D *camera);
void handle_editor_input(Camera2D *camera, int pressed_key);
void load_level_editor(const char *filename);
void render_entity_dropdown();

#endif
